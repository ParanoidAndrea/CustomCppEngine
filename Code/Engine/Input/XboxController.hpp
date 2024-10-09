#pragma once
#include "AnalogJoystick.hpp"
#include "KeyButtonState.hpp"
enum XboxButtonID
{
	START,
	BACK,
	A,
	B,
	X,
	Y,
	SHOUDLER_RIGHT,
	SHOUDLER_LEFT,
	THUMB_RIGHT,
	THUMB_LEFT,
	DPAD_RIGHT,
	DPAD_LEFT,
	DPAD_UP,
	DPAD_DOWN,
	NUM
};

class XboxController
{
	friend class InputSystem;
public:
	XboxController();
	~XboxController();
	bool IsConnected() const;
	int GetControllerID() const;
	AnalogJoystick const& GetLeftStick() const;
	AnalogJoystick const& GetRightStick() const;
	float GetLeftTrigger() const;
	float GetRightTrigger() const;
	KeyButtonState const& GetButton(XboxButtonID buttonID) const;
	bool IsButtonDown(XboxButtonID buttonID) const;
	bool WasButtonJustPressed(XboxButtonID buttonID) const;
	bool WasButtonJustReleased (XboxButtonID buttonID) const;

private:
	void Update();
	void Reset();
	void UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY);
	void UpdateTrigger(float& out_triggerValue, unsigned char rawValue);
	void UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag);

private:
	int m_id = -1;
	bool m_isConnected = false;
	float m_leftTrigger = 0.f;
	float m_rightTrigger = 0.f;
	AnalogJoystick m_leftStick;
	AnalogJoystick m_rightStick;
	KeyButtonState m_buttons[(int)XboxButtonID::NUM];

};


