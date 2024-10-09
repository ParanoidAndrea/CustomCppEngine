#pragma once
#include "KeyButtonState.hpp"
#include "Engine/Input/XboxController.hpp"
#include "Engine/Core/EventSystem.hpp"
extern unsigned char const KEYCODE_F1;
extern unsigned char const KEYCODE_F2;
extern unsigned char const KEYCODE_F3;
extern unsigned char const KEYCODE_F4;
extern unsigned char const KEYCODE_F5;
extern unsigned char const KEYCODE_F6;
extern unsigned char const KEYCODE_F7;
extern unsigned char const KEYCODE_F8;
extern unsigned char const KEYCODE_F9;
extern unsigned char const KEYCODE_F10;
extern unsigned char const KEYCODE_F11;
extern unsigned char const KEYCODE_ESC;
extern unsigned char const KEYCODE_UPARROW;
extern unsigned char const KEYCODE_DOWNARROW;
extern unsigned char const KEYCODE_LEFTARROW;
extern unsigned char const KEYCODE_RIGHTARROW;
extern unsigned char const KEYCODE_LEFT_MOUSE;
extern unsigned char const KEYCODE_RIGHT_MOUSE;
extern unsigned char const KEYCODE_TILDE;        // ~ key
extern unsigned char const KEYCODE_LEFTBRACKET;  // [ key
extern unsigned char const KEYCODE_RIGHTBRACKET; // ] key
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_BACKSPACE;
extern unsigned char const KEYCODE_INSERT;
extern unsigned char const KEYCODE_DELETE;
extern unsigned char const KEYCODE_HOME;
extern unsigned char const KEYCODE_END;
extern unsigned char const KEYCODE_SHIFT;
extern unsigned char const KEYCODE_COMMA;
extern unsigned char const KEYCODE_PERIOD;
constexpr int NUM_KEYCODES = 256;
constexpr int NUM_XBOX_CONTROLLERS = 4;
struct InputConfig
{

};

struct CursorState
{
	IntVec2 m_cursorClientDelta;
	IntVec2 m_cursorClientPosition;

	bool m_hiddenMode = false;
	bool m_relativeMode = false;
};

class InputSystem
{
public:
	InputSystem(InputConfig const& inputConfig);
	~InputSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasAnyKeyPressed();
	bool WasKeyJustReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	bool IsAnyKeyDown();
	void HandleKeyPressed(unsigned char keyCode);
	void HandleKeyReleased(unsigned char keyCode);
	void ResetInputState(unsigned char keyCode);
	void ResetCursorState();
	XboxController const& GetController(int controllerID);
	CursorState const& GetCursorState() const;
	void SetCursorMode(bool hiddenMode, bool relativeMode); // Hidden mode controls whether the cursor is visible or not. Relative mode will calculate a  cursor client delta and then reset the cursor to the client region center each frame. Both of these together can be used to implement an FPS-style mouse look camera.
	void SetCursorClientDeltaAndPosition(); // Returns the current frame cursor delta in pixels, relative to the client region. Only valid in relative mode, will be zero otherwise.
	Vec2 GetCursorClientDelta() const;
	Vec2 GetCursorNormalizedPosition() const; // Returns the cursor position, normalized to the range [0,1], relative to the client region, with the y-axis inverted to map from Windows conventions to game screen camera conventions
protected:
	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_KeyReleased(EventArgs& args);
    KeyButtonState m_keyStates[NUM_KEYCODES];
    XboxController m_controller[NUM_XBOX_CONTROLLERS];
	InputConfig m_inputConfig;
	CursorState m_cursorState;
	IntVec2 m_previousCursorPos;
};
