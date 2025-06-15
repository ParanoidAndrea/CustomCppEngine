#pragma once
#include "Engine/UI/Widget.hpp"
#include "Engine/Render/BitmapFont.hpp"
struct ScrollBoxLine
{
public:
    ScrollBoxLine(std::string const& text, Rgba8 color)
        :m_text(text)
        ,m_color(color)
    {

    }
    std::string m_text = "";
    Rgba8 m_color = Rgba8::WHITE;
};

class ScrollBox : public Widget
{

public:
    ScrollBox(Renderer* renderer, BitmapFont* bitmapFont, AABB2 const& widgetArea, float linesOnScreen = 10, float fontAspect = 1.f, Rgba8 backgroundColor = Rgba8(0, 0, 0, 200));
    ~ScrollBox();
    virtual void Update() override;
    virtual void Render() const override;
    void AddText(std::string const& text, Rgba8 const& color = Rgba8::WHITE);
    bool Event_MouseScroll(EventArgs& args);
    bool IsMouseInScrollBoxArea() const;
private:
    void UpdateScrollBarArea();
    void RenderBackground() const;
    void RenderText() const;
    void RenderScrollBar() const;
public:
    float m_mouseScrollSensitivity = 2.f;
    float m_linesOnScreen = 10.f;
    float m_fontAspect = 1.f;
    BitmapFont* m_bitmapFont = nullptr;
private:
    float m_scrollBackLineCount = 0.f;
    float m_fontHeight = 0.f;
    bool m_isShowingScrollBar = false;
    std::vector<ScrollBoxLine> m_lines;
    float m_maxHorizontalText = 100.f;
    AABB2 m_verticalScrollBar;
    AABB2 m_verticalScrollArea;
};