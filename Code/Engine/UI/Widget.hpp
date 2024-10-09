#pragma once
#include "Engine/UI/Button.hpp"
#include <vector>
struct Widget
{
public:
	Widget(unsigned int widgetID, std::vector<Button> buttons, bool isVisible = true);
	int IsButtonOnClick(Vec2 const& mousePosition) const;
	void Render() const;
public:
	std::vector<Button> m_buttons;
	bool m_isVisible = true;
	unsigned int m_widgetID = 0;
};