
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
		for (GuiWindow* window: m_windows)
		{
			window->ShowWindow();
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
	if (g_theInput->WasKeyJustPressed('P'))
	{
		ToggleGuiWindowFocus();
	}
	
	//ImGui::ShowDemoWindow(); // Show demo window! :)
}

void ImGuiEndFrame()
{
	// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
		ImGui::SetWindowSize(ImVec2(size.x, size.y));
	}
	Vec2 windowPos;
	windowPos.x = g_guiSystem->m_currentViewportSize.x * normalizedPos.x - alignment.x * windowSize.x;
	windowPos.y = g_guiSystem->m_currentViewportSize.y * normalizedPos.y - alignment.y * windowSize.y;

	ImGui::SetWindowPos(ImVec2(windowPos.x, windowPos.y));

}

void AddGuiWindow(GuiWindow* window)
{
	g_guiSystem->AddGuiWindow(window);
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

