#pragma once
#include <string>
#include <vector>
#include "Rgba8.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/XmlUtils.hpp"
class Renderer;
class Camera;
class BitmapFont;
class Timer;
struct AABB2;
class DevConsole;
class Window;
struct DevConsoleLine
{
	DevConsoleLine(Rgba8 color, std::string text);
	Rgba8 m_color;
	std::string m_text = {};

};
struct DevConsoleConfig
{

	Renderer* m_defalutRenderer = nullptr;
	Camera* m_camera = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
	float m_fontAspect = 0.7f;
	float m_lineHeight = 20.f;
	float m_linesOnScreen = 30.f;
	int m_frameNumberPrinted = 0;
	double m_timePrinted = 0.f;
	float m_mouseScrollSensitivity = 2.f;
};
enum DevConsoleMode
{
	DEVCONSOLE_HIDDEN,
	DEVCONSOLE_SHOWN
};
class DevConsole
{
public:
	DevConsole(DevConsoleConfig const& config);
	~DevConsole();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void Execute(std::string const& consoleCommandText, bool echoCommand = true); // Parses the current input line and executes it using the event system. Commands and arguments and delimited from each other with ' ' and argument names and values are delimited with '=' 
	void ExcuteCommand(std::string const& commandLine);
	void ExecuteXmlCommandScriptNode( XmlElement const& commandScriptXmlElement );
	void ExecuteXmlCommandScriptFile( std::string const& commandScriptXmlFilePathName );
	void AddLine(Rgba8 const& color, std::string const& text);
	void AddLineWithLineAndFrameNumber(Rgba8 const& color, int lineNumber);
	void Render (AABB2 bounds = AABB2(Vec2(),Vec2(1600.f,800.f)) , Renderer* rendererOverride = nullptr) const;
	Camera const* GetCamera() const;
	void SetConfig(DevConsoleConfig const& config);
	void ToggleOpen();
	bool IsOpen();

	static const Rgba8 INFO_ERROR;
	static const Rgba8 INFO_WARNING;
	static const Rgba8 INFO_MAJOR;
	static const Rgba8 INFO_MINOR;
	static const Rgba8 INFO_CONSOLE;
	static const Rgba8 INPUT_TEXT;
	static const Rgba8 INPUT_INSERTION_POINT;
	static const Rgba8 INFO_ECHO;

	static bool Event_KeyPressed(EventArgs& args); // Handle key input.
	static bool Event_KeyReleased(EventArgs& args); // Handle key input.
	static bool Event_CharInput(EventArgs& args); // Handle char input by appending valid characters to our current input line.
	static bool Event_MouseScroll(EventArgs& args); // Handle the mouse scroll input.
	static bool Command_Clear(EventArgs& args); // Clear all lines of text.
	static bool Command_Help(EventArgs& args); // Display all currently registered commands in the event system.

protected:
	void SetScrollBarDimension();
	void TranslateScrollBar(Vec2 const& translation);
	bool InScrollBarArea(Vec2 const& pos);
	void Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font) const;
	void RenderScrollBar(Renderer & renderer) const;

protected:
	Vec2 m_previousMousePos;
	DevConsoleConfig m_config;
	DevConsoleMode m_mode;
	std::vector<DevConsoleLine> m_lines;
	std::string m_inputText = ""; // Current line of input text
	int m_frameNumber = 0;
	bool m_isOpen = false; // True if the dev console is currently visible and accepting input.
	bool m_isShowingScrollBar = false;
	int m_insertionPointPosition = 0; // Index of the insertion point in our current input text.
	bool m_insertionPointVisible = true; // BLINKING
	Timer* m_insertionPointBlinkTimer = nullptr; // Timer for controlling insertion point visibility
	AABB2 m_scrollBar;
	std::vector<std::string> m_commandHistory;
	int m_historyIndex = -1; // Current index in our history of commands as we are scrolling.
	float m_scrollBackLineCount = 0.f;
	Window* m_window = nullptr;
	bool m_isMousePressing = false;
};