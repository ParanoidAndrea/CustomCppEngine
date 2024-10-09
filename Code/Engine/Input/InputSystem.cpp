#include "InputSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Render/DebugRender.hpp"
#define WIN32_LEARN_AND_MEAN
#include <windows.h>
unsigned char const KEYCODE_F1           = VK_F1;
unsigned char const KEYCODE_F2           = VK_F2;
unsigned char const KEYCODE_F3           = VK_F3;
unsigned char const KEYCODE_F4           = VK_F4;
unsigned char const KEYCODE_F5           = VK_F5;
unsigned char const KEYCODE_F6           = VK_F6;
unsigned char const KEYCODE_F7           = VK_F7;
unsigned char const KEYCODE_F8           = VK_F8;
unsigned char const KEYCODE_F9           = VK_F9;
unsigned char const KEYCODE_F10          = VK_F10;
unsigned char const KEYCODE_F11          = VK_F11;
unsigned char const KEYCODE_ESC          = VK_ESCAPE;
unsigned char const KEYCODE_UPARROW      = VK_UP;
unsigned char const KEYCODE_DOWNARROW    = VK_DOWN;
unsigned char const KEYCODE_LEFTARROW    = VK_LEFT;
unsigned char const KEYCODE_RIGHTARROW   = VK_RIGHT;
unsigned char const KEYCODE_LEFT_MOUSE   = VK_LBUTTON;
unsigned char const KEYCODE_RIGHT_MOUSE  = VK_RBUTTON;
unsigned char const KEYCODE_TILDE        = 0xC0;
unsigned char const KEYCODE_LEFTBRACKET  = 0xDB;
unsigned char const KEYCODE_RIGHTBRACKET = 0xDD;
unsigned char const KEYCODE_ENTER        = VK_RETURN;
unsigned char const KEYCODE_BACKSPACE    = VK_BACK;
unsigned char const KEYCODE_INSERT		 = VK_INSERT;
unsigned char const KEYCODE_DELETE		 = VK_DELETE;
unsigned char const KEYCODE_HOME		 = VK_HOME;
unsigned char const KEYCODE_END			 = VK_END;
unsigned char const KEYCODE_SHIFT        = VK_SHIFT;
unsigned char const KEYCODE_COMMA        = 0xBC;
unsigned char const KEYCODE_PERIOD		 = 0xBE;
InputSystem::InputSystem(InputConfig const& inputConfig)
	:m_inputConfig(inputConfig)
{
	for (int i = 0; i < NUM_KEYCODES; i++) 
	{ 
		m_keyStates[i].m_isPressed = false; 
		m_keyStates[i].m_wasPressedLastFrame = false;
	}
	
}

InputSystem::~InputSystem()
{
}
bool InputSystem::Event_KeyPressed(EventArgs& args)
{
	if (!g_theInput || g_theConsole->IsOpen())
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}

bool InputSystem::Event_KeyReleased(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}

void InputSystem::Startup()
{
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", InputSystem::Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyReleased", InputSystem::Event_KeyReleased);
}

void InputSystem::Shutdown()
{
}

void InputSystem::BeginFrame()
{
	for (int controllerID = 0; controllerID < NUM_XBOX_CONTROLLERS; controllerID++)
	{
		XboxController& controller = m_controller[controllerID];
		controller.m_id = controllerID;
		controller.Update();
	}

	CURSORINFO cursorInfo = {};
	::GetCursorInfo(&cursorInfo);
	bool isCursorShowing = cursorInfo.flags & CURSOR_SHOWING;

	if (m_cursorState.m_hiddenMode != isCursorShowing) {
		
		if (m_cursorState.m_hiddenMode)
		{
			while (::ShowCursor(false) >= 0)
			{
			}
		}
		else
		{
			while (::ShowCursor(true) < 0)
			{
			}
		}
	}
	else 
	{
		while (::ShowCursor(true) < 0)
		{
		}
	}
	SetCursorClientDeltaAndPosition();

}

void InputSystem::EndFrame()
{ 
	for (int i = 0; i < NUM_KEYCODES; i++)
	{
		m_keyStates[i].m_wasPressedLastFrame = m_keyStates[i].m_isPressed;
	}

}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return !m_keyStates[keyCode].m_wasPressedLastFrame && m_keyStates[keyCode].m_isPressed;
}



bool InputSystem::WasAnyKeyPressed()
{
	for (int i = 0; i < NUM_KEYCODES; i++)
	{
		if (!m_keyStates[i].m_wasPressedLastFrame&& m_keyStates[i].m_isPressed)
			return true;
	}
	return false;
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_wasPressedLastFrame && !m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_isPressed;
}

bool InputSystem::IsAnyKeyDown()
{
	for (int i = 0; i < NUM_KEYCODES; i++)
	{
		if (m_keyStates[i].m_isPressed)
			return true;
	}
	return false;
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = true; 

}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false; 
}

void InputSystem::ResetInputState(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false;
	m_keyStates[keyCode].m_wasPressedLastFrame = false;
}

void InputSystem::ResetCursorState()
{
	CURSORINFO cursorInfo = {};
	::GetCursorInfo(&cursorInfo);
	bool isCursorShowing = cursorInfo.flags & CURSOR_SHOWING;

	if (m_cursorState.m_hiddenMode != isCursorShowing) {

		if (m_cursorState.m_hiddenMode)
		{
			while (::ShowCursor(false) >= 0)
			{
			}
		}
		else
		{
			while (::ShowCursor(true) < 0)
			{
			}
		}
	}
	else
	{
		while (::ShowCursor(true) < 0)
		{
		}
	}
	SetCursorClientDeltaAndPosition();
}

XboxController const& InputSystem::GetController(int controllerID)
{
	return m_controller[controllerID];
}

CursorState const& InputSystem::GetCursorState() const
{
	return m_cursorState;
}

void InputSystem::SetCursorMode(bool hiddenMode, bool relativeMode)
{
	m_cursorState.m_hiddenMode = hiddenMode;
	m_cursorState.m_relativeMode = relativeMode;

}

void InputSystem::SetCursorClientDeltaAndPosition()
{
	HWND windowHandle = ::GetActiveWindow();
	POINT cursorCoords;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	IntVec2 currentCursorPos(cursorCoords.x, cursorCoords.y);
	m_cursorState.m_cursorClientPosition = currentCursorPos;
	if (!m_cursorState.m_relativeMode)
	{
		m_cursorState.m_cursorClientDelta = IntVec2();
	}
	else
	{
		m_cursorState.m_cursorClientDelta = currentCursorPos - m_previousCursorPos;
		RECT clientRect;
		::GetClientRect(windowHandle, &clientRect);
		cursorCoords.x = clientRect.right /2;
		cursorCoords.y = clientRect.bottom /2;
		::ClientToScreen(GetActiveWindow(), &cursorCoords);
		::SetCursorPos((int)cursorCoords.x, (int)cursorCoords.y);
		::ScreenToClient(windowHandle, &cursorCoords);
		
		m_cursorState.m_cursorClientPosition = IntVec2(cursorCoords.x, cursorCoords.y);
	}
	m_previousCursorPos = m_cursorState.m_cursorClientPosition;
}

Vec2 InputSystem::GetCursorClientDelta() const
{
	if (!m_cursorState.m_relativeMode || Window::GetMainWindowInstance()->GetHwnd()!=GetActiveWindow())
	{
		return Vec2();
	}
	else
	{
		return Vec2((float)m_cursorState.m_cursorClientDelta.x, (float)m_cursorState.m_cursorClientDelta.y);
	}
}

Vec2 InputSystem::GetCursorNormalizedPosition() const
{
	HWND windowHandle = ::GetActiveWindow();
// 	POINT cursorCoords;
 	RECT clientRect;
// 	::GetCursorPos(&cursorCoords);
// 	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = (float)m_cursorState.m_cursorClientPosition.x / float(clientRect.right);
	float cursorY = (float)m_cursorState.m_cursorClientPosition.y / float(clientRect.bottom);
// 	float cursorX = float(cursorCoords.x) / float(clientRect.right);
// 	float cursorY = float(cursorCoords.y) / float(clientRect.bottom);
	return Vec2(cursorX, 1.f - cursorY);
}


