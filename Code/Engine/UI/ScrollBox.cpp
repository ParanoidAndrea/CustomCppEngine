#include "Engine/UI/ScrollBox.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"

ScrollBox::ScrollBox(Renderer* renderer, BitmapFont* bitmapFont, AABB2 const& widgetArea, float linesOnScreen /*= 10*/, float fontAspect /*= 1.f*/, Rgba8 backgroundColor /*= Rgba8(0, 0, 0, 200)*/) :Widget(renderer, widgetArea)
, m_bitmapFont(bitmapFont)
, m_linesOnScreen(linesOnScreen)
, m_fontAspect(fontAspect)
{
    m_borderColor = backgroundColor;
    Vec2 dimension = widgetArea.GetDimensions();
    m_fontHeight = dimension.y / (float)linesOnScreen;
    m_maxHorizontalText = widgetArea.GetDimensions().x / (m_fontHeight * fontAspect);
    g_theEventSystem->SubscribeEventCallbackObjectMethod("MouseScroll",this, &ScrollBox::Event_MouseScroll);
}

ScrollBox::~ScrollBox()
{
    g_theEventSystem->UnsubscribeAllEventCallbackObjectMethods(this);
}

void ScrollBox::Update()
{
    static float scrollSensitivity = 1.f;
    if (g_theInput->WasKeyJustPressed(KEYCODE_UPARROW))
    {
        m_scrollBackLineCount += scrollSensitivity;
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_DOWNARROW))
    {
        m_scrollBackLineCount -= scrollSensitivity;
    }
    m_scrollBackLineCount = GetClamped(m_scrollBackLineCount, 0.f, (float)m_lines.size() - m_linesOnScreen+ 1.f);

    UpdateScrollBarArea();
}
void ScrollBox::AddText(std::string const& text, Rgba8 const& color /*= Rgba8::WHITE*/)
{
    if (text.size() > (int)m_maxHorizontalText)
    {
        Strings subStrings = SplitStringOnDelimiter(text, ' ', false);
        std::string pushBackLine;
        int startIndex = 0;
        while (pushBackLine.size() < (int)m_maxHorizontalText -subStrings[startIndex].size()  && startIndex < subStrings.size())
        {
            pushBackLine += subStrings[startIndex];
            pushBackLine += " ";
            startIndex++;
        }
        m_lines.push_back(ScrollBoxLine(pushBackLine, color));
        std::string nextPushBackLine;
        for (int i = startIndex; i < (int)subStrings.size(); ++i)
        {
            nextPushBackLine += subStrings[i];
            nextPushBackLine += " ";
        }
        AddText(nextPushBackLine, color);
        //m_lines.push_back(text.substr(0, (int)m_maxHorizontalText));
        //AddText(text.substr((int)m_maxHorizontalText, text.size() - (int)m_maxHorizontalText), color);
    }
    else
    {
        m_lines.push_back(ScrollBoxLine(text, color));
    }
}

bool ScrollBox::Event_MouseScroll(EventArgs& args)
{
    if ( IsMouseInScrollBoxArea() && m_lines.size() >= (int)m_linesOnScreen + 1)
    {
        int scrollDirection = args.GetValue("ScrollDirection", 1);
        float scrollSensitivity = m_mouseScrollSensitivity;
        if (scrollDirection > 0)
        {
            m_scrollBackLineCount += scrollSensitivity;
        }
        else
        {
            m_scrollBackLineCount -= scrollSensitivity;
        }
        m_scrollBackLineCount = GetClamped(m_scrollBackLineCount, 0.f, (float)m_lines.size() - m_linesOnScreen + 1.f);
        return true;
    }
    return false;
}

bool ScrollBox::IsMouseInScrollBoxArea() const
{
    return IsPointInsideAABB2D(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos(), m_widgetArea);
}

void ScrollBox::UpdateScrollBarArea()
{
    float scrollBarLength = (float)m_linesOnScreen / m_lines.size();
    if (scrollBarLength < 1.f)
    {
        Vec2 screenSize = m_widgetArea.GetDimensions();
        m_isShowingScrollBar = true;
        float scrollBarWidth = screenSize.x * 0.02f;
        float scrollBarBottom = (float)m_scrollBackLineCount / m_lines.size();
        float scrollBarTop = scrollBarBottom + scrollBarLength;

        m_verticalScrollBar = AABB2(m_widgetArea.m_maxs.x - scrollBarWidth, m_widgetArea.m_mins.y + scrollBarBottom * screenSize.y, m_widgetArea.m_maxs.x, m_widgetArea.m_mins.y +scrollBarTop * screenSize.y);
    }
    else
    {
        m_isShowingScrollBar = false;
    }
}

void ScrollBox::Render() const
{
    m_renderer->SetBlendMode(BlendMode::ALPHA);
    m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
    m_renderer->SetModelConstants(m_transformMatrix);
    if (m_isShowingBackground)
    {
        RenderBackground();
    }
    RenderText();
    if (m_isShowingScrollBar)
    {
        RenderScrollBar();
    }
}



void ScrollBox::RenderBackground() const
{
    std::vector<Vertex_PCU> verts;
    AddVertsForAABB2D(verts, m_widgetArea, m_borderColor);
    m_renderer->BindTexture(m_backgroundTexture);
    m_renderer->BindShader(nullptr);
    m_renderer->DrawVertexArray((int)verts.size(), verts.data());
}

void ScrollBox::RenderText() const
{
    static std::vector<Vertex_PCU> textVerts;
    textVerts.clear();
    AABB2 textBounds = m_widgetArea;
    float lineHeight = (textBounds.m_maxs.y - textBounds.m_mins.y) / m_linesOnScreen;
    textBounds.m_mins.y = m_widgetArea.m_mins.y;
    textBounds.m_maxs.y = textBounds.m_mins.y + lineHeight;

    int startIndex = (int)m_lines.size() - 1 - (int)m_scrollBackLineCount;
    for (int i = startIndex; i >= 0; i--)
    {
        if ((int)m_lines.size() - i - (int)m_scrollBackLineCount >= m_linesOnScreen)
        {
            m_renderer->BindTexture(&m_bitmapFont->GetTexture());
            m_renderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
            return;
        }
        textBounds.m_maxs.y = textBounds.m_mins.y + lineHeight;
        m_bitmapFont->AddVertsForTextBox2D(textVerts, textBounds, lineHeight, m_lines[i].m_text, Rgba8(0,0,0,100), m_fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN, 100, Vec2(0.8f, 0.8f));
        m_bitmapFont->AddVertsForTextBox2D(textVerts, textBounds, lineHeight,m_lines[i].m_text, m_lines[i].m_color, m_fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN);
        textBounds.m_mins.y += lineHeight;
    }
    m_renderer->BindTexture(&m_bitmapFont->GetTexture());
    m_renderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
}

void ScrollBox::RenderScrollBar() const
{

    std::vector<Vertex_PCU> verts;
    AddVertsForAABB2D(verts, m_verticalScrollBar, Rgba8::GREY);
    m_renderer->BindTexture(nullptr);
    m_renderer->DrawVertexArray((int)verts.size(), verts.data());

}
