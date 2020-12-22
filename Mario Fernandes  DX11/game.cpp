#include "game.h"

HRESULT Game::Initialise(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, Input* Input)
{
	//m_input = Input;
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	if (NULL == m_pD3DDevice  || NULL == m_pImmediateContext || NULL == m_input)
		return E_FAIL;

	HRESULT hr = S_OK;

	g_2Dtext = new Text2D("assets/font1.png", m_pD3DDevice, m_pImmediateContext);

	SkyBox = new skyBox(m_pD3DDevice, m_pImmediateContext);

	hr = SkyBox->AddTexture((char*)"assets/skybox02.dds");
	if (FAILED(hr)) return hr;

	camera1 = new camera(0, 0, -15, 0);

	model0 = new Model(m_pD3DDevice, m_pImmediateContext);

	hr = model0->LoadObjModel((char*)"assets/cube.obj");
	if (FAILED(hr)) return hr;

	hr = model0->AddTexture((char*)"assets/BoxTexture.bmp");
	if (FAILED(hr)) return hr;


	model1 = new reflectiveModel(m_pD3DDevice, m_pImmediateContext);

	hr = model1->LoadObjModel((char*)"assets/Sphere.obj");
	if (FAILED(hr)) return hr;

	hr = model1->AddTexture((char*)"assets/skybox02.dds");
	if (FAILED(hr)) return hr;

	particle = new ParticleGenerator(m_pD3DDevice, m_pImmediateContext);

	hr = particle->CreateParticle();
	if (FAILED(hr)) return hr;
	
	//particle->ModXPos(10);

	return hr;    
}

void Game::Update()
{
	m_input->Update();
}

void Game::Draw()
{
	XMMATRIX projection, world, view;


	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), 640.0 / 480.0, 1.0, 100.0);
	view = camera1->GetViewMatrix();

	model0->SetZPos(10.0f);

	model0->LookAt_XZ(camera1->GetX(), camera1->GetZ());

	model0->MovedFoward(0.001f);

	//g_pImmediateContext->OMSetBlendState(g_pAlphaBlendEnable, 0, 0xffffffff);
	SkyBox->Follow(camera1->GetX(), camera1->GetY(), camera1->GetZ());

	SkyBox->Draw(&view, &projection);

	//model0->Draw(&view, &projection);

	//model1->Draw(&view, &projection);


	particle->Draw(&view, &projection, XMFLOAT3(camera1->GetX(), camera1->GetY(), camera1->GetZ()));


	g_2Dtext->RenderText();

	//if (model0->CheckCollision(model1))
	//positions are from -1.0 for x and y, represens topf left of string on screen
	//size is fraction of screen size
	g_2Dtext->AddText("Colliding", -1.0, +1.0, .05);
}

Game::~Game()
{
	if (camera1)
	{
		delete camera1;
		camera1 = nullptr;
	}

	if (model0)
	{
		delete model0;
		model0 = nullptr;
	}

	if (model1)
	{
		delete model1;
		model1 = nullptr;
	}

	if (g_2Dtext)
	{
		delete g_2Dtext;
		g_2Dtext = nullptr;
	}
}

void Game::ReadInputStates()
{
#pragma region KeyInput
	
	
	if (m_input->IsKeyPressed(DIK_U)) camera1->Up(1);
	if (m_input->IsKeyPressed(DIK_W)) camera1->Forward(0.01);
	if (m_input->IsKeyPressed(DIK_S)) camera1->Forward(-0.01);
	if (m_input->IsKeyPressed(DIK_A)) camera1->Starf(-0.01);
	if (m_input->IsKeyPressed(DIK_D)) camera1->Starf(0.01);
	if (m_input->IsKeyPressed(DIK_Q)) camera1->Rotate(-0.01);
	if (m_input->IsKeyPressed(DIK_E)) camera1->Rotate(0.01);

#pragma endregion
}
