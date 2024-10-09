#include "DebugRender.hpp"
#include "Engine/Render/Renderer.hpp"
#include "Engine/Render/BitmapFont.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <mutex>
struct DebugRenderGeometry
{
	//DebugRenderGeometry();
public:
	std::vector <Vertex_PCU> m_vertexes;
	Timer* m_timer = nullptr;
	Rgba8 m_startColor = Rgba8::WHITE;
	Rgba8 m_endColor = Rgba8::WHITE;
	Rgba8 m_currentColor = Rgba8::WHITE;
	Texture* m_texture = nullptr;
	DebugRenderMode m_mode = DebugRenderMode::USE_DEPTH;
	RasterizerMode m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	BillboardType m_billboardType = BillboardType::NONE;
	Vec3 m_billboardPosition;
};
struct DebugScreenMessage
{
	Timer* m_timer = nullptr;
	std::vector <Vertex_PCU> m_vertexes;
};
// DebugRenderGeometry::DebugRenderGeometry()
// {
// 
// }

class DebugRenderSystem
{
public:
	DebugRenderSystem(DebugRenderConfig const& config);
	void Startup();
	void BeginFrame();
	void Update();
	void Render();
	void EndFrame();
	void ShutDown();
	void Clear();
	void RenderWorld(Camera const& camera);
	void RenderScreen(Camera const& camera);
	void AddWorldPoint(Vec3 const& position, float radius, float duration, Rgba8 const& startColor, DebugRenderMode mode);
	void AddWorldWireCylinder(Vec3 const& basePos, Vec3 const& topPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode);
	void AddWorldWiredSphere(Vec3 const& centerPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode);
	void AddWorldLine(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor , Rgba8 const& endColor, DebugRenderMode mode);
	void AddWorldArrow(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode);
	void AddWorldBasis(Mat44 const& transform, float duration, DebugRenderMode mode, float scale = 1.f);
	void AddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode);
	void AddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode);
	void AddScreenText(std::string const& text, AABB2 const& textBox, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColo, DebugRenderMode mode);
	void AddMessage(std::string const& text, float duration, Rgba8 const& startColor, Rgba8 const& endColor);
public:
	DebugRenderConfig m_config;
	DebugRenderMode m_mode = DebugRenderMode::USE_DEPTH;
	bool m_isShowing = true;
	BitmapFont* m_bitMapFont = nullptr;
	Camera m_worldCamera;
	Camera m_screenCamera;
	std::vector<Vertex_PCU> m_billboardTextVerts;
	std::vector<DebugRenderGeometry> m_geometries;
	std::vector<Vertex_PCU> m_screenTextVerts;
	std::vector<DebugScreenMessage> m_screenMessages;
	mutable std::mutex m_debugRenderMutex;
};

DebugRenderSystem::DebugRenderSystem(DebugRenderConfig const& config)
	:m_config(config)
{

}

void DebugRenderSystem::Startup()
{
	m_debugRenderMutex.lock();
	Clear();
	m_debugRenderMutex.unlock();
	m_bitMapFont = m_config.m_renderer->CreateOrGetBitmapFont((std::string("Data/Fonts/") + m_config.m_fontName).c_str());

	g_theEventSystem->SubscribeEventCallbackFunction("DebugRenderClear", Command_DebugRenderClear);
	g_theEventSystem->SubscribeEventCallbackFunction("DebugRenderToggle", Command_DebugRenderToggle);
}

void DebugRenderSystem::BeginFrame()
{
	//Processing the system message
	for (auto it = m_screenMessages.begin(); it != m_screenMessages.end();)
	{
		if (it->m_timer && it->m_timer->HasPeriodElapsed())
		{
			size_t index = std::distance(m_screenMessages.begin(), it);
			m_debugRenderMutex.lock();
			it = m_screenMessages.erase(it);
			for (int i = (int)index; i < (int)m_screenMessages.size(); ++i)
			{
				TransformVertexArray3D(m_screenMessages[i].m_vertexes, Mat44(Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), Vec3(0.f, 15.f, 0.f)));
			}
			m_debugRenderMutex.unlock();
		}
		else
		{
			++it;
		}
	}
	// Erase the geometries that has elapsed its timer period
	for (auto it = m_geometries.begin(); it != m_geometries.end();)
	{
		if (it->m_timer && it->m_timer->HasPeriodElapsed())
		{
			m_debugRenderMutex.lock();
			it = m_geometries.erase(it);
			m_debugRenderMutex.unlock();
		}
		else if (it->m_timer)
		{
			m_debugRenderMutex.lock();
			it->m_currentColor = InterpolateFromNewColor(it->m_startColor, it->m_endColor, it->m_timer->GetElapsedFraction());
			m_debugRenderMutex.unlock();
			++it;
		}
		else
		{
			++it;
		}
	}


}

void DebugRenderSystem::Update()
{
}

void DebugRenderSystem::Render()
{
}

void DebugRenderSystem::EndFrame()
{
	m_screenTextVerts.clear();
}

void DebugRenderSystem::ShutDown()
{
	m_debugRenderMutex.lock();
	Clear();
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::Clear()
{
	m_geometries.clear();
	m_screenTextVerts.clear();
	m_billboardTextVerts.clear();
	m_screenMessages.clear();
}

void DebugRenderSystem::RenderWorld(Camera const& camera)
{
	m_debugRenderMutex.lock();
	if (m_isShowing)
	{
 		m_config.m_renderer->BeginCamera(camera);
		m_config.m_renderer->BindTexture(nullptr);
		for (int i = 0; i < (int)m_geometries.size(); ++i)
		{
			m_config.m_renderer->SetRasterizerMode(m_geometries[i].m_rasterizerMode);
			if (m_geometries[i].m_billboardType == BillboardType::FULL_OPPOSING)
			{
				m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
				m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
				m_config.m_renderer->SetModelConstants(GetBillboardMatrix(BillboardType::FULL_OPPOSING,camera.GetCameraOrientation().GetAsMatrix_IFwd_JLeft_KUp(),m_geometries[i].m_billboardPosition), m_geometries[i].m_currentColor);
				m_config.m_renderer->BindTexture(m_geometries[i].m_texture);
				m_config.m_renderer->DrawVertexArray((int)m_geometries[i].m_vertexes.size(), m_geometries[i].m_vertexes.data());
			}
			else
			{ 
			    if (m_geometries[i].m_mode == DebugRenderMode::ALWAYS)
			    {
			    	m_config.m_renderer->SetDepthMode(DepthMode::DISABLED);
			    	m_config.m_renderer->SetModelConstants(Mat44(), m_geometries[i].m_currentColor);
			    	m_config.m_renderer->BindTexture(m_geometries[i].m_texture);
			    	m_config.m_renderer->DrawVertexArray((int)m_geometries[i].m_vertexes.size(), m_geometries[i].m_vertexes.data());
			    }
			    else if (m_geometries[i].m_mode == DebugRenderMode::USE_DEPTH)
			    {
			    	m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
					m_config.m_renderer->SetBlendMode(BlendMode::OPAQUE);
			    	m_config.m_renderer->SetModelConstants(Mat44(), m_geometries[i].m_currentColor);
			    	m_config.m_renderer->BindTexture(m_geometries[i].m_texture);
			    	m_config.m_renderer->DrawVertexArray((int)m_geometries[i].m_vertexes.size(), m_geometries[i].m_vertexes.data());
			    }
			}
		}
		for (int i = 0; i < (int)m_geometries.size(); ++i)
		{
			if (m_geometries[i].m_mode == DebugRenderMode::X_RAY)
			{

				m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
				m_config.m_renderer->SetDepthMode(DepthMode::DISABLED);
				m_config.m_renderer->BindTexture(m_geometries[i].m_texture);
				Rgba8 xrayColor = m_geometries[i].m_currentColor;
				xrayColor += Rgba8(20, 20, 20, 0);
				xrayColor.a = 125;
				m_config.m_renderer->SetModelConstants(Mat44(), xrayColor);
				m_config.m_renderer->DrawVertexArray((int)m_geometries[i].m_vertexes.size(), m_geometries[i].m_vertexes.data());

				m_config.m_renderer->SetBlendMode(BlendMode::OPAQUE);
				m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
				m_config.m_renderer->SetModelConstants(Mat44(), m_geometries[i].m_currentColor);
				m_config.m_renderer->DrawVertexArray((int)m_geometries[i].m_vertexes.size(), m_geometries[i].m_vertexes.data());
			}
		}
		m_config.m_renderer->EndCamera(camera);
	}
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::RenderScreen(Camera const& camera)
{
	m_debugRenderMutex.lock();
	if (m_isShowing)
	{
		m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
		m_config.m_renderer->BeginCamera(camera);
		m_config.m_renderer->BindTexture(&m_bitMapFont->GetTexture());
		m_config.m_renderer->SetModelConstants();
		m_config.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
		for (int i = 0; i < (int)m_screenMessages.size(); ++i)
		{
			m_config.m_renderer->DrawVertexArray((int)m_screenMessages[i].m_vertexes.size(), m_screenMessages[i].m_vertexes.data());
		}
		m_config.m_renderer->DrawVertexArray((int)m_screenTextVerts.size(), m_screenTextVerts.data());
		m_config.m_renderer->EndCamera(camera);
	}
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldPoint(Vec3 const& position, float radius, float duration, Rgba8 const& startColor, DebugRenderMode mode)
{
	DebugRenderGeometry point;
	point.m_mode = mode;
	point.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	if (duration == 0.f)
	{
		point.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		point.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		point.m_timer = new Timer(duration);
		point.m_timer->Start();
	}
	AddVertsForSphere3D(point.m_vertexes, position, radius, startColor);
	m_debugRenderMutex.lock();
	m_geometries.push_back(point);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldWireCylinder(Vec3 const& basePos, Vec3 const& topPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry cylinder;
	cylinder.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	cylinder.m_currentColor = startColor;
	cylinder.m_startColor = startColor;
	cylinder.m_endColor = endColor;
	cylinder.m_mode = mode;
	if (duration == 0.f)
	{
		cylinder.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		cylinder.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		cylinder.m_timer = new Timer(duration);
		cylinder.m_timer->Start();
	}

	AddVertsForCylinder3D(cylinder.m_vertexes, basePos, topPos, radius,Rgba8::WHITE,Vec2(),Vec2(1.f,1.f),16);
	m_debugRenderMutex.lock();
	m_geometries.push_back(cylinder);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldWiredSphere(Vec3 const& centerPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry point;
	point.m_startColor = startColor;
	point.m_endColor = endColor;
	point.m_currentColor = startColor;
	point.m_mode = mode;
	point.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	if (duration == 0.f)
	{
		point.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		point.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		point.m_timer = new Timer(duration);
		point.m_timer->Start();
	}
	AddVertsForSphere3D(point.m_vertexes, centerPos, radius, Rgba8::WHITE);
	m_debugRenderMutex.lock();
	m_geometries.push_back(point);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldArrow(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry arrow;
	arrow.m_mode = mode;
	arrow.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	arrow.m_currentColor = startColor;
	arrow.m_startColor = startColor;
	arrow.m_endColor = endColor;
	if (duration == 0.f)
	{
		arrow.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		arrow.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		arrow.m_timer = new Timer(duration);
		arrow.m_timer->Start();
	}
	AddVertsForArrow3D(arrow.m_vertexes, startPos, endPos, radius, Rgba8::WHITE, Vec2(), Vec2(1.f,1.f),16);
	m_debugRenderMutex.lock();
	m_geometries.push_back(arrow);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldLine(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry line;
	line.m_mode = mode;
	line.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	line.m_currentColor = startColor;
	line.m_startColor = startColor;
	line.m_endColor = endColor;
	if (duration == 0.f)
	{
		line.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		line.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		line.m_timer = new Timer(duration);
		line.m_timer->Start();
	}
	AddVertsForCylinder3D(line.m_vertexes, startPos, endPos, radius, Rgba8::WHITE);
	m_debugRenderMutex.lock();
	m_geometries.push_back(line);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldBasis(Mat44 const& transform, float duration, DebugRenderMode mode, float scale)
{
	DebugRenderGeometry basis;
	basis.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	basis.m_mode = mode;
	if (duration == 0.f)
	{
		basis.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		basis.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		basis.m_timer = new Timer(duration);
		basis.m_timer->Start();
	}
	AddVertsForCylinder3D(basis.m_vertexes, Vec3(0.f, 0.f, 0.f),Vec3(scale * 0.7f, 0.f, 0.f), scale * 0.1f, Rgba8::RED);
	AddVertsForCone3D(basis.m_vertexes,   Vec3(scale * 0.7f, 0.f, 0.f), Vec3(scale * 1.2f, 0.f, 0.f), scale * 0.17f, Rgba8::RED);
	AddVertsForCylinder3D(basis.m_vertexes, Vec3(0.f, 0.f, 0.f), Vec3(0.f, scale * 0.7f, 0.f), scale * 0.1f, Rgba8::GREEN);
	AddVertsForCone3D(basis.m_vertexes, Vec3(0.f, scale * 0.7f, 0.f),  Vec3(0.f, scale * 1.2f, 0.f), scale * 0.17f, Rgba8::GREEN);
	AddVertsForCylinder3D(basis.m_vertexes, Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, scale * 0.7f), scale * 0.1f, Rgba8::BLUE);
	AddVertsForCone3D(basis.m_vertexes, Vec3(0.f, 0.f, scale*0.7f), Vec3(0.f, 0.f, scale * 1.2f), scale * 0.17f, Rgba8::BLUE);

	TransformVertexArray3D(basis.m_vertexes, transform);
	m_debugRenderMutex.lock();
	m_geometries.push_back(basis);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	UNUSED(mode);
	DebugRenderGeometry textGeometery;
	textGeometery.m_texture = &m_bitMapFont->GetTexture();
	textGeometery.m_mode = DebugRenderMode::USE_DEPTH;
	textGeometery.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	textGeometery.m_currentColor = startColor;
	textGeometery.m_startColor = startColor;
	textGeometery.m_endColor = endColor;
	if (duration == 0.f)
	{
		textGeometery.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		textGeometery.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		textGeometery.m_timer = new Timer(duration);
		textGeometery.m_timer->Start();
	}

	m_bitMapFont->AddVertsForTextBox3DArOriginXForward(textGeometery.m_vertexes, textHeight, text, Rgba8::WHITE, 1.f, alignment);
	TransformVertexArray3D(textGeometery.m_vertexes, transform);
	m_debugRenderMutex.lock();
	m_geometries.push_back(textGeometery);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	UNUSED(mode);
	DebugRenderGeometry textGeometery;
	textGeometery.m_texture = &m_bitMapFont->GetTexture();
	textGeometery.m_billboardType = BillboardType::FULL_OPPOSING;
	textGeometery.m_mode = DebugRenderMode::USE_DEPTH;
	textGeometery.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	textGeometery.m_currentColor = startColor;
	textGeometery.m_startColor = startColor;
	textGeometery.m_endColor = endColor;
	textGeometery.m_billboardPosition = origin;
	if (duration == 0.f)
	{
		textGeometery.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		textGeometery.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		textGeometery.m_timer = new Timer(duration);
		textGeometery.m_timer->Start();
	}
	m_bitMapFont->AddVertsForTextBox3DArOriginXForward(textGeometery.m_vertexes, textHeight, text, Rgba8::WHITE, 1.f, alignment);
	//Mat44 localMatrix = EulerAngles(90.f, 0.f, 90.f).GetAsMatrix_IFwd_JLeft_KUp();

	//TransformVertexArray3D(textGeometery.m_vertexes, localMatrix);
	m_debugRenderMutex.lock();
	m_geometries.push_back(textGeometery);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddScreenText(std::string const& text, AABB2 const& textBox, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	UNUSED(duration);
	UNUSED(mode);
	Rgba8 color = InterpolateFromNewColor(startColor, endColor, 1.f);	
	m_debugRenderMutex.lock();
	m_bitMapFont->AddVertsForTextBox2D(m_screenTextVerts, textBox, textHeight, text, color, 1.f, alignment);
	m_debugRenderMutex.unlock();
}

void DebugRenderSystem::AddMessage(std::string const& text, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	UNUSED(endColor);
	DebugScreenMessage message;
	if (duration == 0.f)
	{
		message.m_timer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		message.m_timer->Start();
	}
	else if (duration != -1.f)
	{
		message.m_timer = new Timer(duration);
		message.m_timer->Start();
	}

	AABB2 textBounds = AABB2(0.f,800.f - 15.f * ((int)m_screenMessages.size()+2), 400.f, 800.f - 15.f * (int)m_screenMessages.size()+1 );
	m_bitMapFont->AddVertsForTextBox2D(message.m_vertexes, textBounds, 15.f, text, startColor, 1.f,Vec2());
	m_debugRenderMutex.lock();
	m_screenMessages.push_back(message);
	m_debugRenderMutex.unlock();
}

DebugRenderSystem* g_debugRenderSystem = nullptr;
void DebugRenderSystemStartup(DebugRenderConfig const& config)
{
	g_debugRenderSystem = new DebugRenderSystem(config);
	g_debugRenderSystem->Startup();
}

void DebugRenderSystemShutdown()
{
	g_debugRenderSystem->ShutDown();
}

void DebugRenderSetVisible()
{
	g_debugRenderSystem->m_debugRenderMutex.lock();
	g_debugRenderSystem->m_isShowing = true;
	g_debugRenderSystem->m_debugRenderMutex.unlock();
}

void DebugRenderSetHidden()
{
	g_debugRenderSystem->m_debugRenderMutex.lock();
	g_debugRenderSystem->m_isShowing = false;
	g_debugRenderSystem->m_debugRenderMutex.unlock();
}

void DebugRenderClear()
{
	g_debugRenderSystem->Clear();
}

void DebugRenderBeginFrame()
{
	g_debugRenderSystem->BeginFrame();
}

void DebugRenderWorld(Camera const& camera)
{
	g_debugRenderSystem->RenderWorld(camera);
}

void DebugRenderScreen(Camera const& camera)
{
	g_debugRenderSystem->RenderScreen(camera);
}

void DebugRenderEndFrame()
{
	g_debugRenderSystem->EndFrame();
}

void DebugAddWorldPoint(Vec3 const& position, float radius, float duration, Rgba8 const& startColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldPoint(position, radius, duration, startColor, mode);
}

void DebugAddWorldLine(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldLine(startPos, endPos, radius, duration, startColor, endColor, mode);
}


void DebugAddWorldWireCylinder(Vec3 const& basePos, Vec3 const& topPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldWireCylinder(basePos, topPos, radius, duration, startColor, endColor, mode);
}

void DebugAddWorldWiredSphere(Vec3 const& centerPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldWiredSphere(centerPos, radius, duration, startColor, endColor, mode);
}

void DebugAddWorldArrow(Vec3 const& startPos, Vec3 const& endPos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldArrow(startPos, endPos, radius, duration, startColor, endColor, mode);
}

bool Command_DebugRenderClear(EventArgs& args)
{
	UNUSED(args);
	g_debugRenderSystem->Clear();
	return true;
}

bool Command_DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);
	g_debugRenderSystem->m_isShowing = !g_debugRenderSystem->m_isShowing;
	return true;
}

void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldText(text, transform, textHeight, alignment, duration, startColor, endColor, mode);
}

void DebugAddWorldBasis(Mat44 const& transform, float duration, DebugRenderMode mode, float scale)
{
	g_debugRenderSystem->AddWorldBasis(transform, duration, mode, scale);
}

void DebugAddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddWorldBillboardText(text, origin, textHeight, alignment, duration, startColor, endColor, mode);
}

void DebugAddScreenText(std::string const& text, AABB2 const& textBox, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	g_debugRenderSystem->AddScreenText(text, textBox, textHeight, alignment, duration, startColor, endColor, mode);
}

void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	g_debugRenderSystem->AddMessage(text, duration, startColor, endColor);
}
