#pragma once

#include<DirectXMath.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT

using namespace DirectX;

#include "timer.h"


class CarPhysics
{
public:

	CarPhysics(Timer* time, float m_acceleration, float brakingForce);
	~CarPhysics();

	void Update();

	void Accelerate();
	void Brake();
	void Rotate(float roationDegrees);

	XMVECTOR GetPosition() { return m_position; };
	XMVECTOR GetDirection() { return m_direction; };
	XMVECTOR GetCurrentVelocity() { return m_CurrentVelocity; };

	float GetRot() { return m_rotation; };

	XMVECTOR GetPos() { return m_position; };
	void SetPos(XMVECTOR Pos) { m_position = Pos; };

	void StopCar() { m_CurrentVelocity = XMVectorZero(); };

private:

	XMVECTOR m_direction = XMVectorSet( 0, 0, 1, 0 );
	XMVECTOR m_position = XMVectorSet(0, 0, 0, 0 );
	XMVECTOR m_acceleration = XMVectorSet(0, 0, 0, 0 );
	XMVECTOR m_CurrentVelocity = XMVectorSet(0, 0, 0, 0);
	XMVECTOR m_externalForce = XMVectorSet(0.9f,0,0.9f,0);

	float m_accelerationforce = 0, m_brakingForce = 0, m_rotation = 0;
	

	Timer* m_time;
};

