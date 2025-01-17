#pragma once
#include "Engine/UI/Widget.hpp"
#include "Engine/Render/BitmapFont.hpp"
class Textbox : public Widget
{
public:
    Textbox(Renderer* render,BitmapFont* bitmapFont = nullptr) 
        :m_bitmapFont(bitmapFont), Widget(render)
    {

    }
    Textbox(
        Renderer* render, AABB2 const& textBoxArea,
        std::string const& label = "",
        BitmapFont* bitmapFont = nullptr,
        Vec2 const& alignment = Vec2(0.5f, 0.5f),
        float fontSize = 25.f,
        float fontAspect = 1.f,
        Rgba8 const& textColor = Rgba8::WHITE,
        bool isEnableShadow = true,
        Rgba8 const& shadowColor = Rgba8(0, 0, 0, 200),
        Vec2 const& shadowOffset = Vec2(2.5f, 2.5f),
        TextBoxMode const& mode = SHRINK_TO_FIT
    );
    Textbox(
        Renderer* render, Vec2 const& boxDimension,Vec2 const& relativePos, Vec2 const& pivot,
        std::string const& label = "",
        BitmapFont* bitmapFont = nullptr,
        Vec2 const& alignment = Vec2(0.5f, 0.5f),
        float fontSize = 25.f,
        float fontAspect = 1.f,
        Rgba8 const& textColor = Rgba8::WHITE,
        bool isEnableShadow = true,
        Rgba8 const& shadowColor = Rgba8(0, 0, 0, 200),
        Vec2 const& shadowOffset = Vec2(2.5f, 2.5f),
        TextBoxMode const& mode = SHRINK_TO_FIT
    );
    virtual void Update() override;
    virtual void Render() const override;
    void SetTextLabel(std::string const& label)
    {
        m_label = label;
    }
    void SetTextColor(Rgba8 const& textColor)
    {
        m_textColor = textColor;
    }
    void SetTextAlign(Vec2 const& alignment)
    {
        m_alignment = alignment;
    }
    void SetTextFont(float fontSize, float fontAspect)
    {
        m_fontSize = fontSize;
        m_fontAspect = fontAspect;
    }
    void SetTextShadow(Rgba8 const& shadowColor, Vec2 const& shadowOffset)
    {
        m_shadowColor  = shadowColor  ;
        m_shadowOffset = shadowOffset ;
    }
    void DisableShadow()
    {
        m_isEnableShadow = false;
    }
    void EnableShadow()
    {
        m_isEnableShadow = true;
    }

public:
    std::string m_label          = "";
    BitmapFont* m_bitmapFont     = nullptr;
    Vec2        m_alignment      = Vec2(0.5f,0.5f);
    float       m_fontSize       = 25.f;
    float       m_fontAspect     = 1.f;
    Rgba8       m_textColor      = Rgba8::WHITE;
    bool        m_isEnableShadow = true;
    Rgba8       m_shadowColor    = Rgba8(0, 0, 0, 200);
    Vec2        m_shadowOffset   = Vec2(2.5f, 2.5f);
    TextBoxMode m_mode           = SHRINK_TO_FIT;
};