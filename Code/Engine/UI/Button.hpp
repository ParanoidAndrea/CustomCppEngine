#pragma once
#include <string>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"

class Texture;
class Renderer;
class BitmapFont;
class Button
{
public:
	Button(AABB2 const& buttonArea,Renderer* render = nullptr, std::string buttonTextureName="", Rgba8 buttonColor = Rgba8::WHITE, std::string label = "", std::string eventName = "", std::string fontName = "SquirrelFixedFont",AudioSystem* audioSystem = nullptr, Rgba8 hoverColor = Rgba8(255,255,255,125),Rgba8 pressedColor = Rgba8::GREY);
	Button(AABB2 const& buttonArea,  EventArgs const& args, std::string const& label = "", std::string const& eventName = "", std::string const& fontName = "ClassicComic", Renderer* render = nullptr, AudioSystem* audioSystem = nullptr, Texture* buttonTexture = nullptr);
	void Update();
	void Render() const;
	void Reset();
	bool InButtonArea(Vec2 const& mousePos) const;
	void OnClick();
	void OnHover();
	void OnUnhover();
public:
	AABB2 m_buttonArea;
	std::string m_eventName = "";
private:
	EventArgs m_args;
	Rgba8 m_buttondDefaultColor = Rgba8::WHITE;
	Rgba8 m_buttonColor = Rgba8::WHITE;
	Rgba8 m_buttonHoverColor = Rgba8(255, 255, 255, 125);
	Rgba8 m_buttonPressedColor = Rgba8::GREY;
	Renderer* m_renderer = nullptr;
	std::string m_label = "";
	Texture* m_buttonTexture = nullptr;
	BitmapFont* m_bitmapFont = nullptr;
	AudioSystem* m_audioSystem = nullptr;
	SoundID m_buttonClickID = 0xffffffffffffffff;
	SoundID m_buttonHoverID = 0xffffffffffffffff;
	bool m_isHoverSoundPlayed = false;
	bool m_isHover = false;
};
