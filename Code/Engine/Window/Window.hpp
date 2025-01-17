#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include <string.h>
#define WIN32_LEARN_AND_MEAN
#include <windows.h>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"

class InputSystem;

struct WindowConfig
{
	InputSystem*  m_inputSystem = nullptr;
	std::string  m_windowTitle = "Untitled App";
	float m_aspectRatio = 2.f;
	bool m_isFullscreen = false;
	IntVec2 m_screenSize = IntVec2(-1, -1);
	IntVec2 m_screenPos = IntVec2(-1, -1);
};

class Window
{
public:
	Window(WindowConfig const& config);
	~Window();

	void StartUp();
	void BeginFrame();
	void EndFrame();
	void ShutDown();

	WindowConfig const& GetConfig() const;
	static Window* GetMainWindowInstance();
	HDC const& GetDisplayContext();
	Vec2 GetNormalizedCursorPos() const;
	void* GetHwnd() const;
	HWND GetWindowHandle() const;
	IntVec2 GetClientDimensions() const;
	Vec2 const GetMouseScreenPos() const;
	Vec2 const GetScreenSize() const;
	AABB2 const GetScreenBound() const;
	std::string const& OpenFileDialogAndGetFilename() const;
	void ShowWindowLastError();
	float GetAspectRatio() const;
protected:
	void CreateOSWindow();
	void RunMessagePump();

private:
	HDC m_displayContext = nullptr;
	HWND m_windowHandle = nullptr;
	IntVec2 m_clientDimensions;
	WindowConfig m_config;
	static Window* s_theWindow;
};

