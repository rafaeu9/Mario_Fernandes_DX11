#include "Model.h"


struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; //64 bytes (4 x 4 = 16 floats x 4 bytes)
	XMVECTOR directional_light_vector;	//16 bytes
	XMVECTOR directional_light_colour;		//16 bytes
	XMVECTOR ambient_light_colour;			//16 bytes
};// TOTAL SIZE = 64 bytes

/// <param name="D3DDevice"></param>
/// <param name="ImmediateContext"></param>
Model::Model(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;
	m_x = m_y = m_z = 0.0f;
	m_xAngle = m_yAngle = m_zAngle = 0.0f;
	m_scale = 1.0f;
}

Model::~Model()
{
	if (m_pObject)
	{
		delete m_pObject;
		m_pObject = nullptr;
	}

	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();	
	if (m_pTexture) m_pTexture->Release();
	if (m_pSampler) m_pSampler->Release();
	if (m_pAlphaBlendEnable) m_pAlphaBlendEnable->Release();
	if (m_pAlphaBlendDisable) m_pAlphaBlendDisable->Release();
	if (rastStateCullNone) rastStateCullNone->Release();
	if (rastStateCullBack) rastStateCullBack->Release();
}

/// <param name="filename"></param>
/// <returns></returns>
HRESULT Model::LoadObjModel(char* filename)
{
	m_pObject = new ObjFileModel(filename, m_pD3DDevice, m_pImmediateContext);
	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);

	if (FAILED(hr)) return hr;//Resturn an error code if failed

		//Create constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = sizeof(MODEL_CONSTANT_BUFFER);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;//Resturn an error code if failed

	//Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	ID3DBlob* VS, * PS, * error;
	//Load and compile the pixel and vertex Model shaders - use vs_5_0 to target DX11 hardware only

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	if (error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) return hr;//Don't fail if error is just a warning
	}

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);
	if (error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) return hr;//Don't fail if error is just a warning
	}

	//Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);
	if (FAILED(hr)) return hr;

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);
	if (FAILED(hr)) return hr;

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr)) return hr;

	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();
}

/// <param name="filename"></param>
/// <returns></returns>
HRESULT Model::AddTexture(char* filename)
{
	HRESULT hr = S_OK;

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice,
		filename, NULL, NULL,
		&m_pTexture, NULL);
	if (!m_pTexture) return S_FALSE;

		return hr;
}

/// <param name="view"></param>
/// <param name="projection"></param>
void Model::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	m_directional_light_shines_from = XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f);
	m_directional_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);//green
	m_ambient_light_colour = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);//dark grey - always use a small value for ambient lighting
	
	XMMATRIX world, transpose, DirLightRotate;
	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_x, m_y, m_z);
	
	MODEL_CONSTANT_BUFFER model_cb_value;
	model_cb_value.WorldViewProjection = world * (*view) * (*projection);

	transpose = XMMatrixTranspose(world);
	
	

	model_cb_value.directional_light_colour = m_directional_light_colour;
	model_cb_value.ambient_light_colour = m_ambient_light_colour;
	model_cb_value.directional_light_vector = XMVector3Transform(m_directional_light_shines_from, transpose);
	model_cb_value.directional_light_vector = XMVector3Normalize(model_cb_value.directional_light_vector);
	//model_cb_value.directional_light_vector = XMVector3Transform(XMVector3Transform(m_directional_light_shines_from, DirLightRotate), transpose);
	
	
	
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_value, 0, 0);

	//Set shaders Active
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	//for texture
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pObject->Draw();	

}

/// <param name="x"></param>
/// <param name="z"></param>
void Model::LookAt_XZ(float x, float z)
{

	m_yAngle = atan2(x - m_x, z - m_z) * (180.0 / XM_PI);

}

void Model::MovedFoward(float distance)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * distance;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * distance;
}

/// <summary>
/// get Collider position
/// </summary>
/// <returns></returns>
void Model::CalculateModelCentrePoint()
{
	float max_x = 0, max_y = 0, max_z = 0;
	float min_x = 0, min_y = 0, min_z = 0;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
		if (m_pObject->vertices[i].Pos.x > max_x)
			max_x = m_pObject->vertices[i].Pos.x;
		else if (m_pObject->vertices[i].Pos.x < max_x)
			min_x = m_pObject->vertices[i].Pos.x;

		if (m_pObject->vertices[i].Pos.y > max_y)
			max_y = m_pObject->vertices[i].Pos.y;
		else if (m_pObject->vertices[i].Pos.y < max_y)
			min_y = m_pObject->vertices[i].Pos.y;

		if (m_pObject->vertices[i].Pos.z > max_z)
			max_z = m_pObject->vertices[i].Pos.z;
		else if (m_pObject->vertices[i].Pos.z < max_z)
			min_z = m_pObject->vertices[i].Pos.z;
	}

	m_bounding_sphere_centre_x = (max_x + min_x) / 2;
	m_bounding_sphere_centre_y = (max_y + min_y) / 2;
	m_bounding_sphere_centre_z = (max_z + min_z) / 2;

}

/// <summary>
/// Get Collider area of effect
/// </summary>
void Model::CalculateBoundingSphereRadius()
{
	float distance = 0;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
	
		if (distance <	pow(m_bounding_sphere_centre_x - m_pObject->vertices[i].Pos.x, 2) +
						pow(m_bounding_sphere_centre_y - m_pObject->vertices[i].Pos.y, 2) +
						pow(m_bounding_sphere_centre_z - m_pObject->vertices[i].Pos.z, 2))
			distance = (pow(m_bounding_sphere_centre_x - m_pObject->vertices[i].Pos.x, 2) + pow(m_bounding_sphere_centre_y - m_pObject->vertices[i].Pos.y, 2) + pow(m_bounding_sphere_centre_z - m_pObject->vertices[i].Pos.z, 2));
	}		 

	m_bounding_sphere_radius = sqrtf(distance);

}


XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;
	world  = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;
	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0);
	offset = XMVector3Transform(offset, world);

	return offset;
}

/// <summary>
/// Check the collision in a shpere area
/// </summary>
/// <param name="model"></param>
/// <returns></returns>
bool Model::CheckCollision(Model* model)
{
	if (model == this) return false;

	float x1 = XMVectorGetX(GetBoundingSphereWorldSpacePosition());
	float y1 = XMVectorGetY(GetBoundingSphereWorldSpacePosition());
	float z1 = XMVectorGetZ(GetBoundingSphereWorldSpacePosition());

	float x2 = XMVectorGetX(model->GetBoundingSphereWorldSpacePosition());
	float y2 = XMVectorGetY(model->GetBoundingSphereWorldSpacePosition());
	float z2 = XMVectorGetZ(model->GetBoundingSphereWorldSpacePosition());

	float distance = GetBoundingSphereRadius() + model->GetBoundingSphereRadius();
	if (distance > sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)))
		return true;
	else
		return false;
}

/// <summary>
/// Check the collision in a XY 2D circle
/// </summary>
/// <param name="model"></param>
/// <returns></returns>
bool Model::CheckCollisionXY(Model* model)
{
	if (model == this) return false;


	float x1 = XMVectorGetX(GetBoundingSphereWorldSpacePosition());
	float y1 = XMVectorGetY(GetBoundingSphereWorldSpacePosition());
	float z1 = XMVectorGetZ(GetBoundingSphereWorldSpacePosition());

	float x2 = XMVectorGetX(model->GetBoundingSphereWorldSpacePosition());
	float y2 = XMVectorGetY(model->GetBoundingSphereWorldSpacePosition());
	float z2 = XMVectorGetZ(model->GetBoundingSphereWorldSpacePosition());
	

	float distance = GetBoundingSphereRadius() /*+ model->GetBoundingSphereRadius()*/;
	if (distance > sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)) && distance > sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)))
		return true;
	else
		return false;
}


