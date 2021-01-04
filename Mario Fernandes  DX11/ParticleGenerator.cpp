#include "ParticleGenerator.h"


struct PARTICLE_CONSTANT_BUFFER
{
	XMMATRIX	WorldViewProjection; //64 bytes (4 x 4 = 16 floats x 4 bytes)
	XMFLOAT4	Color; //64 bytes
};// TOTAL SIZE = 64 bytes

ParticleGenerator::ParticleGenerator(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{

	srand(time(NULL));

	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;
	m_x = m_y = m_z = 0.0f;
	m_xAngle = m_yAngle = m_zAngle = 0.0f;
	m_scale = 1;
	count = 100;

	for (int i = 0; i < count; i++)
	{
		m_free.push_back(new Particle());
	}

	m_timePrevious = float(timeGetTime());

	m_untilParticle = -30000000000;	

}

ParticleGenerator::~ParticleGenerator()
{

	while (m_free.size() > 0)
	{
		delete m_free.front();
		m_free.pop_front();
	}

	while (m_active.size() > 0)
	{
		delete m_active.front();
		m_active.pop_front();
	}

	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pTexture) m_pTexture->Release();
	if (m_pSampler) m_pSampler->Release();
}

HRESULT ParticleGenerator::CreateParticle(ParticleType inp_type)
{	
	type = inp_type;
	HRESULT hr = S_OK;
	
	XMFLOAT3 vertices[6] =//verts for the quad NOTE: could be changed to make different shapes??
	{
		XMFLOAT3(-1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,1.0f,0.0f),
		XMFLOAT3(-1.0f,1.0f,0.0f),
		XMFLOAT3(-1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,1.0f,0.0f),
	};
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;//sets no culling
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterSolid);
	
	rasterizer_desc.CullMode = D3D11_CULL_BACK;//sets backface culling
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterParticle);
	
	//create the vert buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//both cpu and gpu
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * 6/*VERTCOUNT*/;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//edit on the cpu

	hr = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);

	if (FAILED(hr)) return 0;

	//copy verts to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//mapping = locking the buffer which allows writing
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));//copy data
	//unlock the buffer
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	//Create constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = sizeof(PARTICLE_CONSTANT_BUFFER);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;//Resturn an error code if failed

	/*CalcModelCentrePoint();
	CalcBoundingSphereRadius();*/
	//CompileShaders();

	hr = LoadShaders();
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT ParticleGenerator::AddTexture(char* filename)
{
	HRESULT hr = S_OK;

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice,
		filename, NULL, NULL,
		&m_pTexture, NULL);
	if (!m_pTexture) return S_FALSE;

		return hr;
}


void ParticleGenerator::Draw(XMMATRIX* view, XMMATRIX* projection, XMFLOAT3 cameraposition)
{
	//local vars
	XMMATRIX world;
	PARTICLE_CONSTANT_BUFFER pcb;
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	float timeNow = float(timeGetTime()) / 1000.0f;
	float deltaTime = timeNow - m_timePrevious;
	m_timePrevious = timeNow;
	m_untilParticle -= deltaTime;

	if (m_untilParticle <= 0.0f)
	{
		if (m_isActive)//a bool to check if the particle engine is on or off. Make a getter/setter and use it in main
		{
			it = m_free.begin();//point to the beggining of the free list
			//add a new particle to the back of m_active from the front of m_free
			if (m_free.size() != NULL)//safety check
			{
				switch (type)//the name of my enum
				{
				case RAINBOW_FOUNTAIN:
				{
					m_age = 2.0f;
					m_untilParticle = 0.008f;
					////////////////////////initialise the particle NOTE: all of this is adjustable for different effects////////////////////////
					(*it)->color = XMFLOAT4(0.f, RandomZeroToOne(), RandomZeroToOne(), 1);
					(*it)->gravity = 1.f;
					(*it)->position = XMFLOAT3(0.0f, 0.f, 0.0f);
					(*it)->velocity = XMFLOAT3(RandomNegOneToPosOne(), 4.50f, RandomNegOneToPosOne());
					////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case JETT:
				{
					m_age = 0.1f;
					m_untilParticle = 0.008f;
					////////////////////////initialise the particle NOTE: all of this is adjustable for different effects////////////////////////
					(*it)->color = XMFLOAT4(RandomZeroToOne(), 0.5f, 0, 1);
					(*it)->gravity = 0.f;
					(*it)->position = XMFLOAT3(m_x, m_y, m_z);
					(*it)->velocity = XMFLOAT3(RandomNegOneToPosOne(), 4.50f, RandomNegOneToPosOne());
					////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case BARRIER:
				{
					m_age = 3.0f;
					m_untilParticle = 0.008f;
					////////////////////////initialise the particle NOTE: all of this is adjustable for different effects////////////////////////
					(*it)->color = XMFLOAT4(0.f, RandomZeroToOne(), RandomZeroToOne(), 1);
					(*it)->gravity = 0.f;
					(*it)->position = XMFLOAT3(m_x, m_y, m_z);
					(*it)->velocity = XMFLOAT3(RandomNegOneToPosOne(), 4.50f, RandomNegOneToPosOne());
					////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				default:
				{
					break;
				}
				}
				(*it)->age = 0.0f;//set age to 0. this is used for knowing when to delete the particle

			//////add the particle from the front of the available list to the back of the active list and remove it
				m_active.push_back(*it);
				m_free.pop_front();
			}
		}
		else m_untilParticle = 0.001f;
	}

	if (m_active.size() != NULL)//safety check
	{
		it = m_active.begin();//point the iterator to the front of the active list ready for processing
		while (it != m_active.end())//move all of the particles
		{	

			switch (type)
			{
			case RAINBOW_FOUNTAIN:
			{
				/////////////////////////ALL of this is adjustable for different effects///////////////////////////////////////////////////////////
				(*it)->age += deltaTime;
				(*it)->velocity.y -= (*it)->gravity * (deltaTime);
				(*it)->position.x += (*it)->velocity.x * (deltaTime);
				(*it)->position.y += (*it)->velocity.y * (deltaTime);
				(*it)->position.z += (*it)->velocity.z * (deltaTime);
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			}
			case JETT:
			{
				/////////////////////////ALL of this is adjustable for different effects///////////////////////////////////////////////////////////
				(*it)->age += deltaTime;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			}
			case BARRIER:
			{
				/////////////////////////ALL of this is adjustable for different effects///////////////////////////////////////////////////////////
				(*it)->age += deltaTime;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			}
			default:
			{
				break;
			}
			}
			world = XMMatrixIdentity();
			switch (type)
			{
				case RAINBOW_FOUNTAIN:
				{
					/*set scale and world transforms here*/
					world = XMMatrixScaling(m_scale, m_scale, m_scale);
					world *= XMMatrixTranslation(m_x, m_y, m_z);
					break;
				}
				case JETT:
				{
					/*set scale and world transforms here*/
					world = XMMatrixScaling(m_scale, m_scale, m_scale);
					world = XMMatrixTranslation(0, 0, 0);
					break;
				}
				case BARRIER:
				{
					/*set scale and world transforms here*/
					world = XMMatrixScaling(m_scale, m_scale, m_scale);
					world = XMMatrixTranslation(0, 0, 0);
					break;
				}
				default:
				{
					break;
				}
			}

			LookAt_XZ(cameraposition.x, cameraposition.z);
			world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
			world *= XMMatrixTranslation((*it)->position.x, (*it)->position.y, (*it)->position.z);

			//constant buffer stuff for shader
			pcb.WorldViewProjection = (world) * (*view) * (*projection);
			pcb.Color = (*it)->color;

			m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
			m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &pcb, 0, 0);

			//set the shader objects as active
			m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
			m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
			m_pImmediateContext->IASetInputLayout(m_pInputLayout);

			m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_pImmediateContext->RSSetState(m_pRasterParticle);//set backface culling to on
			m_pImmediateContext->Draw(6, 0);//draw the particle
			m_pImmediateContext->RSSetState(m_pRasterSolid);//set backface culling to off
			
			if ((*it)->age >= m_age)//check the age of the current particle
			{
				it++;
				m_active.front()->age = m_age;
				m_active.front()->position = { (RandomNegOneToPosOne() + m_x * 10) * (RandomZeroToOne() * 10),m_y + 5.0f, /*position.z*/ cameraposition.z + 7.0f };
				m_active.front()->velocity = { /*RandomNegOneToPosOne()*/0.0f, 4.50f, RandomNegOneToPosOne() };
				m_free.push_back(m_active.front());//move the (now previously) current active particle to the back of the pool			
				m_active.pop_front();//remove the particle			
			}
			else it++;			
		}//end of while
	}//end of if(m_active.size()!=NULL)


	Particle test;
	test.color = XMFLOAT4(1.f, 0.f, 0.3f, 1.f);
	test.gravity = 1;
	test.position = XMFLOAT3(0.f, 0, 0);
	test.velocity = XMFLOAT3(0.f, 0.f, 0.f);
	
	DrawOne(&test, view, projection, cameraposition);
}

void ParticleGenerator::DrawOne(Particle* one, XMMATRIX* view, XMMATRIX* projection, XMFLOAT3 cameraposition)
{
	UINT stride = sizeof(XMFLOAT3);

	XMMATRIX world;
	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	LookAt_XZ(cameraposition.x, cameraposition.z);
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	PARTICLE_CONSTANT_BUFFER particle_cb_value;
	particle_cb_value.WorldViewProjection = world * (*view) * (*projection);
	particle_cb_value.Color = one->color;

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &particle_cb_value, 0, 0);

	//Set shaders Active
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->Draw(sizeof(PARTICLE_CONSTANT_BUFFER), 0);
}

void ParticleGenerator::LookAt_XZ(float x, float z)
{

	m_yAngle = atan2(x - m_x, z - m_z) * (180.0 / XM_PI);

}

void ParticleGenerator::MovedFoward(float distance)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * distance;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * distance;
}

float ParticleGenerator::RandomZeroToOne()
{
	return ((float)rand() / (RAND_MAX));
}

float ParticleGenerator::RandomNegOneToPosOne()
{
	return (((float)(rand()) / ((RAND_MAX) / 2)) -1);
}

void ParticleGenerator::CalculateModelCentrePoint()
{
	//float max_x = 0, max_y = 0, max_z = 0;
	//float min_x = 0, min_y = 0, min_z = 0;

	//for (int i = 0; i < m_pObject->numverts; i++)
	//{
	//	if (m_pObject->vertices[i].Pos.x > max_x)
	//		max_x = m_pObject->vertices[i].Pos.x;
	//	else if (m_pObject->vertices[i].Pos.x < max_x)
	//		min_x = m_pObject->vertices[i].Pos.x;

	//	if (m_pObject->vertices[i].Pos.y > max_y)
	//		max_y = m_pObject->vertices[i].Pos.y;
	//	else if (m_pObject->vertices[i].Pos.y < max_y)
	//		min_y = m_pObject->vertices[i].Pos.y;

	//	if (m_pObject->vertices[i].Pos.z > max_z)
	//		max_z = m_pObject->vertices[i].Pos.z;
	//	else if (m_pObject->vertices[i].Pos.z < max_z)
	//		min_z = m_pObject->vertices[i].Pos.z;
	//}

	//m_bounding_sphere_centre_x = (max_x + min_x) / 2;
	//m_bounding_sphere_centre_y = (max_y + min_y) / 2;
	//m_bounding_sphere_centre_z = (max_z + min_z) / 2;

}

void ParticleGenerator::CalculateBoundingSphereRadius()
{
	/*float distance = 0;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
	
		if (distance <	pow(m_bounding_sphere_centre_x - m_pObject->vertices[i].Pos.x, 2) +
						pow(m_bounding_sphere_centre_y - m_pObject->vertices[i].Pos.y, 2) +
						pow(m_bounding_sphere_centre_z - m_pObject->vertices[i].Pos.z, 2))
			distance = (pow(m_bounding_sphere_centre_x - m_pObject->vertices[i].Pos.x, 2) + pow(m_bounding_sphere_centre_y - m_pObject->vertices[i].Pos.y, 2) + pow(m_bounding_sphere_centre_z - m_pObject->vertices[i].Pos.z, 2));
	}		 

	m_bounding_sphere_radius = sqrt(distance);*/
}

HRESULT ParticleGenerator::LoadShaders()
{
	HRESULT hr = S_OK;

	ID3DBlob* VS, * PS, * error;
	//Load and compile the pixel and vertex Model shaders - use vs_5_0 to target DX11 hardware only

	hr = D3DX11CompileFromFile("particle_shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	if (error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) return hr;//Don't fail if error is just a warning
	}

	hr = D3DX11CompileFromFile("particle_shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);
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

	return hr;
}

XMVECTOR ParticleGenerator::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;
	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0);
	offset = XMVector3Transform(offset, world);

	return offset;
}

bool ParticleGenerator::CheckCollision(ParticleGenerator* model)
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


