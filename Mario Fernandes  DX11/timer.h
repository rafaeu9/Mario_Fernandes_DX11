#pragma once
#include <time.h>
#include <stdio.h>

class Timer
{
public:
	Timer();

	void Reset();
	void Update();

	float GetTimeDelta() { return m_TimeDelta; }
	double GetTime() { return m_Time; }

private:
	float           m_TimeDelta;
	double          m_Time;
	unsigned long   m_Ticks;
};

