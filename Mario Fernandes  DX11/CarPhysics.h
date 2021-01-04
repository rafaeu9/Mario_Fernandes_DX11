#pragma once

#include "MathMario.h"
#include "timer.h"


class CarPhysics
{
public:

	CarPhysics(Timer* time, float m_acceleration, float brakingForce);
	~CarPhysics();

	void Update();

	void Accelerate();
	void Boost(float extraSpeed);
	void Brake();
	void Rotate(float roationDegrees);

	Vector3 GetPosition() { return m_position; };
	Vector3 GetDirection() { return m_direction; };
	Vector3 GetCurrentVelocity() { return m_CurrentVelocity; };

	float GetRot() { return m_rotation; };
	void SetPos(Vector3 Pos) { m_position = Pos; };

	void StopCar() { m_CurrentVelocity = { 0, 0, 0 }; };

private:

	Vector3 m_direction = { 0, 0, 1 };
	Vector3 m_position = { 0, 0, 0 };
	Vector3 m_acceleration = { 0, 0, 0 };
	Vector3 m_CurrentVelocity = { 0, 0, 0 };
	Vector3 m_externalForce = { 0.9f,0,0.9f };

	float m_accelerationforce = 0, m_brakingForce = 0, m_rotation = -90;
	

	Timer* m_time;
};

