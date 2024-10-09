#pragma once

struct KeyButtonState 
{ 
	bool m_isPressed; 
	bool m_wasPressedLastFrame; 
	KeyButtonState() 
	{
		m_isPressed = false;
		m_wasPressedLastFrame = false;
	};
	~KeyButtonState() {};
};