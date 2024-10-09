#pragma comment( lib, "xinput9_1_0" ) // Xinput 1_4 doesn't work in older Windows versions; use XInput 9_1_0 explicitly for best compatibility
#include "XboxController.hpp"
#define WIN32_LEARN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Engine/Math/MathUtils.hpp"
XboxController::XboxController()
{
	m_rightStick.SetDeadZoneThresholds(0.3f, 0.95f);
	m_leftStick.SetDeadZoneThresholds(0.3f, 0.95f);
}

XboxController::~XboxController()
{
}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_leftStick;
}

AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_rightStick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

KeyButtonState const& XboxController::GetButton(XboxButtonID buttonID) const
{
	return m_buttons[buttonID];
}

bool XboxController::IsButtonDown(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].m_isPressed;
}

bool XboxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
	return !m_buttons[buttonID].m_wasPressedLastFrame && m_buttons[buttonID].m_isPressed;
}

bool XboxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].m_wasPressedLastFrame && !m_buttons[buttonID].m_isPressed;
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState = {};
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus == ERROR_SUCCESS)
	{
		m_isConnected = true;
		UpdateJoystick(m_leftStick, xboxControllerState.Gamepad.sThumbLX, xboxControllerState.Gamepad.sThumbLY);
		UpdateJoystick(m_rightStick, xboxControllerState.Gamepad.sThumbRX, xboxControllerState.Gamepad.sThumbRY);

		UpdateTrigger(m_leftTrigger, xboxControllerState.Gamepad.bLeftTrigger);
		UpdateTrigger(m_rightTrigger, xboxControllerState.Gamepad.bRightTrigger);

		unsigned short buttonFlags = xboxControllerState.Gamepad.wButtons;
		UpdateButton(START, buttonFlags, XINPUT_GAMEPAD_START);
		UpdateButton(BACK, buttonFlags, XINPUT_GAMEPAD_BACK);
		UpdateButton(A, buttonFlags, XINPUT_GAMEPAD_A);
		UpdateButton(B, buttonFlags, XINPUT_GAMEPAD_B);
		UpdateButton(X, buttonFlags, XINPUT_GAMEPAD_X);
		UpdateButton(Y, buttonFlags, XINPUT_GAMEPAD_Y);
		UpdateButton(SHOUDLER_RIGHT, buttonFlags, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		UpdateButton(SHOUDLER_LEFT, buttonFlags, XINPUT_GAMEPAD_LEFT_SHOULDER);
		UpdateButton(THUMB_RIGHT, buttonFlags, XINPUT_GAMEPAD_RIGHT_THUMB);
		UpdateButton(THUMB_LEFT, buttonFlags, XINPUT_GAMEPAD_LEFT_THUMB);
		UpdateButton(DPAD_RIGHT, buttonFlags, XINPUT_GAMEPAD_DPAD_RIGHT);
		UpdateButton(DPAD_LEFT,      buttonFlags, XINPUT_GAMEPAD_DPAD_LEFT);
		UpdateButton(DPAD_UP,        buttonFlags, XINPUT_GAMEPAD_DPAD_UP);
		UpdateButton(DPAD_DOWN,      buttonFlags, XINPUT_GAMEPAD_DPAD_DOWN);

	}
	else if (errorStatus == ERROR_DEVICE_NOT_CONNECTED)
	{
		m_isConnected = false;
		Reset();
	}
}

void XboxController::Reset()
{
	m_id = -1;
	m_isConnected = false;
	m_leftTrigger = 0.f;
	m_rightTrigger = 0.f;
	m_leftStick.Reset();
	m_rightStick.Reset();
	for (int i = 0; i < (int)XboxButtonID::NUM; i++)
	{
		m_buttons[i].m_isPressed = false;
		m_buttons[i].m_wasPressedLastFrame = false;
	}
}

void XboxController::UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
	float rawNormalizedX = RangeMapClamped(float(rawX), -32768.f, +32768.f, -1.f, 1.f);
	float rawNormalizedY = RangeMapClamped(float(rawY), -32768.f, +32768.f, -1.f, 1.f);
	out_joystick.UpdatePosition(rawNormalizedX, rawNormalizedY);
}

void XboxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
{
	float normalizedValue = (float)rawValue;
	out_triggerValue = normalizedValue;

}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	KeyButtonState& button = m_buttons[buttonID];
	button.m_wasPressedLastFrame = button.m_isPressed;
	button.m_isPressed = ((buttonFlags & buttonFlag) == buttonFlag);
}
