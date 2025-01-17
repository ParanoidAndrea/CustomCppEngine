#include "Engine/UI/Button.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/UI/Textbox.hpp"

Button::Button(AABB2 const& buttonArea, Renderer* render, std::string buttonTextureName, Rgba8 buttonColor, std::string label, std::string eventName, std::string fontName, AudioSystem* audioSystem,Rgba8 hoverColor, Rgba8 pressedColor)
	:m_eventName(eventName),
	m_buttondDefaultColor(buttonColor),
	m_buttonColor(buttonColor),
	m_buttonHoverColor(hoverColor),
	m_buttonPressedColor(pressedColor),
	m_audioSystem(audioSystem),
	Widget(render)
{
	m_widgetArea = buttonArea;
	if (m_audioSystem)
	{
		m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
		m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
	}
	if (buttonTextureName.compare("") != 0)
	{
		m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(buttonTextureName.c_str());
	}
    BitmapFont* font = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
    m_labelText = new Textbox(m_renderer, m_widgetArea, label, font);
	m_children.push_back(m_labelText);
	m_isShowingBackground = true;
}

Button::Button(AABB2 const& buttonArea, EventArgs const& args, std::string const& label, std::string const& eventName, std::string const& fontName, Renderer* render, AudioSystem* audioSystem, Texture* buttonTexture)
	:m_eventName(eventName),
	m_audioSystem(audioSystem),
	m_args(args),
	Widget(render)
{
	m_backgroundTexture = buttonTexture;
	m_widgetArea = buttonArea;
	if (m_audioSystem)
	{
		m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
		m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
	}
	BitmapFont* font = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
	m_labelText = new Textbox(m_renderer,m_widgetArea,label, font);
	m_children.push_back(m_labelText);
	m_isShowingBackground = true;
}

Button::Button(Vec2 const& buttonDimension, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, Renderer* render, std::string buttonTextureName, Rgba8 buttonColor , std::string label, std::string eventName, std::string fontName, AudioSystem* audioSystem, Rgba8 hoverColor, Rgba8 pressedColor)
    :m_eventName(eventName),
    m_buttondDefaultColor(buttonColor),
    m_buttonColor(buttonColor),
    m_buttonHoverColor(hoverColor),
    m_buttonPressedColor(pressedColor),
    m_audioSystem(audioSystem),
	Widget(render)
{
	m_widgetNormalizedDimension = buttonDimension;
	m_widgetRelativePos = widgetRelativePos;
	m_widgetPivot = widgetPivot;
    if (m_audioSystem)
    {
        m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
        m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
    }
    if (buttonTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(buttonTextureName.c_str());
    }

    BitmapFont* font = m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + fontName).c_str());
	m_labelText = new Textbox(m_renderer,buttonDimension,Vec2(0.5f, 0.5f), Vec2(0.5f, 0.5f), label, font);
	m_children.push_back(m_labelText);
	m_isShowingBackground = true;
}

Button::Button(Vec2 const& buttonDimension, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, Textbox* testBox, Renderer* render, std::string buttonTextureName, Rgba8 buttonColor, std::string eventName, AudioSystem* audioSystem, Rgba8 hoverColor, Rgba8 pressedColor)
	:m_eventName(eventName),
	m_buttondDefaultColor(buttonColor),
	m_buttonColor(buttonColor),
	m_buttonHoverColor(hoverColor),
	m_buttonPressedColor(pressedColor),
	m_audioSystem(audioSystem),
	Widget(render)
{
	m_widgetNormalizedDimension = buttonDimension;
    //m_widgetArea = AABB2(Vec2(0.f, 0.f), buttonDimension);
   
    m_widgetRelativePos = widgetRelativePos;
    m_widgetPivot = widgetPivot;
    if (m_audioSystem)
    {
        m_buttonClickID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonClick2.wav", false);
        m_buttonHoverID = m_audioSystem->CreateOrGetSound("Data/Audio/UI/ButtonHover2.wav", false);
    }
    if (buttonTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(buttonTextureName.c_str());
    }
	m_labelText = testBox;
	m_children.push_back(m_labelText);
	m_isShowingBackground = true;
}

void Button::Update()
{
	if (m_parent == nullptr || (m_parent && !m_parent->m_isNavigableCanvas))
	{
		if (InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
		{
			OnHover();
		}
		else if (!InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
		{
			OnUnhover();
		}
	}
	else if (m_parent && m_parent->m_isNavigableCanvas)
	{
		if (InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
		{
			m_isCanvasMouseControl = true;
			for (size_t i = 0; i < m_parent->m_navigableChildren.size(); ++i)
			{
				if (m_parent->m_navigableChildren[i] == this)
				{
					m_parent->m_selectedWidgetIndex = (unsigned int)i;
					OnHover();
				}
				else
				{
					m_parent->m_navigableChildren[i]->OnUnhover();
				}

			}
		}
		else if (!InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
		{
			m_isCanvasMouseControl = false;
		}
	}

	if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE)
		&& InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
	{
		m_buttonColor = m_buttonPressedColor;
		OnClick();
	}
	else if (m_isHover)
	{
		//m_buttonColor = m_buttonHoverColor;
	}
	else
	{
		//m_buttonColor = m_buttondDefaultColor;
	}

 	if (g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE)
 		&& InButtonArea(m_renderer->GetRenderConfig().m_window->GetMouseScreenPos()))
 	{
 		m_buttonColor = Rgba8::GREY;
 		OnClick();
 
 	}
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->Update();
	}
}

void Button::Render() const
{
    if (m_isShowingBorder)
    {
        if (m_isHiddenBorderOnHover)
        {
            if (m_isHover)
            {
                RenderBorder();
            }
        }
        else
        {
            RenderBorder();
        }
    }
	if (m_isShowingBackground)
	{
		std::vector<Vertex_PCU> buttonBoxVerts;
		AddVertsForAABB2D(buttonBoxVerts, m_widgetArea, Rgba8::WHITE);
		m_renderer->SetBlendMode(BlendMode::ALPHA);
		m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
		m_renderer->SetDepthMode(DepthMode::DISABLED);
		m_renderer->BindShader(nullptr);
		m_renderer->SetModelConstants(Mat44(), m_buttonColor);
		m_renderer->BindTexture(m_backgroundTexture);
		m_renderer->DrawVertexArray((int)buttonBoxVerts.size(), buttonBoxVerts.data());
	}

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->Render();
	}

}

void Button::Reset()
{
	m_buttonColor = m_buttondDefaultColor;
	OnUnhover();
}

bool Button::InButtonArea(Vec2 const& mousePos) const
{
	return IsPointInsideAABB2D(mousePos, m_widgetArea);
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
	m_buttonColor = m_buttonHoverColor;
	m_labelText->m_textColor = m_textHoverColor;
	m_isHover = true;
}

void Button::OnUnhover()
{
	m_labelText->m_textColor = m_textColor;
	m_buttonColor = m_buttondDefaultColor;
	m_isHoverSoundPlayed = false;
	m_isHover = false;
}

void Button::SetTextNormalColorAndHoverColor(Rgba8 const& textNormalColor, Rgba8 const& textHoverColor)
{
	m_textColor = textNormalColor;
	m_textHoverColor = textHoverColor;
}
