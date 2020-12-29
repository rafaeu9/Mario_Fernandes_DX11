#include "timer.h"

Timer::Timer()
	: m_DeltaTime(0.f)
	, m_Time(0.)
	, m_Ticks(0)
{

	Reset();
}

void Timer::Reset()
{
	m_Time = 0.;
	m_DeltaTime = 0.f;
	m_Ticks = clock();
}

void Timer::Update()
{
	long ticks = clock();

	m_DeltaTime = float((double)(ticks - m_Ticks) / CLOCKS_PER_SEC);
	if (m_DeltaTime > 0.2f)
	{
		m_DeltaTime = 0.2f;
	}

	m_Time = m_Time + m_DeltaTime;
	m_Ticks = ticks;
}