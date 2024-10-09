#pragma once
#include <vector>
//Hierarchical clock that inherits time scale. Parent clocks pass scaled delta seconds down to child clocks to be used as their delta seconds. Child clocks in turn scale that time and pass that down to their children. There is one system clock at the root of the hierarchy. 
class Clock;

class Clock
{
public:
	Clock(); //Uses the system clock as the parent of the new clock.
	explicit Clock(Clock& parent); //Constructor to specify a parent clock for the new clock

	~Clock();
	Clock(const Clock& copy) = delete;

	void Reset(); // Reset all book keeping variables values back to zero and then set the last updated time to be the current system time.
	bool IsPaused() const;
	void Pause();
	void Unpause();
	void TogglePause();

	void StepSingleFrame(); //Unpause for frame then pause again the next frame.039348
	bool IsStepSingleFrame() const;
	void SetTimeScale(float timeScale); //Set and get the value by which this clock scales delta seconds.
	float GetTImeScale() const;

	float GetDeltaSeconds() const;
	float GetTotalSeconds() const;
	void SetDeltaSeconds(float deltaSeconds);
	float GetFrameCount() const;

public:
	void Tick();	 //Calculates the current delta seconds and clamps it to the max delta time, sets the last updated time, then calls Advance, passing down the delta seconds.
	static Clock& GetSystemClock(); //Returns a reference to a static system clock that by default will be the parent of all other clocks if a parent is not specified.
	static void TickSystemClock(); //Called in BeginFrame to Tick the system clock, which will in turn Advance the system clock, which will in turn Advance all of its children, thus updating the entire hierarchy.
	int  m_frameCount = 0;
protected:


	void Advance(float deltaTimeSeconds);//Calculates delta seconds based on pausing and time scale, updates all remaining book keeping variables, calls Advance on all child clocks and passes down our delta seconds, and handles pausing after frames for stepping single frames.

	void AddChild(Clock* childClock);//Add a child clock as one of our children. Does not handle cases where the child clock already has a parent.

	void RemoveChild(Clock* childClock); //Removes a child clock from our children if it is a child, otherwise does nothing.

protected:
	Clock* m_parent = nullptr;
	std::vector<Clock*> m_children;

	//Book keeping variables.
	float m_lastUpdateTimeInSeconds = 0.f;
	float m_totalSeconds = 0.f;
	float m_deltaSeconds = 0.f;

	float m_timeScale = 1.f;
	bool m_isPaused = false;
	bool m_stepSingleFrame = false;
	float m_maxDeltaSeconds = 0.1f; //Useful for preventing large time steps when stepping in a debugger.

};