#if !defined( ENGINE_DISABLE_NETWORK )

#include "Engine/Core/NetSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

NetSystem::NetSystem(NetSystemConfig const& config)
	:m_config(config)
{
	if (IsEqualWithoutProperCase(m_config.m_modeString, "Client"))
	{
		m_mode = Mode::CLIENT;
	}
	else if (IsEqualWithoutProperCase(m_config.m_modeString, "Server"))
	{
		m_mode = Mode::SERVER;
	}
	else
	{
		m_mode = Mode::NONE;
	}
}

void NetSystem::Startup()
{
	m_sendBuffer = new char[m_config.m_sendBufferSize];
	m_recvBuffer = new char[m_config.m_recvBufferSize];
	WSADATA data;			
	int result = WSAStartup(MAKEWORD(2, 2), &data);
	if (result == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		switch (errorCode) 
		{
		case WSAVERNOTSUPPORTED:
			ERROR_AND_DIE("Net System Startup failed: Requested version is not supported.");
			break;
		case WSAEINPROGRESS:
			ERROR_AND_DIE("Net System Startup failed: A blocking operation is in progress.");
			break;
		case WSAEALREADY:
			ERROR_AND_DIE("Net System Startup failed: The operation is already in progress.");
			break;
		case WSAEFAULT:
			ERROR_AND_DIE("Net System Startup failed: The data is not a valid pointer.");
			break;
		default:
			ERROR_AND_DIE(Stringf("Net System Startup failed. Error code:%d", errorCode));
			break;
		}
	}
	if (IsServer())
	{
		m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		unsigned long blockingMode = 1;
		ioctlsocket(m_listenSocket, FIONBIO, &blockingMode);

		Strings netWorkStrings = SplitStringOnDelimiter(m_config.m_hostAddressString, ":",true);
		m_hostAddress = INADDR_ANY;
		if (netWorkStrings.size() == 2)
		{
			m_hostPort = (unsigned short)(atoi(netWorkStrings[1].c_str()));
		}
		else
		{
			ERROR_AND_DIE(Stringf("NetWork host address string %s is not in the correct format.",m_config.m_hostAddressString.c_str()));
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
		addr.sin_port = htons(m_hostPort);

		result = bind(m_listenSocket, (sockaddr*)&addr, (int)sizeof(addr));
		if (result == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			switch (errorCode)
			{
			case WSAEADDRINUSE:
				ERROR_AND_DIE("Net System bind failed: Address already in use.");
				break;
			case WSAEADDRNOTAVAIL:
				ERROR_AND_DIE("Net System bind failed: Address not available.");
				break;
			case WSAEFAULT:
				ERROR_AND_DIE("Net System bind failed: The address is not a valid pointer.");
				break;
			case WSAEINVAL:
				ERROR_AND_DIE("Net System bind failed: Invalid argument.");
				break;
			case WSAENOTSOCK:
				ERROR_AND_DIE("Net System bind failed: Not a socket.");
				break;
			default:
				ERROR_AND_DIE(Stringf("Net System bind failed. Error code:%d", errorCode));
				break;
			}
		}
		
		listen(m_listenSocket, SOMAXCONN);

	}
}

void NetSystem::BeginFrame()
{
	if (IsServer())
	{
		if (m_clientSocket == INVALID_SOCKET) 
		{
			m_clientSocket = accept(m_listenSocket, NULL, NULL);
			
			if (m_clientSocket != INVALID_SOCKET) 
			{
				unsigned long blockingMode = 1;
				ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
				m_serverState = ServerState::CLIENT_CONNECTED;
			}
			else
			{
				m_serverState = ServerState::LISTENING;
			}
		}
		else 
		{
			//If we have a connected client socket, send all queued outgoing data and receive all incoming data.
			ProcessData();

		}
 		switch (m_serverState)
 		{
 		case ServerState::NOT_LISTENING:
 			PrintLineToDebug("Server state: Not Listening");
 			break;
 		case ServerState::LISTENING:
 			PrintLineToDebug("Server state: Listening");
 			break;
 		case ServerState::CLIENT_CONNECTED:
 			PrintLineToDebug("Server state: Client Connected");
 			break;
 
 		}
		
	}
	else if (IsClient())
	{
		if (m_clientSocket == INVALID_SOCKET)
		{
			m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_clientSocket == INVALID_SOCKET) 
			{
				int errorCode = WSAGetLastError();
				ERROR_AND_DIE(Stringf("Failed to create client socket. Error Code:%d",errorCode));
			}
			unsigned long blockingMode = 1;
			ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
			ParseNetworkString(m_config.m_hostAddressString);
		}
		if (m_clientState == ClientState::DISCONNECTED) 
		{
			m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			unsigned long blockingMode = 1;
			ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
			addr.sin_port = htons(m_hostPort);
			int result = connect(m_clientSocket, (sockaddr*)&addr, (int)sizeof(addr));
			if (result == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();
				if (errorCode == WSAEWOULDBLOCK) 
				{
					m_clientState = ClientState::CONNECTING;
				}
				else 
				{
					PrintLineToDebug(Stringf("Network address and port invalid. %s", m_config.m_hostAddressString.c_str()).c_str());
				}
			}
			else
			{
				m_clientState = ClientState::CONNECTING;
			}
		}
		else if (m_clientState == ClientState::CONNECTING)
		{
			fd_set writeSockets;
			fd_set exceptSockets;
			FD_ZERO(&writeSockets);
			FD_ZERO(&exceptSockets);
			FD_SET(m_clientSocket, &writeSockets);
			FD_SET(m_clientSocket, &exceptSockets);
			timeval waitTime = {};
			int result = select(0, NULL, &writeSockets, &exceptSockets, &waitTime);
			if (result == SOCKET_ERROR)
			{
				
				ERROR_AND_DIE(Stringf("Failed to check connection status. Error Code:%d",WSAGetLastError()));
			}
			else if (result == 0) 
			{
				// Do nothing and keep checking the socket status each frame
			}
			else 
			{
				if (FD_ISSET(m_clientSocket, &exceptSockets))
				{
					closesocket(m_clientSocket);
					m_clientSocket = INVALID_SOCKET;
					m_clientState = ClientState::DISCONNECTED;
				}
				else if (FD_ISSET(m_clientSocket, &writeSockets))
				{
					m_clientState = ClientState::CONNECTED;
				}
			}
		}
		else if (m_clientState == ClientState::CONNECTED)
		{
			// If we have a connected client socket, send all queued outgoing data and receive all incoming data
			ProcessData();
		}
		switch (m_clientState)
		{
		case ClientState::DISCONNECTED:
			PrintLineToDebug("Client State: Disconnected");
			break;
		case ClientState::CONNECTING:
			PrintLineToDebug("Client State: Connecting");
			break;
		case ClientState::CONNECTED:
			PrintLineToDebug("Client State: Connected");
			break;
		}
	}
}

void NetSystem::EndFrame()
{
	if (m_isReadyToDisconnected)
	{
		BeginFrame();
		Shutdown();
		m_isReadyToDisconnected = false;
		m_mode = Mode::NONE;
		Startup();
	}
}

void NetSystem::Shutdown()
{
	if (IsServer())
	{
		closesocket(m_clientSocket);
		closesocket(m_listenSocket);
		
	}
	else if (IsClient())
	{
		closesocket(m_clientSocket);
	}
	WSACleanup();
	delete[] m_sendBuffer;
	m_sendBuffer = nullptr;
	delete[] m_recvBuffer;
	m_recvBuffer = nullptr;
}

bool NetSystem::IsEnabled() const
{
	return m_mode != Mode::NONE;
}

bool NetSystem::IsServer() const
{
	return m_mode == Mode::SERVER;
}

bool NetSystem::IsClient() const
{
	return m_mode == Mode::CLIENT;
}

bool NetSystem::IsConnected() const
{
	if (IsClient())
	{
		return m_clientState == ClientState::CONNECTED;
	}
	else if (IsServer())
	{
		return m_serverState == ServerState::CLIENT_CONNECTED;
	}
	return false;
}

void NetSystem::Send(std::string const& data)
{
	for (char c: data)
	{
		m_sendQueue.push_back(c);
	}
	
	m_sendQueue.push_back('\0');
}

void NetSystem::Send(std::string const& data, EventArgs& args)
{
	for (char c: data)
	{
		m_sendQueue.push_back(c);
	}
	
	auto properties = args.GetProperties();
	for (auto property : properties)
	{
		m_sendQueue.push_back(' ');
		std::string propertyString = property.first.GetOriginalString()+'='+ property.second->GetAsString();
		for (char c: propertyString)
		{
			m_sendQueue.push_back(c);
		}
	}

	m_sendQueue.push_back('\0');
}

void NetSystem::Disconnected()
{
	m_isReadyToDisconnected = true;
}

NetSystemConfig NetSystem::GetNetSystemConfig() const
{
	return m_config;
}

void NetSystem::ProcessData()
{
// 	if (IsClient())
// 	{
// 		Send("ClientHelp");
// 		Send("Yeah");
// 	}
// 	if (IsServer())
// 	{
// 		Send("ServerHelp");
// 	}

	int result = 0;

	//Send Data
	while (!m_sendQueue.empty())
	{
		size_t lastFoundNull = 0;
		bool messageSent = false;
		for (size_t i = 0; i < m_sendQueue.size(); ++i)
		{
			if (m_sendQueue[i] == '\0') 
			{
				size_t messageLength = i - lastFoundNull + 1;

				
				if (messageLength > m_config.m_sendBufferSize) 
				{
					messageLength = m_config.m_sendBufferSize;
				}

				std::copy(m_sendQueue.begin() + lastFoundNull, m_sendQueue.begin() + lastFoundNull + messageLength, m_sendBuffer);

				result = send(m_clientSocket, m_sendBuffer, (int)messageLength, 0);
				if (result > 0)
				{
					// Remove the sent message from the queue
					m_sendQueue.erase(m_sendQueue.begin() + lastFoundNull, m_sendQueue.begin() + lastFoundNull + messageLength);
					messageSent = true;
					break; 
				}
				else 
				{
					int errorCode = WSAGetLastError();
					if (errorCode == WSAEWOULDBLOCK) 
					{
						// The send operation would block, try again later
						return;
					}
					else if (errorCode == WSAECONNABORTED || errorCode == WSAECONNRESET)
					{
						if (IsServer())
						{
							m_serverState = ServerState::LISTENING;
						}
						else if (IsClient())
						{
							m_clientState = ClientState::DISCONNECTED;
						}
						closesocket(m_clientSocket);
						m_clientSocket = INVALID_SOCKET;
					}
					else 
					{
						PrintLineToDebug(Stringf("Send data failed with error code %d", errorCode));
						return;
					}
				}
			}
		}
		if (!messageSent) 
		{
			break;
		}
	}

	//Receive Data
	result = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);
	if (result == 0) 
	{
		// Client or Server socket has closed
		if (IsServer()) 
		{
			m_serverState = ServerState::LISTENING;
		}
		else if (IsClient()) 
		{
			m_clientState = ClientState::DISCONNECTED;
		}
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}
	else if (result == SOCKET_ERROR) 
	{
		int errorCode = WSAGetLastError();
		if (errorCode == WSAECONNABORTED || errorCode == WSAECONNRESET)
		{
			if (IsServer()) 
			{
				m_serverState = ServerState::LISTENING;
			}
			else if (IsClient()) 
			{
				m_clientState = ClientState::DISCONNECTED;
			}
			closesocket(m_clientSocket);
			m_clientSocket = INVALID_SOCKET;
		}
	}
	else if (result > 0)
	{
		m_recvQueue.insert(m_recvQueue.end(), m_recvBuffer, m_recvBuffer + result);
		auto it = m_recvQueue.begin();
		while ((it = std::find(it, m_recvQueue.end(), '\0')) != m_recvQueue.end()) 
		{
			std::string message(m_recvQueue.begin(), it);
			g_theConsole->Execute(message);
			it = m_recvQueue.erase(m_recvQueue.begin(), it + 1);
		}
		//PrintLineToDebug(Stringf("Received message: %s", m_recvQueue));
	}
}

void NetSystem::ParseNetworkString(std::string const& netWorkString)
{
	Strings netWorkStrings = SplitStringOnDelimiter(netWorkString, ":",true);
	if (netWorkStrings.size() == 2)
	{
		IN_ADDR addr;
		int result = inet_pton(AF_INET, netWorkStrings[0].c_str(), &addr);
		if (result == 1) 
		{
			m_hostAddress = ntohl(addr.S_un.S_addr);
			m_hostPort = (unsigned short)(atoi(netWorkStrings[1].c_str()));
		}
		else 
		{
			switch (result) 
			{
			case 0:
				ERROR_AND_DIE(Stringf("NetWork address string %s is not a valid IP address.",netWorkStrings[0].c_str()));
				break;
			case -1:
				ERROR_AND_DIE(Stringf("NetWork address string %s is not in the correct format.",netWorkStrings[0].c_str()));
				break;
			default:
				ERROR_AND_DIE("Unknown error parsing network address string.");
				break;
			}
		}
	}
// 	else
// 	{
// 		ERROR_AND_DIE("NetWork address and port string format is wrong");
// 	}
}

#endif