#pragma once
#include<d3d11.h>
#include "text2D.h"

class skyBox
{
public:
	skyBox(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
	~skyBox();

	void Draw(XMMATRIX* view, XMMATRIX* projection);

	void Follow(float x, float y, float z);
	HRESULT AddTexture(char* filename);

private:

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;

	ID3D11RasterizerState*		m_pRasterSolid = 0;
	ID3D11RasterizerState*		m_pRasterSkybox = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSolid = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSkybox = 0;


	float			m_x, m_y, m_z;
	float			m_scale;

	HRESULT LoadModel();
};

