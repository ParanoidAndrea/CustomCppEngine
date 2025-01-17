#include "Engine/UI/ImGuiUtils.hpp"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_impl_dx11.h"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include <vector>


class GuiSystem
{
public:
	GuiSystem() 
	{ 	
 
	}
	void AddGuiWindow(GuiWindow* newWindow = nullptr);
	void BeginGuiWindows();
	void ShowGuiWindows();
	void EndGuiWindows();
	std::vector<GuiWindow*> m_windows;
	Vec2 m_currentViewportSize;
	bool m_isFocus = false;
};

void GuiSystem::AddGuiWindow(GuiWindow* newWindow /*= nullptr*/)
{
	if (newWindow)
	{
		m_windows.push_back(newWindow);
	}
	else
	{
		m_windows.push_back(new GuiWindow());
	}
}

void GuiSystem::BeginGuiWindows()
{
	for (GuiWindow* window : m_windows)
	{
		window->BeginWindow();
	}
}

void GuiSystem::ShowGuiWindows()
{
	if (m_windows.size() == 0)
	{
		//ImGui::ShowDemoWindow();
	}
	else
	{
		for (size_t i = 0; i < m_windows.size(); ++i)
		{
			m_windows[i]->ShowWindow();
		}
	}
}

void GuiSystem::EndGuiWindows()
{
	for (GuiWindow* window : m_windows)
	{
		window->EndWindow();
	}
}

GuiWindow::GuiWindow()
{

}

void GuiWindow::ShowWindow()
{
	ImGui::ShowDemoWindow();
}

GuiSystem* g_guiSystem = nullptr;


void ImGuiStartup(Renderer* renderer, HWND windowHandler)
{
	// Setup Dear ImGui context
	g_guiSystem = new GuiSystem();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(windowHandler);
	ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetDeviceContext());
}

void ImGuiBeginFrame()
{
	// (Your code process and dispatch Win32 messages)
    // Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	g_guiSystem -> m_currentViewportSize = Vec2(ImGui::GetMainViewport()->WorkSize.x, ImGui::GetMainViewport()->WorkSize.y); 
	g_guiSystem->ShowGuiWindows();
	
	//ImGui::ShowDemoWindow(); // Show demo window! :)
}

void ImGuiEndFrame()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (g_theInput->WasKeyJustPressed(' '))
	{
		ToggleGuiWindowFocus();
	}
	// (Your code calls swapchain's Present() function)
}

void ImGuiShutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void SetImGuiWindowPosAndSize(Vec2 const& normalizedPos, Vec2 const& alignment, Vec2 const& size)
{
	Vec2 windowSize;
	if (size == Vec2(-1.f, -1.f))
	{
		windowSize = Vec2(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
	}
	else
	{
		windowSize = size;
		ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_FirstUseEver);
	}
	Vec2 windowPos;
	windowPos.x = g_guiSystem->m_currentViewportSize.x * normalizedPos.x - alignment.x * windowSize.x;
	windowPos.y = g_guiSystem->m_currentViewportSize.y * normalizedPos.y - alignment.y * windowSize.y;

	ImGui::SetWindowPos(ImVec2(windowPos.x, windowPos.y), ImGuiCond_FirstUseEver);

}

void SetImGuiNextWindowPosAndSize(Vec2 const& normalizedPos, Vec2 const& alignment, Vec2 const& size)
{
	Vec2 windowSize;
	if (size == Vec2(-1.f, -1.f))
	{
		windowSize = Vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	}
	else
	{
		windowSize = size;
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_FirstUseEver);
	}
	Vec2 windowPos;
	windowPos.x = g_guiSystem->m_currentViewportSize.x * normalizedPos.x - alignment.x * windowSize.x;
	windowPos.y = g_guiSystem->m_currentViewportSize.y * normalizedPos.y - alignment.y * windowSize.y;

	ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y), ImGuiCond_FirstUseEver);
}

Vec2 const GetImGuiWindowNormalizePosFromSize(Vec2 const& alignment, Vec2 const& size)
{
	Vec2 windowPos = Vec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
	Vec2 windowSize = (size == Vec2(-1.f, -1.f)) ?Vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y) : size;
	return Vec2((alignment.x * windowSize.x + windowPos.x) / g_guiSystem->m_currentViewportSize.x,
		(alignment.y * windowSize.y + windowPos.y) / g_guiSystem->m_currentViewportSize.y);
}

void AddGuiWindow(GuiWindow* window)
{
	g_guiSystem->AddGuiWindow(window);
}

void AddGuiPopup(std::string const& popupTitle, std::string const& popupText, Vec2 size)
{
	g_guiSystem->m_windows.push_back(new GuiPopup(popupTitle, popupText, size)) ;
}

void AddGuiPopup(std::string const& popupTitle, std::string const& popupText, std::function<void()> yesCallback, std::function<void()> noCallback, Vec2 size)
{
	GuiPopup* popup = new GuiPopup(popupTitle, popupText, size, POPUPTYPE_CALLBACK);
	popup->SetYesCallback(yesCallback);
	popup->SetNoCallback(noCallback);
}

bool IsGuiWindowFocus()
{
	return g_guiSystem->m_isFocus;
}

void SetGuiWindowFocus(bool isFocus)
{
	g_guiSystem->m_isFocus = isFocus;
}

void ToggleGuiWindowFocus()
{
	g_guiSystem->m_isFocus = !g_guiSystem->m_isFocus;
}

void SetToggleWindowKey(char keyCode)
{
	if (ImGui::IsWindowCollapsed())
	{
		if (g_theInput->WasKeyJustPressed(keyCode))
		{
			ImGui::SetWindowCollapsed(false);
			SetGuiWindowFocus(true);
		}
	}
	else
	{
		if (g_theInput->WasKeyJustPressed(keyCode))
		{
			ImGui::SetWindowCollapsed(true);
			SetGuiWindowFocus(false);
		}
	}
}

void GuiPopup::ShowWindow()
{
	ImGui::OpenPopup(m_popupTitle.c_str());
	SetImGuiNextWindowPosAndSize(Vec2(0.5f,0.5f),Vec2(0.5f,0.5f),m_size);
	// Create the popup window
	if (ImGui::BeginPopupModal(m_popupTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Draw the popup contents
		ImGui::SetWindowFontScale(1.25f);
		ImGui::Spacing();
		ImGui::TextWrapped(m_popupText.c_str());
		ImGui::Spacing();
		ImGui::SetWindowFontScale(1.f);
		// Add a button to close the popup
		if (m_type == POPUPTYPE_NORMAL)
		{
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				g_guiSystem->m_windows.erase(std::remove(g_guiSystem->m_windows.begin(), g_guiSystem->m_windows.end(), this));
				delete this;
			}
		}
		else
		{
			bool isOpen = false;
			if (ImGui::Button("Yes"))
			{
				if (m_yesCallback) m_yesCallback();
				ImGui::CloseCurrentPopup();
				isOpen = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				if (m_noCallback) m_noCallback();
				ImGui::CloseCurrentPopup();
				isOpen = true;
			}
			if (isOpen)
			{
				delete this;
			}
		}

		// End the popup window definition
		ImGui::EndPopup();
	}
}

