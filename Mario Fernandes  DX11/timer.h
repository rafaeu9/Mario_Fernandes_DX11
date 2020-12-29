#pragma once
#include <time.h>
#include <stdio.h>

class Timer
{
public:
	Timer();

	void Reset();
	void Update();

	float GetDeltaTime() { return m_DeltaTime; }
	double GetTime() { return m_Time; }

private:
	float           m_DeltaTime;
	double          m_Time;
	unsigned long   m_Ticks;
};

