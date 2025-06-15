#pragma once
#include <string>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/UI/Widget.hpp"
class Texture;
class Renderer;
class BitmapFont;
class Textbox;
class Button : public Widget
{
public:
	Button(AABB2 const& buttonArea,Renderer* render = nullptr, std::string buttonTextureName="", Rgba8 buttonColor = Rgba8::WHITE, std::string label = "", std::string eventName = "", std::string fontName = "SquirrelFixedFont",AudioSystem* audioSystem = nullptr, Rgba8 hoverColor = Rgba8(255,255,255,125),Rgba8 pressedColor = Rgba8::GREY);
    Button(Vec2 const& buttonDimension, Vec2 const& widgetRelativePos,Vec2 const& widgetPivot, Renderer* render = nullptr, std::string buttonTextureName = "", Rgba8 buttonColor = Rgba8::WHITE, std::string label = "", std::string eventName = "", std::string fontName = "SquirrelFixedFont", AudioSystem* audioSystem = nullptr, Rgba8 hoverColor = Rgba8(255, 255, 255, 125), Rgba8 pressedColor = Rgba8::GREY);
	Button(Vec2 const& buttonDimension, Vec2 const& widgetRelativePos,Vec2 const& widgetPivot, Textbox* testBox, Renderer* render = nullptr, std::string buttonTextureName = "", Rgba8 buttonColor = Rgba8::WHITE, std::string eventName = "", AudioSystem* audioSystem = nullptr, Rgba8 hoverColor = Rgba8(255, 255, 255, 125), Rgba8 pressedColor = Rgba8::GREY);
	Button(AABB2 const& buttonArea,  EventArgs const& args, std::string const& label = "", std::string const& eventName = "", std::string const& fontName = "ClassicComic", Renderer* render = nullptr, AudioSystem* audioSystem = nullptr, Texture* buttonTexture = nullptr, Rgba8 const& buttonTextNormalColor = Rgba8::WHITE, Rgba8 const& buttonTextHoverColor = Rgba8::WHITE );
	virtual void Update() override;
	virtual void Render() const override;
	void Reset();
	bool InButtonArea(Vec2 const& mousePos) const;
	virtual void OnClick() override;
	virtual void OnHover() override;
	virtual void OnUnhover() override;
	void DisableTextShadow();
	void SetTextAspect(float textAspect);
	void SetTextNormalColorAndHoverColor(Rgba8 const& textNormalColor,Rgba8 const& textHoverColor);
    template<typename T>
    void SetValue(std::string const& keyName, T const& value);
	void SetArgs(EventArgs const& args);
public:
	std::string m_eventName = "";
	bool m_isHiddenBorderOnHover = true;
	//unsigned int m_navigateIndex = 0xffffffff;

private:
	EventArgs m_args;
	Rgba8 m_buttondDefaultColor = Rgba8::WHITE;
	Rgba8 m_buttonColor			= Rgba8::WHITE;
	Rgba8 m_buttonHoverColor    = Rgba8(255, 255, 255, 125);
	Rgba8 m_buttonPressedColor  = Rgba8::GREY;
    Rgba8 m_textColor			= Rgba8::WHITE;
    Rgba8 m_textHoverColor		= Rgba8::WHITE;
	AudioSystem* m_audioSystem = nullptr;
	SoundID m_buttonClickID = 0xffffffffffffffff;
	SoundID m_buttonHoverID = 0xffffffffffffffff;
	bool m_isHoverSoundPlayed = false;
	bool m_isHover = false;
	Textbox* m_labelText = nullptr;
};

template<typename T>
void Button::SetValue(std::string const& keyName, T const& value)
{
	m_args.SetValue(keyName, value);
}
