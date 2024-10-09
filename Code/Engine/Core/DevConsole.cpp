#include "DevConsole.hpp"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
Rgba8 const DevConsole::INFO_ERROR   = Rgba8::RED;
Rgba8 const DevConsole::INFO_WARNING = Rgba8::YELLOW;
Rgba8 const DevConsole::INFO_MAJOR   = Rgba8::GREEN;
Rgba8 const DevConsole::INFO_MINOR   = Rgba8(0,0,255,125);
Rgba8 const DevConsole::INPUT_TEXT   = Rgba8::WHITE;
Rgba8 const DevConsole::INFO_CONSOLE = Rgba8::BLUE;
Rgba8 const DevConsole::INPUT_INSERTION_POINT = Rgba8::WHITE;

DevConsoleLine::DevConsoleLine(Rgba8 color, std::string text)
	:m_color(color),
	m_text(text)
{

}

DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config),
	m_mode(DevConsoleMode::DEVCONSOLE_HIDDEN)
{
}

DevConsole::~DevConsole()
{
}

void DevConsole::Startup()
{
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", DevConsole::Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("CharInput",  DevConsole::Event_CharInput);
	g_theEventSystem->SubscribeEventCallbackFunction("Help",       DevConsole::Command_Help);
	g_theEventSystem->SubscribeEventCallbackFunction("Clear",      DevConsole::Command_Clear);
	m_insertionPointBlinkTimer = new Timer(0.5f);
}


void DevConsole::Shutdown()
{
	//m_insertionPointBlinkTimer->Stop();
}

void DevConsole::BeginFrame()
{
	if (m_insertionPointBlinkTimer->IsStopped())
	{
		m_insertionPointBlinkTimer->Start();
	}
	m_frameNumber++;

	if (m_insertionPointBlinkTimer->HasPeriodElapsed())
	{
		m_insertionPointVisible = !m_insertionPointVisible;
		m_insertionPointBlinkTimer->Stop();

	}

	// 	if (m_insertionPointBlinkTimer->HasPeriodElapsed())
// 	{
// 		m_insertionPointBlinkTimer->Stop();
// 	}

// 		std::string timerString = Stringf("SystemTotalTime: %f, insertionTimer:%f  ", m_insertionPointBlinkTimer->m_clock->GetTotalSeconds(),m_insertionPointBlinkTimer->m_startTime);
// 		if ((int)m_lines.size() >= 1)
// 		{
// 			m_lines[0].m_text = timerString;
// 			m_lines[0].m_color = INFO_MINOR;
// 		}
// 		else
// 		{
// 			AddLine(INFO_MINOR, timerString);
// 		}
}

void DevConsole::EndFrame()
{

}

void DevConsole::Execute(std::string const& consoleCommandText, bool echoCommand)
{
	if (echoCommand)
	{
		AddLine(Rgba8(255,0,255,255), consoleCommandText);
	}
	m_commandHistory.push_back(consoleCommandText);
	m_historyIndex= (int)m_inputText.size()-1;
 	std::string eventName;
 	EventArgs args;
	std::vector<std::string> registeredCommands = g_theEventSystem->GetAllRegisteredCommands();
 	Strings commandLine = SplitStringOnDelimiter(consoleCommandText, ' ');
	if ((int)commandLine.size() == 1)
	{
		for (int i = 0; i < (int)registeredCommands.size(); ++i)
		{
			if (IsEqualWithoutProperCase(registeredCommands[i], commandLine[0]))
			{
				//AddLine(INFO_MAJOR, registeredCommands[i]);
				FireEvent(registeredCommands[i], args);
				return;
			}
		}
	}
	else if ((int)commandLine.size() >= 2)
	{
		for (int i = 1; i < (int)commandLine.size(); ++i)
		{
			Strings keyAndValue = SplitStringOnDelimiter(commandLine[i], '=');
			if ((int)keyAndValue.size() == 2)
			{
				args.SetValue(keyAndValue[0], keyAndValue[1]);
			}
			else
			{
				AddLine(INFO_ERROR, "Execute failed. The argument is not the right format");
				return;
			}
		}
		
		for (int i = 0; i < (int)registeredCommands.size(); ++i)
		{
			if (IsEqualWithoutProperCase(registeredCommands[i], commandLine[0]))
			{
				//AddLine(INFO_MAJOR, registeredCommands[i]);
				FireEvent(registeredCommands[i], args);
				return;
			}
		}
	}

	AddLine(INFO_ERROR, "Execute failed. The command does not exit.");
	

}

void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	Strings texts = SplitStringOnDelimiter(text, '\n');
	for (int i = 0; i < (int)texts.size(); ++i)
	{
		m_lines.push_back(DevConsoleLine::DevConsoleLine(color, texts[i]));
	}

}

void DevConsole::AddLineWithLineAndFrameNumber(Rgba8 const& color, int lineNumber)
{
	double currentTime = GetCurrentTimeSeconds();
	UNUSED(lineNumber);
	int number = (int)m_lines.size()+1;
	std::string const& text = Stringf("This is Line#%d, Frame #%d and Time:%.3f",number, m_frameNumber, currentTime);
	m_lines.push_back(DevConsoleLine::DevConsoleLine(color, text));
}

void DevConsole::Render(AABB2 bounds, Renderer* rendererOverride) const
{
	Renderer* renderer = nullptr;
	if (m_config.m_camera)
	{
		bounds = m_config.m_camera->GetCameraBounds();
	}
	if (m_config.m_defalutRenderer)
	{
		renderer = m_config.m_defalutRenderer;
	}
	else
	{
		renderer = rendererOverride;
	}

 	if (m_isOpen)
 	{
		BitmapFont* bitmapFont = renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/")+m_config.m_fontName).c_str());
		Render_OpenFull(bounds, *renderer, *bitmapFont);
	}
}

Camera const* DevConsole::GetCamera() const
{
	return m_config.m_camera;
}

void DevConsole::ToggleOpen()
{
	m_isOpen = !m_isOpen;
}

bool DevConsole::IsOpen()
{
	return m_isOpen;
}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	if (keyCode == KEYCODE_TILDE)
	{
		g_theConsole->ToggleOpen();
		return true;
	}
	else if (g_theConsole->m_isOpen)
	{
		//g_theConsole->m_insertionPointBlinkTimer->Start();
		g_theConsole->m_insertionPointVisible = true;
		g_theConsole->m_insertionPointBlinkTimer->Stop();
		if (keyCode == KEYCODE_ESC)
		{
			g_theConsole->m_insertionPointPosition = 0;
			if (g_theConsole->m_inputText == "")
			{
				g_theConsole->m_isOpen = false;
			}
			else
			{
				g_theConsole->m_inputText = "";
			}
		}
		if (keyCode == KEYCODE_ENTER)
		{
			g_theConsole->m_insertionPointPosition = 0;
			if (g_theConsole->m_inputText == "")
			{
				g_theConsole->m_isOpen = false;
			}
			else
			{
				g_theConsole->Execute(g_theConsole->m_inputText);
				g_theConsole->m_inputText = "";
			}
			
		}
		if (keyCode == KEYCODE_UPARROW)
		{
			if (g_theConsole->m_inputText != "")
			{
				g_theConsole->m_historyIndex--;
			}
			if (g_theConsole->m_historyIndex < 0 )
			{
				g_theConsole->m_historyIndex = 0;
			}
			if (g_theConsole->m_historyIndex >= (int)g_theConsole->m_commandHistory.size())
			{
				g_theConsole->m_historyIndex = (int)g_theConsole->m_commandHistory.size()-1;
			}
				g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_historyIndex];
		}
		if (keyCode == KEYCODE_DOWNARROW)
		{
			g_theConsole->m_historyIndex++;
			if (g_theConsole->m_historyIndex < 0 )
			{
				g_theConsole->m_historyIndex = 0;
			}
			if (g_theConsole->m_historyIndex >= (int)g_theConsole->m_commandHistory.size())
			{
				g_theConsole->m_historyIndex = (int)g_theConsole->m_commandHistory.size()-1;
			}

				g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_historyIndex];
		}

		if (keyCode == KEYCODE_RIGHTARROW)
		{
			if (g_theConsole->m_insertionPointPosition < (int)g_theConsole->m_inputText.length())
			{
				g_theConsole->m_insertionPointPosition++;
			}
		}

		if (keyCode == KEYCODE_LEFTARROW)
		{
			if (g_theConsole->m_insertionPointPosition > 0)
			{
				g_theConsole->m_insertionPointPosition--;
			}
		}
		if (keyCode == KEYCODE_HOME)
		{
			g_theConsole->m_insertionPointPosition = 0;
		}
		if (keyCode == KEYCODE_END)
		{
			g_theConsole->m_insertionPointPosition = (int)g_theConsole->m_inputText.length();
		}
		if (keyCode == KEYCODE_BACKSPACE)
		{
			if (g_theConsole->m_inputText != "" && g_theConsole->m_insertionPointPosition > 0)
			{
				g_theConsole->m_insertionPointPosition--;
				g_theConsole->m_inputText.erase((size_t)g_theConsole->m_insertionPointPosition, 1);
			}
		}
		if (keyCode == KEYCODE_DELETE)
		{
			if (g_theConsole->m_inputText != "")
			{
				g_theConsole->m_inputText.erase((size_t)g_theConsole->m_insertionPointPosition, 1);
				//g_theConsole->m_insertionPointPosition--;	
			}
		}
		return true;
	}
	
	return false;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	unsigned char keyCode = (unsigned char)args.GetValue("CharCode", -1);
	std::string inputChar(1, keyCode);

	if (inputChar=="·" ||inputChar=="~"|| inputChar == "`" || keyCode<32 || keyCode >126)
	{
		return false;
	}
	if (g_theConsole->IsOpen())
	{
		g_theConsole->m_insertionPointVisible = true;
		g_theConsole->m_insertionPointBlinkTimer->Stop();
		g_theConsole->m_inputText = g_theConsole->m_inputText.insert(g_theConsole->m_insertionPointPosition,1,keyCode);
		g_theConsole->m_insertionPointPosition++;
		
		return true;
	}
	
 	return false;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);
	g_theConsole->m_lines.clear();
	return true;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED(args);
	std::vector<std::string> commands = g_theEventSystem->GetAllRegisteredCommands();
	for (int i = 0; i < (int)commands.size(); ++i)
	{

		g_theConsole->AddLine(INFO_CONSOLE,commands[i]);
	}
	return true;
}

void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font) const
{
	renderer.SetBlendMode(BlendMode::ALPHA);
	std::vector<Vertex_PCU> blurScreenVerts;
	AddVertsForAABB2D(blurScreenVerts, bounds, Rgba8(0, 0, 0, 125));
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray((int)blurScreenVerts.size(), blurScreenVerts.data());

	float fontAspect = m_config.m_fontAspect;



	std::vector<Vertex_PCU> textVerts;
	AABB2 textBounds = bounds;
	int numVisibleLines = (int)m_config.m_linesOnScreen;
	//float lineHeight = (bounds.m_maxs.y - bounds.m_mins.y) / m_config.m_linesOnScreen;
	float lineHeight = m_config.m_lineHeight;
	textBounds.m_mins.y = bounds.m_mins.y;
	textBounds.m_maxs.y = textBounds.m_mins.y + lineHeight;

	if (m_insertionPointVisible)
	{
		std::vector<Vertex_PCU> insertionPointVerts;
		AddVertsForLineSegment2D(insertionPointVerts, textBounds.m_mins + Vec2(m_insertionPointPosition * lineHeight * fontAspect + 2.f, 0.f), textBounds.m_mins + Vec2(m_insertionPointPosition * lineHeight * fontAspect + 2.f, lineHeight), 2.f, Rgba8::WHITE);
		renderer.BindTexture(nullptr);
		renderer.DrawVertexArray((int)insertionPointVerts.size(), insertionPointVerts.data());
	}

	font.AddVertsForTextBox2D(textVerts, textBounds, lineHeight, m_inputText, Rgba8::BLACK, fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN,100,Vec2(0.8f,0.8f));
	font.AddVertsForTextBox2D(textVerts, textBounds, lineHeight, m_inputText, INPUT_TEXT, fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN);
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray((int)textVerts.size(), textVerts.data());

	textBounds.m_mins.y += lineHeight;
	textBounds.m_maxs.y = textBounds.m_mins.y + lineHeight;
	int startIndex = (int)m_lines.size() - 1;
	for (int i = startIndex; i >= 0; i--)
	{
		if ((int)m_lines.size() - i >= numVisibleLines)
		{

			return;
		}
		textBounds.m_maxs.y = textBounds.m_mins.y + lineHeight;
		font.AddVertsForTextBox2D(textVerts, textBounds, lineHeight, m_lines[i].m_text, Rgba8::BLACK, fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN, 100, Vec2(0.8f,0.8f));
		font.AddVertsForTextBox2D(textVerts, textBounds, lineHeight, m_lines[i].m_text, m_lines[i].m_color, fontAspect, Vec2(0.f, 0.f), TextBoxMode::OVERRUN);
		renderer.BindTexture(&font.GetTexture());
		renderer.DrawVertexArray((int)textVerts.size(), textVerts.data());
		textBounds.m_mins.y += lineHeight;
	}

}

