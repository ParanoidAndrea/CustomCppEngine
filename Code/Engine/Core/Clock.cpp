#include "Clock.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Time.hpp"
static Clock g_theSystemClock;

Clock::Clock()
{
	if (&g_theSystemClock)
	{
        g_theSystemClock.AddChild(this);
    }
}

Clock::Clock(Clock& parent)
    :m_parent(&parent)
{
    m_parent->AddChild(this);
}

Clock::~Clock()
{
    m_parent->RemoveChild(this);
	for (int i = 0; i < (int)m_children.size(); ++i)
	{
		RemoveChild(m_children[i]);
	}

}

void Clock::Reset()
{
    m_lastUpdateTimeInSeconds = 0.f;
    m_totalSeconds = 0.f;
    m_deltaSeconds = 0.f;
    m_frameCount = 0;
    m_lastUpdateTimeInSeconds = GetSystemClock().GetTotalSeconds();
}

bool Clock::IsPaused() const
{
    return m_isPaused;
}

void Clock::Pause()
{
    m_isPaused = true;
}

void Clock::Unpause()
{
    m_isPaused = false;
}

void Clock::TogglePause()
{
    m_isPaused = !m_isPaused;
}

void Clock::StepSingleFrame()
{
    Unpause();
    m_stepSingleFrame = true;
}

bool Clock::IsStepSingleFrame() const
{
    return m_stepSingleFrame;
}

void Clock::SetTimeScale(float timeScale)
{
    m_timeScale = timeScale;
}

float Clock::GetTImeScale() const
{
    return m_timeScale;
}

float Clock::GetDeltaSeconds() const
{
    return m_deltaSeconds;
}

float Clock::GetTotalSeconds() const
{
    return m_totalSeconds;
}

void Clock::SetDeltaSeconds(float deltaSeconds)
{
    m_deltaSeconds = deltaSeconds;
}

float Clock::GetFrameCount() const
{
    return (float)m_frameCount;
}

Clock& Clock::GetSystemClock()
{
    return g_theSystemClock;
}

void Clock::TickSystemClock()
{
    g_theSystemClock.Tick();
}

void Clock::Tick()
{
	float currentTime = (float)GetCurrentTimeSeconds();
    m_deltaSeconds = currentTime - m_lastUpdateTimeInSeconds;
    m_lastUpdateTimeInSeconds = currentTime;

    Advance(m_deltaSeconds);
}

void Clock::Advance(float deltaTimeSeconds)
{
    if (m_isPaused)
    {
        m_deltaSeconds = 0.f;
    }
    else
    {
        m_deltaSeconds = deltaTimeSeconds * m_timeScale;
    }
	if ((float)(m_frameCount + 1) / m_deltaSeconds >= 1.f)
	{
		m_frameCount = 0;
	}
	m_frameCount++;

	m_totalSeconds += m_deltaSeconds;

   // m_frameCount = 1.f / m_deltaSeconds;
    for (int i = 0; i < (int)m_children.size(); ++i)
    {
        m_children[i]->Advance(m_deltaSeconds);
    }

    if (m_stepSingleFrame)
    {
        m_stepSingleFrame = false;
        Pause();
    }
    m_deltaSeconds = GetClamped(m_deltaSeconds, 0.f, m_maxDeltaSeconds);
}

void Clock::AddChild(Clock* childClock)
{
    if (childClock->m_parent != nullptr)
    {
        childClock->m_parent = this;
        m_children.push_back(childClock);
    }
}

void Clock::RemoveChild(Clock* childClock)
{
	if (childClock->m_parent != nullptr)
	{
        childClock->m_parent = nullptr;
		auto it = std::find(m_children.begin(), m_children.end(), childClock);
		if (it != m_children.end()) 
        { 
            m_children.erase(it); 
        }
        
	}
}
