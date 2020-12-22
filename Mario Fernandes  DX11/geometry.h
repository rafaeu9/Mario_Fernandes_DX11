#pragma once
#include<d3d11.h>
#include "text2D.h"

class geometry
{
public:
	geometry(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
	~geometry();


	HRESULT AddTexture(char* filename);

	void Draw(XMMATRIX* view, XMMATRIX* projection);

	void SetXPos(float num) { m_x = num; };
	void SetYPos(float num) { m_y = num; };
	void SetZPos(float num) { m_z = num; };

	void SetXRot(float num) { m_xAngle = num; };
	void SetYRot(float num) { m_yAngle = num; };
	void SetZRot(float num) { m_zAngle = num; };

	void SetScale(float num) { m_scale = num; };

	float GetXPos() { return m_x; };
	float GetYPos() { return m_y; };
	float GetZPos() { return m_z; };

	float GetXRot() { return m_xAngle; };
	float GetYRot() { return m_yAngle; };
	float GetZRot() { return m_zAngle; };

	float GetScale() { return m_scale; };

	void ModXPos(float num) { m_x += num; };
	void ModYPos(float num) { m_y += num; };
	void ModZPos(float num) { m_z += num; };

	void ModXRot(float num) { m_xAngle += num; };
	void ModYRot(float num) { m_yAngle += num; };
	void ModZRot(float num) { m_zAngle += num; };

	void ModScale(float num) { m_scale += num; };

	void LookAt_XZ(float x, float y);

private:

	float			m_x, m_y, m_z;
	float			m_xAngle, m_yAngle, m_zAngle;
	float			m_scale = 1;
};

