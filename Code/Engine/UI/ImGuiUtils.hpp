#pragma once
#include "Engine/Window/Window.hpp"
#include <functional>
class Renderer;
class GuiWindow
{
public:
	GuiWindow();
	virtual void BeginWindow() {}
	virtual void ShowWindow();
	virtual void EndWindow() {}
public:
	Vec2 m_normalizedPos = Vec2();
	Vec2 m_size = Vec2(-1.f,-1.f);
};
enum PopupType
{
	POPUPTYPE_NORMAL,
	POPUPTYPE_CALLBACK,
};
class GuiPopup : public GuiWindow
{
public:
	 using PopupCallback = std::function<void()>;
	GuiPopup(std::string const& popupTitle, std::string const& popupText, Vec2 size, PopupType type =POPUPTYPE_NORMAL, float duration = -1.f)
		:m_popupText(popupText), m_popupTitle(popupTitle), m_duration(duration),m_type(type)
	{
		m_size = size;
	}
	virtual void BeginWindow() override
	{

	}
	virtual void ShowWindow() override;


	virtual void EndWindow() override
	{
	}
	void SetYesCallback(PopupCallback callback) { m_yesCallback = callback; }
	void SetNoCallback(PopupCallback callback) { m_noCallback = callback; }
private:
	PopupType m_type = POPUPTYPE_NORMAL;
	PopupCallback m_yesCallback;
	PopupCallback m_noCallback;
	std::string m_popupTitle = {};
	std::string m_popupText = {};
	float m_duration = -1.f;
};
Vec2 const GetImGuiWindowNormalizePosFromSize(Vec2 const& alignment = Vec2(0.f, 0.f), Vec2 const& size = Vec2(-1.f, -1.f));
void ImGuiStartup(Renderer* renderer, HWND windowHandler);
void ImGuiBeginFrame();
void ImGuiEndFrame();
void ImGuiShutdown();
void SetImGuiWindowPosAndSize(Vec2 const& normalizedPos, Vec2 const& alignment = Vec2(0.f, 0.f), Vec2 const& size = Vec2(-1.f,-1.f));
void SetImGuiNextWindowPosAndSize(Vec2 const& normalizedPos, Vec2 const& alignment = Vec2(0.f, 0.f), Vec2 const& size = Vec2(-1.f,-1.f));
void AddGuiWindow(GuiWindow* window);
void AddGuiPopup(std::string const& popupTitle, std::string const& popupText, Vec2 size = Vec2(200.f,100.f));
void AddGuiPopup(std::string const& popupTitle, std::string const& popupText,std::function<void()> yesCallback, std::function<void()> noCallback, Vec2 size = Vec2(200.f,100.f));
bool IsGuiWindowFocus();
void SetGuiWindowFocus(bool isFocus);
void ToggleGuiWindowFocus();
void SetToggleWindowKey(char keyCode);