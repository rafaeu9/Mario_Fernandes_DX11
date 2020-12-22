#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include<math.h>

using namespace DirectX;

class camera
{
private:
	float m_x, m_y, m_z, m_dx, m_dz, m_camera_rotation;

	XMVECTOR m_position, m_lookat, m_up;
public:
	camera(float x, float y, float z, float rotation);

	void Rotate(float roationDegrees);
	void Forward(float distance);
	void Up(float distance);
	void Starf(float distance);
	XMMATRIX GetViewMatrix();

	float GetX() { return m_x; };
	float GetY() { return m_y; };
	float GetZ() { return m_z; };
};

