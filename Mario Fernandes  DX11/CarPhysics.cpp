#include "CarPhysics.h"

/// <summary>
/// Initialize car
/// </summary>
/// <param name="time"></param>
/// <param name="accelerationforce"></param>
/// <param name="brakingForce"></param>
CarPhysics::CarPhysics(Timer* time, float accelerationforce, float brakingForce)
{
	m_time = time;
	m_accelerationforce = accelerationforce;
	m_brakingForce = brakingForce;
}

CarPhysics::~CarPhysics()
{
}

/// <summary>
/// Update current velocity, drag and position
/// </summary>
void CarPhysics::Update()
{
	m_CurrentVelocity +=  m_acceleration * m_time->GetDeltaTime();

	m_position += m_CurrentVelocity  * m_time->GetDeltaTime();

	if(m_acceleration.x + m_acceleration.y + m_acceleration.z == 0)
		m_CurrentVelocity -= m_externalForce * m_CurrentVelocity * m_time->GetDeltaTime();
	else
		m_acceleration *= 0;

}

void CarPhysics::Accelerate()
{
	m_acceleration = m_direction * m_accelerationforce;
}

void CarPhysics::Boost(float extraSpeed)
{
	m_CurrentVelocity += m_CurrentVelocity * extraSpeed * m_time->GetDeltaTime();
}

void CarPhysics::Brake()
{
	m_acceleration = -m_direction * m_brakingForce;
}

void CarPhysics::Rotate(float roationDegrees)
{
	m_rotation += roationDegrees * m_time->GetDeltaTime();

	float m_dx = sin(m_rotation * (M_PI / 180.0)) + m_direction.x;
	float m_dz = cos(m_rotation * (M_PI / 180.0)) + m_direction.z;

	m_direction = { m_dx, 0, m_dz };
	m_direction = normalise(m_direction);
}


