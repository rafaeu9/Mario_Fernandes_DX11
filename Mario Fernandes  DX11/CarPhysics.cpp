#include "CarPhysics.h"

CarPhysics::CarPhysics(Timer* time, float accelerationforce, float brakingForce)
{
	m_time = time;
	m_accelerationforce = accelerationforce;
	m_brakingForce = brakingForce;
}

CarPhysics::~CarPhysics()
{
}

void CarPhysics::Update()
{
	m_CurrentVelocity += m_time->GetDeltaTime() * m_acceleration;

	m_position += m_time->GetDeltaTime() * m_CurrentVelocity;

	if(XMVectorGetX(m_acceleration) + XMVectorGetY(m_acceleration) + XMVectorGetZ(m_acceleration) == 0)
		m_CurrentVelocity -= (m_CurrentVelocity * m_externalForce) * m_time->GetDeltaTime();

	if(XMVectorGetX(m_acceleration) + XMVectorGetY(m_acceleration) + XMVectorGetZ(m_acceleration) != 0)
		m_acceleration *= 0;

}

void CarPhysics::Accelerate()
{
	m_acceleration = m_direction * m_accelerationforce;
}

void CarPhysics::Brake()
{
	m_acceleration = -m_direction * m_brakingForce;
}

void CarPhysics::Rotate(float roationDegrees)
{
	m_rotation += roationDegrees * m_time->GetDeltaTime();

	float m_dx = sin(m_rotation * (XM_PI / 180.0)) + XMVectorGetX(m_direction);
	float m_dz = cos(m_rotation * (XM_PI / 180.0)) + XMVectorGetZ(m_direction);

	m_direction = XMVectorSet(m_dx, 0, m_dz,0);
	m_direction = XMVector3Normalize(m_direction);
}


