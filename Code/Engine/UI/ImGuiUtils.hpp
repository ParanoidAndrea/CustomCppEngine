#pragma once
#include "Engine/Window/Window.hpp"
class Renderer;
class GuiWindow
{
public:
	GuiWindow();
	virtual void BeginWindow() {}
	virtual void ShowWindow();
	virtual void EndWindow() {}

};
void ImGuiStartup(Renderer* renderer, HWND windowHandler);
void ImGuiBeginFrame();
void ImGuiEndFrame();
void ImGuiShutdown();
void SetImGuiWindowPosAndSize(Vec2 const& normalizedPos, Vec2 const& alignment = Vec2(0.f, 0.f), Vec2 const& size = Vec2(-1.f,-1.f));
void AddGuiWindow(GuiWindow* window);
bool IsGuiWindowFocus();
void SetGuiWindowFocus(bool isFocus);
void ToggleGuiWindowFocus();
void SetToggleWindowKey(char keyCode);