#pragma once
#include<d3d11.h>
#include "objfilemodel.h"
#include "text2D.h"

class reflectiveModel
{
public:
	reflectiveModel(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
	~reflectiveModel();

	HRESULT LoadObjModel(char* filename);
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
	void MovedFoward(float distance);

	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius() { return m_bounding_sphere_radius * m_scale; };

	bool CheckCollision(reflectiveModel* model);

private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ObjFileModel* m_pObject;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;

	ID3D11BlendState* m_pAlphaBlendEnable;
	ID3D11BlendState* m_pAlphaBlendDisable;

	ID3D11RasterizerState* rastStateCullNone;
	ID3D11RasterizerState* rastStateCullBack;


	float			m_x, m_y, m_z;
	float			m_xAngle, m_yAngle, m_zAngle;
	float			m_scale = 1;
	
	float			m_bounding_sphere_centre_x = 0, 
					m_bounding_sphere_centre_y = 0, 
					m_bounding_sphere_centre_z = 0, 
					m_bounding_sphere_radius = 0;

	
	void CalculateModelCentrePoint();		
	void CalculateBoundingSphereRadius();
};

