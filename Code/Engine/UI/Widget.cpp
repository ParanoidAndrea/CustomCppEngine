#include "Engine/UI/Widget.hpp"
#include "Engine/Math/MathUtils.hpp"

Widget::Widget(unsigned int widgetID, std::vector<Button> buttons, bool isVisible)
	:m_widgetID(widgetID),
	m_isVisible(isVisible),
	m_buttons(buttons)
{

}

int Widget::IsButtonOnClick(Vec2 const& mousePosition) const
{
	for (int i = 0; i<(int)m_buttons.size(); i++)
	{
		if (IsPointInsideAABB2D(mousePosition, m_buttons[i].m_buttonArea))
		{
			return i;
		}
	}
	return -1;
}

void Widget::Render() const
{
	if (m_isVisible)
	{
		for (int i = 0; i < (int)m_buttons.size(); i++)
		{
			m_buttons[i].Render();
		}
	}
}
