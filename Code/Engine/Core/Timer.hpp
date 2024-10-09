#pragma once

class Clock;

// Timer class that can be attached to any clock in a hierarchy and correctly handles duration regardless of update frequency.

class Timer
{
public:
	explicit Timer(float period,  Clock const* clock = nullptr); // Create a clock with a period and the specified clock. If the clock is null, use the system clock.

	void Start(); // Set the start time to the clock's current total time.
	void Stop(); // Set the start time back to zero.

	float GetElapsedTime() const; // Returns zero if stopped, otherwise returns the time elapsed between the clock's current time and our start time.

	float GetElapsedFraction() const; // Return the elapsed time as a percentage of our period. can be >1

	bool IsStopped() const;
	bool HasPeriodElapsed() const; // Returns true if our elapsed time is greater than our period and we are not stopped.

	bool DecrementPeriodIfElapsed(); // If a period has elapsed and we are not stopped, decrements a period by adding a period to the start time and returns true. Generally called within a loop until it returns false so the caller can process each elapsed period.

	const Clock* m_clock = nullptr; // Get the current time with this clock 
	float m_startTime = 0.f; // Clock time at which the timer was started. This is incremented by one period each time we decrement a period, however, so it is not an absolute start time. It is the start time of all periods that have not yet been decremented.
	float m_period = 0.f; // The time internal it takes for a period to elapse.
};