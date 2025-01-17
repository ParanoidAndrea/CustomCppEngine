#pragma once
#include <string>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/UI/Widget.hpp"
class Renderer;
class BitmapFont;
class Timer;
class Slider : public Widget
{
public:
	Slider(AABB2 const& screenArea, std::string const& label = "", std::string const& bitmapFont = "", std::string const& eventName = "",Renderer* renderer = nullptr, AudioSystem* audioSystem = nullptr, float defaultValue = 0.f, float minValue = 0.f, float maxValue = 1.f);
	virtual void Update() override;
	virtual void Render() const override;
	bool IsCursorInSliderArea() const;
	float GetCurrentFraction() const;
	void OnChangeValue();
private:
	Timer* m_audioTimer = nullptr;
	AudioSystem* m_audioSystem = nullptr;
	std::string m_eventName = "";
	AABB2 m_screenArea = AABB2::ZERO_TO_ONE;
	AABB2 m_sliderArea = AABB2::ZERO_TO_ONE;
	BitmapFont* m_bitmapFont = nullptr;
	SoundID m_sliderHoverID = 0xffffffffffffffff;
	std::string m_label = "";
	float m_value = 0.f;
	float m_maxValue = 1.f;
	float m_minValue = 0.f;
	bool m_isChanging = false;
};