#include "skyBox.h"

struct SKYBOX_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; //64 bytes (4 x 4 = 16 floats x 4 bytes)

};// TOTAL SIZE = 64 bytes

/// <param name="D3DDevice"></param>
/// <param name="ImmediateContext"></param>
skyBox::skyBox(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	m_x = m_y = m_z = 0;
	m_scale = 3.0f;

	LoadModel();

}

skyBox::~skyBox()
{
	if (m_pRasterSolid != 0) m_pRasterSolid->Release();
	if (m_pRasterSkybox != 0) m_pRasterSkybox->Release();
	if (m_pDepthWriteSolid != 0) m_pDepthWriteSolid->Release();
	if (m_pDepthWriteSkybox != 0) m_pDepthWriteSkybox->Release();
}

/// <param name="view"></param>
/// <param name="projection"></param>
void skyBox::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	//Set vertex buffer
	UINT stride = sizeof(POS_TEX_VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	XMMATRIX world;
	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	//Constant buffer
	SKYBOX_CONSTANT_BUFFER cb_value;
	cb_value.WorldViewProjection = world * (*view) * (*projection);
	
	//upload the new value for the constant buffer
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &cb_value, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);
	
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pImmediateContext->RSSetState(m_pRasterSkybox);

	m_pImmediateContext->OMSetDepthStencilState(m_pDepthWriteSkybox,0);

	m_pImmediateContext->Draw(sizeof(SKYBOX_CONSTANT_BUFFER), 0);

	m_pImmediateContext->RSSetState(m_pRasterSolid);

	m_pImmediateContext->OMSetDepthStencilState(m_pDepthWriteSolid,0);
}

/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void skyBox::Follow(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

HRESULT skyBox::AddTexture(char* filename)
{
	HRESULT hr = S_OK;

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice,
		filename, NULL, NULL,
		&m_pTexture, NULL);
	if (!m_pTexture) return S_FALSE;

	return hr;
}

HRESULT skyBox::LoadModel()
{
	HRESULT hr = S_OK;


	POS_TEX_VERTEX vertices[] =
	{
		// back face 
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},

		// front face
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},

		// left face
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},

		// right face
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},

		// bottom face
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},

		// top face
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT2(0.0f, 1.0f)}
	};


	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);
	if (FAILED(hr)) return hr;//Resturn an error code if failed


	//Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);
	//Create the buffer

	if (FAILED(hr)) return hr;

	//Create constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = sizeof(SKYBOX_CONSTANT_BUFFER);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;

	//Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//Lock the buffer to allow writing
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//Copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));

	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);


	//Load and compile the pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob* VS, * PS, * error;
	hr = D3DX11CompileFromFile("SkyBox.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) return hr;
	}

	hr = D3DX11CompileFromFile("SkyBox.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) return hr;
	}

	//Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);
	if (FAILED(hr)) return hr;

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);
	if (FAILED(hr)) return hr;

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very carful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },		
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},		
	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr)) return hr;


	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterSolid);
	if (FAILED(hr)) return hr;

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterSkybox);
	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_DESC DSDecsc;
	ZeroMemory(&DSDecsc, sizeof(DSDecsc));

	DSDecsc.DepthEnable = true;
	DSDecsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSDecsc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = m_pD3DDevice->CreateDepthStencilState(&DSDecsc, &m_pDepthWriteSolid);
	if (FAILED(hr)) return hr;

	DSDecsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_pD3DDevice->CreateDepthStencilState(&DSDecsc, &m_pDepthWriteSkybox);
	if (FAILED(hr)) return hr;


	return hr;
}
