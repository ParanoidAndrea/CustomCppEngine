#include "Engine/UI/Slider.hpp"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/TimeR.hpp"
Slider::Slider(AABB2 const& screenArea, std::string const& label, std::string const& fontName, std::string const& eventName, Renderer* renderer, AudioSystem* audioSystem,float defaultValue, float minValue, float maxValue)
	:m_screenArea(screenArea),
	m_label(label),
	m_eventName(eventName),
	m_renderer(renderer),
	m_audioSystem(audioSystem),
	m_value(defaultValue),
	m_minValue(minValue),
	m_maxValue(maxValue)
{
	if (m_audioSystem)
	{
		m_sliderHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
	}
	m_sliderArea = m_screenArea.GetAABB2ForNormalizePoints(Vec2(0.25f, 0.4f), Vec2(0.8f, 0.6f));
	m_bitmapFont = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
	if (m_audioSystem)
	{
		m_audioTimer = new Timer(0.1f);
	}
}

void Slider::Update()
{
	if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE) && IsCursorInSliderArea())
	{
		OnChangeValue();
	}
	if (m_audioSystem && m_audioTimer->HasPeriodElapsed())
	{
		m_audioTimer->Stop();	
		m_audioSystem->StartSound(m_sliderHoverID, false, m_audioSystem->GetUIVolume(), 0.f, 1.f, false, SoundClass::UI);
	}
}
float Slider::GetCurrentFraction() const
{
	return m_value / (m_maxValue - m_minValue);
}

void Slider::OnChangeValue()
{
	float mouseScreenX = m_renderer->GetRenderConfig().m_window->GetMouseScreenPos().x;
	float previousValue = m_value;
	m_value = (mouseScreenX - m_sliderArea.m_mins.x) / (m_sliderArea.m_maxs.x - m_sliderArea.m_mins.x);
	m_value = GetClamped(m_value, m_minValue, m_maxValue);
	if (m_audioSystem && m_value != previousValue && m_audioTimer->IsStopped())
	{
		m_audioTimer->Start();
	}

	EventArgs eventArgs;
	eventArgs.SetValue("Value", Stringf("%.1f", m_value));
	g_theEventSystem->FireEvent(m_eventName, eventArgs);
}

void Slider::Render() const
{
	std::vector<Vertex_PCU> sliderBoxVerts;

	AABB2 sliderValueArea = AABB2(m_sliderArea.m_mins.x + 
		m_sliderArea.GetDimensions().x * GetCurrentFraction()-5.f,
		m_screenArea.m_mins.y, m_sliderArea.m_mins.x + m_sliderArea.GetDimensions().x * GetCurrentFraction()+5.f, m_screenArea.m_maxs.y);
	AddVertsForAABB2D(sliderBoxVerts, m_sliderArea, Rgba8::WHITE);

	m_renderer->SetBlendMode(BlendMode::ALPHA);
	m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_renderer->SetDepthMode(DepthMode::DISABLED);
	m_renderer->BindShader(nullptr);
	m_renderer->SetModelConstants();
	m_renderer->BindTexture(nullptr);
	m_renderer->DrawVertexArray((int)sliderBoxVerts.size(), sliderBoxVerts.data());
	std::vector<Vertex_PCU> sliderValueBoxVerts;
	m_renderer->SetRasterizerMode(RasterizerMode::WIREFRAME_CULL_NONE);
	AddVertsForAABB2D(sliderValueBoxVerts, sliderValueArea, Rgba8::WHITE);
	m_renderer->DrawVertexArray((int)sliderValueBoxVerts.size(), sliderValueBoxVerts.data());

	std::vector<Vertex_PCU> textVerts;
	m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	//m_bitmapFont->AddVertsForTextBox2D(textVerts, m_screenArea.GetAABB2ForNormalizePoints(Vec2(0.f, 0.f), Vec2(0.2f, 1.f)), 25.f, m_label, Rgba8(0, 0, 0, 200), 1.f, Vec2(0.5f, 0.5f), TextBoxMode::SHRINK_TO_FIT, 1000, Vec2(2.5f, 2.5f));
	m_bitmapFont->AddVertsForTextBox2D(textVerts, m_screenArea.GetAABB2ForNormalizePoints(Vec2(0.f, 0.f), Vec2(0.2f, 1.f)), 25.f, m_label, Rgba8(255, 255, 255, 255), 1.f, Vec2(1.f, 0.5f), TextBoxMode::OVERRUN);
	//m_bitmapFont->AddVertsForTextBox2D(textVerts, m_screenArea.GetAABB2ForNormalizePoints(Vec2(0.f, 0.f), Vec2(0.2f, 1.f)), 25.f, m_label, Rgba8(0, 0, 0, 200), 1.f, Vec2(0.5f, 0.5f), TextBoxMode::SHRINK_TO_FIT, 1000, Vec2(2.5f, 2.5f));
	m_bitmapFont->AddVertsForTextBox2D(textVerts, m_screenArea.GetAABB2ForNormalizePoints(Vec2(0.8f, 0.f), Vec2(1.f, 1.f)), 25.f, Stringf("%.1f",m_value), Rgba8(255, 255, 255, 255), 1.f, Vec2(1.f,0.5f), TextBoxMode::OVERRUN);
	m_renderer->BindTexture(&m_bitmapFont->GetTexture());
	m_renderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
}

bool Slider::IsCursorInSliderArea() const
{
	if (IsPointInsideAABB2D(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos(), 
		AABB2(m_sliderArea.m_mins.x,m_screenArea.m_mins.y, m_sliderArea.m_maxs.x, m_screenArea.m_maxs.y)))
	{
		return true;
	}
	return false;
}

