#include "Timer.hpp"
#include "Engine/Core/Clock.hpp"
Timer::Timer(float period,  Clock const* clock)
	:m_period(period), m_clock(clock)
{
	if (clock == nullptr)
	{
		m_clock = &Clock::GetSystemClock();
	}
}

void Timer::Start()
{

	m_startTime = m_clock->GetTotalSeconds();

}

void Timer::Stop()
{
	m_startTime = 0.f;
}

float Timer::GetElapsedTime() const
{
	if (IsStopped())
	{
		return 0.f;
	}
	return m_clock->GetTotalSeconds()-m_startTime;
}

float Timer::GetElapsedFraction() const
{
	return GetElapsedTime()/m_period;
}

bool Timer::IsStopped() const
{
	return m_startTime == 0.f;
}

bool Timer::HasPeriodElapsed() const
{
	if (GetElapsedFraction() > 1 && !IsStopped())
	{
		return true;
	}
	return false;
}

bool Timer::DecrementPeriodIfElapsed()
{  
	if (HasPeriodElapsed())
	{
		m_startTime += m_period;
		return true;
	}
	return false;
}
