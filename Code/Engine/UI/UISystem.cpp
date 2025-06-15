#include "UISystem.hpp"
#include "Engine/UI/Widget.hpp"
void UISystem::Startup()
{
}

void UISystem::BeginFrame()
{
    m_currentGarbageWidgets.insert(m_currentGarbageWidgets.end(), m_pendingGarbageWidgets.begin(), m_pendingGarbageWidgets.end());
    m_pendingGarbageWidgets.clear();
}

void UISystem::EndFrame()
{
    // Process only the widgets that were marked for deletion before this frame
    for (size_t i = 0; i < m_currentGarbageWidgets.size(); ++i)
    {
        Widget* widget = m_currentGarbageWidgets[i];
        if (widget && widget != reinterpret_cast<Widget*>(0xFFFFFFFFFFFFFFFF))
        {
            delete widget;
        }
        m_currentGarbageWidgets[i] = nullptr;
    }
    m_currentGarbageWidgets.clear();
}

void UISystem::ShutDown()
{
    for (Widget* widget : m_currentGarbageWidgets)
    {
        if (widget)
        {
            delete widget;
            widget = nullptr;
        }
    }
    m_currentGarbageWidgets.clear();

    for (Widget* widget : m_pendingGarbageWidgets)
    {
        if (widget)
        {
            delete widget;
            widget = nullptr;
        }
    }
    m_pendingGarbageWidgets.clear();
}

void UISystem::DeleteWidget(Widget* widget)
{
    if (!widget || widget == reinterpret_cast<Widget*>(0xFFFFFFFFFFFFFFFF))
    {
        printf("Warning: Attempted to delete invalid widget pointer: %p\n", widget);
        return;
    }

    for (Widget* garbageWidget : m_currentGarbageWidgets)
    {
        if (garbageWidget == widget)
        {
            return;
        }
    }

    for (Widget* garbageWidget : m_pendingGarbageWidgets)
    {
        if (garbageWidget == widget)
        {
            return;
        }
    }

    m_pendingGarbageWidgets.push_back(widget);
}
