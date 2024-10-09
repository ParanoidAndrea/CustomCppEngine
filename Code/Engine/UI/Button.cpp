#include "Engine/UI/Button.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Window/Window.hpp"
Button::Button(AABB2 const& buttonArea, Renderer* render, std::string buttonTextureName, Rgba8 buttonColor, std::string label, std::string eventName, std::string fontName, AudioSystem* audioSystem,Rgba8 hoverColor, Rgba8 pressedColor)
	:m_buttonArea(buttonArea),
	m_label(label),
	m_eventName(eventName),
	m_renderer(render),
	m_buttondDefaultColor(buttonColor),
	m_buttonColor(buttonColor),
	m_buttonHoverColor(hoverColor),
	m_buttonPressedColor(pressedColor),
	m_audioSystem(audioSystem)
{
	
	if (m_audioSystem)
	{
		m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
		m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
	}
	if (buttonTextureName.compare("") != 0)
	{
		m_buttonTexture = m_renderer->CreateOrGetTextureFromFile(buttonTextureName.c_str());
	}
	m_bitmapFont = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
}

Button::Button(AABB2 const& buttonArea, EventArgs const& args, std::string const& label, std::string const& eventName, std::string const& fontName, Renderer* render, AudioSystem* audioSystem, Texture* buttonTexture)
	:m_buttonArea(buttonArea),
	m_label(label),
	m_eventName(eventName),
	m_renderer(render),
	m_audioSystem(audioSystem),
	m_args(args),
	m_buttonTexture(buttonTexture)
{
	if (m_audioSystem)
	{
		m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
		m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
	}
	m_bitmapFont = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
}

void Button::Update()
{
	if (m_audioSystem && InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
	{
		OnHover();
	}
	else if (m_audioSystem && !InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
	{
		OnUnhover();
	}

	if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE)
		&& InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
	{
		m_buttonColor = m_buttonPressedColor;
		OnClick();
	}
	else if (m_isHover)
	{
		m_buttonColor = m_buttonHoverColor;
	}
	else
	{
		m_buttonColor = m_buttondDefaultColor;
	}

 	if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE)
 		&& InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
 	{
 		m_buttonColor = Rgba8::GREY;
 		OnClick();
 
 	}

}

void Button::Render() const
{
	std::vector<Vertex_PCU> buttonBoxVerts;
	AddVertsForAABB2D(buttonBoxVerts,m_buttonArea,m_buttonColor);
	m_renderer->SetBlendMode(BlendMode::ALPHA);
	m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_renderer->SetDepthMode(DepthMode::DISABLED);
	m_renderer->BindShader(nullptr);
	m_renderer->SetModelConstants();
	m_renderer->BindTexture(m_buttonTexture);
	m_renderer->DrawVertexArray((int)buttonBoxVerts.size(), buttonBoxVerts.data());

	std::vector<Vertex_PCU> textVerts;
	m_bitmapFont->AddVertsForTextBox2D(textVerts, m_buttonArea, 25.f, m_label, Rgba8(0, 0, 0, 200), 1.f, Vec2(0.5f, 0.5f), TextBoxMode::SHRINK_TO_FIT, 100000, Vec2(2.5f, 2.5f));
	m_bitmapFont->AddVertsForTextBox2D(textVerts, m_buttonArea, 25.f, m_label, Rgba8(255, 255, 255, 255), 1.f, Vec2(0.5f, 0.5f), TextBoxMode::SHRINK_TO_FIT);
	m_renderer->BindTexture(&m_bitmapFont->GetTexture());
	m_renderer->DrawVertexArray((int)textVerts.size(), textVerts.data());
}

void Button::Reset()
{
	m_buttonColor = m_buttondDefaultColor;
	OnUnhover();
}

bool Button::InButtonArea(Vec2 const& mousePos) const
{
	return IsPointInsideAABB2D(mousePos, m_buttonArea);
}

void Button::OnClick()
{
   #if !defined( ENGINE_DISABLE_AUDIO )
	if (m_audioSystem)
	{
		m_audioSystem->StartSound(m_buttonClickID, false, m_audioSystem->GetUIVolume(), 0.f, 1.f, false, SoundClass::UI);
	}
    #endif // !defined( ENGINE_DISABLE_AUDIO )
	g_theEventSystem->FireEvent(m_eventName,m_args);
	g_theInput->ResetInputState(KEYCODE_LEFT_MOUSE);
}

void Button::OnHover()
{
	if (!m_isHoverSoundPlayed)
	{
        #if !defined( ENGINE_DISABLE_AUDIO )
		if (m_audioSystem)
		{
			m_audioSystem->StartSound(m_buttonHoverID, false, m_audioSystem->GetUIVolume(), 0.f, 1.f, false, SoundClass::UI);
		}
        #endif // !defined( ENGINE_DISABLE_AUDIO )
		m_isHoverSoundPlayed = true;
	}

	m_isHover = true;
}

void Button::OnUnhover()
{
	m_isHoverSoundPlayed = false;
	m_isHover = false;
}
