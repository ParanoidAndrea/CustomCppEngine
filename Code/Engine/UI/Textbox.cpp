#include "Engine/UI/Textbox.hpp"


Textbox::Textbox(Renderer* render, AABB2 const& textBoxArea, std::string const& label, BitmapFont* bitmapFont, Vec2 const& alignment , float fontSize, float fontAspect, Rgba8 const& textColor , bool isEnableShadow, Rgba8 const& shadowColor, Vec2 const& shadowOffset, TextBoxMode const& mode) :
    m_label(label)
    ,m_bitmapFont(bitmapFont)
    ,m_alignment(alignment)
    ,m_fontSize(fontSize)
    ,m_fontAspect(fontAspect)
    ,m_textColor(textColor)
    ,m_isEnableShadow(isEnableShadow)
    ,m_shadowColor(shadowColor)
    ,m_shadowOffset(shadowOffset)
    ,m_mode(mode)
    ,Widget(render)
{
    m_widgetArea = textBoxArea;
    m_renderColor = Rgba8(255, 255, 255, 0);
    m_isShowingBackground = false;
}
Textbox::Textbox(Renderer* render, Vec2 const& boxDimension,Vec2 const& relativePos, Vec2 const& pivot, std::string const& label, BitmapFont* bitmapFont, Vec2 const& alignment, float fontSize, float fontAspect, Rgba8 const& textColor, bool isEnableShadow, Rgba8 const& shadowColor, Vec2 const& shadowOffset, TextBoxMode const& mode) :
    m_label(label)
    , m_bitmapFont(bitmapFont)
    , m_alignment(alignment)
    , m_fontSize(fontSize)
    , m_fontAspect(fontAspect)
    , m_textColor(textColor)
    , m_isEnableShadow(isEnableShadow)
    , m_shadowColor(shadowColor)
    , m_shadowOffset(shadowOffset)
    , m_mode(mode)
    , Widget(render)
{

    m_widgetNormalizedDimension = boxDimension;
    m_widgetRelativePos = relativePos;
    m_widgetPivot = pivot;
    m_renderColor = Rgba8(255, 255, 255, 0);
    m_isShowingBackground = false;
}

void Textbox::Update()
{
}

void Textbox::Render() const
{
    if (m_isShowingBorder)
    {
        RenderBorder();
    }
    if (m_isShowingBackground)
    {
        RenderBackground();
    }
    std::vector<Vertex_PCU> textVerts;
    if (m_isEnableShadow)
    {
        m_bitmapFont->AddVertsForTextBox2D(textVerts, m_widgetArea, m_fontSize, m_label, m_shadowColor, m_fontAspect, m_alignment, m_mode, 100000, m_shadowOffset);
    }
    m_bitmapFont->AddVertsForTextBox2D(textVerts, m_widgetArea, m_fontSize, m_label, m_textColor, m_fontAspect, m_alignment, m_mode);
    m_renderer->SetModelConstants(m_transformMatrix, Rgba8::WHITE);
    m_renderer->BindTexture(&m_bitmapFont->GetTexture());
    m_renderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
}
