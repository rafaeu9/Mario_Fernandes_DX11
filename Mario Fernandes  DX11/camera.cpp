#include "camera.h"

camera::camera(float x, float y, float z, float rotation)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = rotation;

	m_lookat = XMVectorSet( 0,0,0,0 );

	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));	
}

void camera::Rotate(float roationDegrees)
{
	m_camera_rotation += roationDegrees;

	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
}

void camera::Forward(float distance)
{
	m_x += distance * m_dx;
	m_z += distance * m_dz;
}

void camera::Up(float distance)
{
	m_y += distance;
}

void camera::Starf(float distance)
{
	//calculate forward vector, which is calculated from 2 normalised points we know: lookat minus position
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);
	//calculate right vector, which is a normalised perpendicular vector to forward and up (hint: use cross product)
	XMVECTOR right = XMVector3Cross(m_up, forward);
	//update m_x and m_z, by multiplying right x and z by distance
	m_x += XMVectorGetX(right) * distance;
	m_z += XMVectorGetZ(right) * distance;

}

XMMATRIX camera::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	//m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookat, m_up);
}

void camera::SetPosition(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void camera::LookAt_XZ(float x, float z)
{
	m_lookat = XMVectorSet(x, m_y, z, 0.0);
}

void camera::LookAt_XYZ(float x, float y, float z)
{
	m_lookat = XMVectorSet(x, y, z, 0.0);
}
