#pragma once
#if !defined( ENGINE_DISABLE_NETWORK )
#include <queue>
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/EventSystem.hpp"
struct NetSystemConfig
{
	std::string m_modeString;
	std::string m_hostAddressString;
	int m_sendBufferSize = 2048;
	int m_recvBufferSize = 2048;
};
enum class Mode
{
	NONE = 0,
	CLIENT,
	SERVER
};
enum class ClientState 
{
	DISCONNECTED,
	CONNECTING,
	CONNECTED
};

enum class ServerState 
{
	NOT_LISTENING,
	LISTENING,
	CLIENT_CONNECTED
};


class NetSystem
{
public:
	NetSystem(NetSystemConfig const& config);
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();
	bool IsEnabled() const;
	bool IsServer() const;
	bool IsClient() const;
	bool IsConnected() const;
	void Send(std::string const& data);
	void Send(std::string const& data, EventArgs& args);
	void Disconnected();
	NetSystemConfig GetNetSystemConfig() const;
private:
	void ProcessData();
	void ParseNetworkString(std::string const& netWorkString);
private:
	NetSystemConfig m_config;
	Mode m_mode = Mode::NONE;
	ClientState m_clientState = ClientState::DISCONNECTED;
	ServerState m_serverState = ServerState::NOT_LISTENING;
	uintptr_t m_clientSocket = ~0ull;
	uintptr_t m_listenSocket = ~0ull;
	unsigned long m_hostAddress = 0;
	unsigned short m_hostPort = 0;
	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;
	bool m_isReadyToDisconnected = false;
	std::vector<char> m_sendQueue;
	std::vector<char> m_recvQueue;
};
#endif