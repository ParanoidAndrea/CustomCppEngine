#include "Engine/Window/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#pragma  warning(disable : 26819)
Window* Window::s_theWindow = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(windowHandle, wmMessageCode, wParam, lParam))
	{
		return true;
	}
	Window* window = Window::GetMainWindowInstance();
	GUARANTEE_OR_DIE(window != nullptr, "Window was null!");

	InputSystem* input = window->GetConfig().m_inputSystem;
	switch (wmMessageCode)
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
	    case WM_CLOSE:
	    {
			
			//ERROR_RECOVERABLE("Proper App closing (through 'Close' or clicking X, etc.) is not yet supported!");
			EventArgs args;
			//args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("Quit", args);
			return 0;
	    }
	    
	    // Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
	    case WM_KEYDOWN:
	    {
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyPressed", args);
			return 0;
	    }

	// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
	    case WM_KEYUP:
	    {
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyReleased", args);
			return 0;
	    }
		case WM_CHAR:
		{
			EventArgs args;
			int keyCode = (int)wParam;
			if ((keyCode < 32 || keyCode> 126) && keyCode == KEYCODE_TILDE && keyCode == 96 )
			{
				return 0;
			}
			args.SetValue("CharCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("CharInput", args);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_LEFT_MOUSE;
			if (input)
			{
                EventArgs args;
                args.SetValue("KeyCode", Stringf("%d", keyCode));
                FireEvent("KeyPressed", args);
				return 0;
			}
		}
		case WM_LBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_LEFT_MOUSE;
			if (input)
			{
                EventArgs args;
                args.SetValue("KeyCode", Stringf("%d", keyCode));
                FireEvent("KeyReleased", args);
                return 0;
			}
		}
		case WM_RBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_RIGHT_MOUSE;

			if (input)
			{
                EventArgs args;
                args.SetValue("KeyCode", Stringf("%d", keyCode));
                FireEvent("KeyPressed", args);
                return 0;
			}
		}
		case WM_RBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_RIGHT_MOUSE;

			if (input)
			{
                EventArgs args;
                args.SetValue("KeyCode", Stringf("%d", keyCode));
                FireEvent("KeyReleased", args);
                return 0;
			}
		}
		case WM_MOUSEWHEEL:
		{
			int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			int scrollDirection = (wheelDelta > 0) ? 1 : -1;
			EventArgs args;
			args.SetValue("ScrollDirection", Stringf("%d", scrollDirection));
			FireEvent("MouseScroll", args);
			return 0;
		}
	}
	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}
Window::Window(WindowConfig const& config)
	:m_config(config)
{   
	s_theWindow = this;
	
}

Window::~Window()
{
}

void Window::StartUp()
{
	CreateOSWindow();
}

void Window::BeginFrame()
{
	RunMessagePump();
}

void Window::EndFrame()
{
	//Sleep(1);
	//SwapBuffers(reinterpret_cast<HDC>(m_displayContext));
}

void Window::ShutDown()
{
}

WindowConfig const& Window::GetConfig() const
{
	return m_config;
}

Window* Window::GetMainWindowInstance()
{
	return s_theWindow;
}


HDC const& Window::GetDisplayContext()
{
	m_displayContext = GetDC(m_windowHandle);
	return m_displayContext;
}

void Window::CreateOSWindow()
{
	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	constexpr float maxClientFractionOfDesktop = 0.9f;
	float clientWidth = 0.f;
	float clientHeight = 0.f;
	if (m_config.m_isFullscreen == false)
	{
		if (m_config.m_screenSize != IntVec2(-1, -1))
		{
			clientWidth = (float)m_config.m_screenSize.x;
			clientHeight = (float)m_config.m_screenSize.y;
			m_config.m_aspectRatio = clientWidth / clientHeight;
		}
		else
		{
			clientWidth = desktopWidth * maxClientFractionOfDesktop;
			clientHeight = desktopHeight * maxClientFractionOfDesktop;
			if (m_config.m_aspectRatio > desktopAspect)
			{
				// Client window has a wider aspect than desktop; shrink client height to match its width
				clientHeight = clientWidth / m_config.m_aspectRatio;
			}
			else
			{
				// Client window has a taller aspect than desktop; shrink client width to match its height
				clientWidth = clientHeight * m_config.m_aspectRatio;
			}
		}
	}
	else
	{
		clientWidth = desktopWidth;
		clientHeight = desktopHeight;
	}
	m_clientDimensions = IntVec2((int)clientWidth, (int)clientHeight);

	RECT clientRect = {};
	if (m_config.m_isFullscreen)
	{
		clientRect.left = 0;
		clientRect.right = (int)desktopWidth;
		clientRect.top = 0;
		clientRect.bottom = (int)desktopHeight;
	}
	else if (m_config.m_screenPos == IntVec2(-1, -1) || m_config.m_isFullscreen)
	{
		// Calculate client rect bounds by centering the client area
		float clientMarginX = 0.5f * (desktopWidth - clientWidth);
		float clientMarginY = 0.5f * (desktopHeight - clientHeight);

		clientRect.left = (int)clientMarginX;
		clientRect.right = clientRect.left + (int)clientWidth;
		clientRect.top = (int)clientMarginY;
		clientRect.bottom = clientRect.top + (int)clientHeight;
	}
	else
	{
		clientRect.left	   = m_config.m_screenPos.x;
		clientRect.right   = (int)clientWidth + clientRect.left;
		clientRect.top	   = m_config.m_screenPos.y;
		clientRect.bottom  = (int)clientHeight + clientRect.top;
	}

	const DWORD windowStyleFlags   = m_config.m_isFullscreen? WS_POPUP : WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = m_config.m_isFullscreen? WS_EX_TOPMOST : WS_EX_APPWINDOW;
	RECT windowRect = clientRect;
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	

	// Calculate the outer dimensions of the physical window, including frame et. al.
	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	m_windowHandle = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		NULL,
		NULL);

	ShowWindow(m_windowHandle, SW_SHOW);
	SetForegroundWindow(m_windowHandle);
	SetFocus(m_windowHandle);

	m_displayContext = GetDC(m_windowHandle);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	GetClientRect(m_windowHandle, &desktopRect);
	m_clientDimensions.x = desktopRect.right - desktopRect.left;
	m_clientDimensions.y = desktopRect.bottom - desktopRect.top;

}

void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}

IntVec2 Window::GetClientDimensions() const
{
	return m_clientDimensions;
}


Vec2 const Window::GetMouseScreenPos() const
{
	Vec2 normalizedCursorPos = GetNormalizedCursorPos();
	return Vec2(normalizedCursorPos.x * (float)m_clientDimensions.x, normalizedCursorPos.y * (float)m_clientDimensions.y);
}

Vec2 const Window::GetScreenSize() const
{
	return Vec2((float)m_clientDimensions.x,(float)m_clientDimensions.y);
}

AABB2 const Window::GetScreenBound() const
{
	return AABB2(Vec2::ZERO, GetScreenSize());
}

std::string const& Window::OpenFileDialogAndGetFilename() const
{
	static std::string selectedFilePath;
	char currentDirectory[MAX_PATH];
    DWORD result = GetCurrentDirectoryA(MAX_PATH, currentDirectory);

	if (result == 0)
	{
		selectedFilePath.clear();
		ERROR_RECOVERABLE("Fail to get the current directoy");
		return selectedFilePath;
	}
	char filename[MAX_PATH];
	filename[0] = '\0';

	OPENFILENAMEA data = {};
	data.lStructSize = sizeof(data);
	data.lpstrFile = filename;
	data.nMaxFile = sizeof(filename);
	data.lpstrFilter = "All\0*.*\0";
	data.nFilterIndex = 1;
	data.lpstrInitialDir = currentDirectory;
	data.hwndOwner = (HWND)GetHwnd();
	// Display the open file dialog
	if (GetOpenFileNameA(&data)) 
	{
		selectedFilePath = filename;
	}
	else 
	{
		selectedFilePath.clear();
	}
	SetCurrentDirectoryA(currentDirectory);
	return selectedFilePath;
}

void Window::ShowWindowLastError()
{
	DWORD error = ::GetLastError();
	if (error != 0)
	{
		LPSTR messageBuffer = nullptr;
		DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		FormatMessageA(flags, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		ERROR_RECOVERABLE(messageBuffer);
		LocalFree(messageBuffer);
	}
}

float Window::GetAspectRatio() const
{
	return m_config.m_aspectRatio;
}

Vec2 Window::GetNormalizedCursorPos() const
{
	HWND windowHandle = HWND(m_windowHandle);
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = float(cursorCoords.x) / float(clientRect.right);
	float cursorY = float(cursorCoords.y) / float(clientRect.bottom);
	return Vec2(cursorX, 1.f - cursorY);
}

void* Window::GetHwnd() const
{
	return static_cast<void*>(m_windowHandle);
	
}

HWND Window::GetWindowHandle() const
{
	return m_windowHandle;
}

