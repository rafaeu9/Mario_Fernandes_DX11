#include "game.h"

HRESULT Game::Initialise(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, Input* Input)
{
	m_input = Input;
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	if (NULL == m_pD3DDevice  || NULL == m_pImmediateContext || NULL == m_input)
		return E_FAIL;

	HRESULT hr = S_OK;

	g_2Dtext = new Text2D("assets/font1.png", m_pD3DDevice, m_pImmediateContext);

	SkyBox = new skyBox(m_pD3DDevice, m_pImmediateContext);

	hr = SkyBox->AddTexture((char*)"assets/skybox02.dds");
	if (FAILED(hr)) return hr;

	camera1 = new camera(0, 0, 0, 0);

	model0 = new Model(m_pD3DDevice, m_pImmediateContext);

	hr = model0->LoadObjModel((char*)"assets/cube.obj");
	if (FAILED(hr)) return hr;

	hr = model0->AddTexture((char*)"assets/Body.bmp");
	if (FAILED(hr)) return hr;

	model2 = new Model(m_pD3DDevice, m_pImmediateContext);

	hr = model2->LoadObjModel((char*)"assets/cube.obj");
	if (FAILED(hr)) return hr;

	hr = model2->AddTexture((char*)"assets/Body.bmp");
	if (FAILED(hr)) return hr;

	m_track = new TrackModel(m_pD3DDevice, m_pImmediateContext);

	hr = m_track->LoadObjModel((char*)"assets/track.obj");
	if (FAILED(hr)) return hr;

	hr = m_track->AddTexture((char*)"assets/Body.bmp");
	if (FAILED(hr)) return hr;

		
	model1 = new reflectiveModel(m_pD3DDevice, m_pImmediateContext);

	hr = model1->LoadObjModel((char*)"assets/Sphere.obj");
	if (FAILED(hr)) return hr;

	hr = model1->AddTexture((char*)"assets/skybox02.dds");
	if (FAILED(hr)) return hr;

	model1->SetXPos(6);

	particle = new ParticleGenerator(m_pD3DDevice, m_pImmediateContext);

	hr = particle->CreateParticle();
	if (FAILED(hr)) return hr;
	
	//particle->ModXPos(10);

	car1 = new Car(camera1, model0, &m_Timer, m_input, m_track);

	return hr;    
}

void Game::Update()
{
	// Update the timer before doing anything else.
	m_Timer.Update();

	m_input->Update();

	ReadInputStates();

	car1->Update();
	
}

void Game::Draw()
{
	XMMATRIX projection, world, view;

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), 640.0 / 480.0, 1.0, 100.0);	

	view = camera1->GetViewMatrix();
	
	SkyBox->Follow(camera1->GetX(), camera1->GetY(), camera1->GetZ());

	SkyBox->Draw(&view, &projection);
	
	car1->Draw(&view, &projection);

	//model0->LookAt_XZ(camera1->GetX(), camera1->GetZ());
	//
	//model0->MovedFoward(3 * m_Timer.GetDeltaTime());

	//m_pImmediateContext->OMSetBlendState(g_pAlphaBlendEnable, 0, 0xffffffff);	


	//model2->ModZRot(0.1f);
	model1->ModZRot(0.1f);
	//model2->MovedFoward(0.01);

	//car1->m_Physics->Rotate(10);

	/*if(model2->CheckCollision(model0))
		car1->m_Physics->StopCar();*/


	m_track->Draw(&view, &projection);

	model1->Draw(&view, &projection);
	model2->Draw(&view, &projection);

	particle->Draw(&view, &projection, XMFLOAT3(camera1->GetX(), camera1->GetY(), camera1->GetZ()));

	//to_string(model1->GetZPos())
	//to_string(XMVectorGetZ(car1->m_Physics->GetPosition()))

	g_2Dtext->RenderText();
	
	//positions are from -1.0 for x and y, represens topf left of string on screen
	//size is fraction of screen size	
	if (model2->CheckCollision(model0))
	g_2Dtext->AddText("Colliding", 0, +1.0, .05);
	

	g_2Dtext->AddText("FPS " + to_string(deltaFPS), -1.0, +1.0, .05);

	if (frametime < 1)
	{
		++FPS;
		frametime += m_Timer.GetDeltaTime();
	}
	else
	{
		deltaFPS = FPS;

		FPS = 1;
		frametime = 0;
	}
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

	if (m_input)
	{
		m_input = nullptr;
	}

	if (m_pD3DDevice) m_pD3DDevice->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();

}

void Game::ReadInputStates()
{
#pragma region KeyInput
	
	
	//if (m_input->IsKeyPressed(DIK_U)) camera1->Up(4 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_W)) camera1->Forward(4 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_S)) camera1->Forward(-4 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_A)) camera1->Starf(-4 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_D)) camera1->Starf(4 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_Q)) camera1->Rotate(-8 * m_Timer.GetDeltaTime());
	//if (m_input->IsKeyPressed(DIK_E)) camera1->Rotate(8 * m_Timer.GetDeltaTime());

#pragma endregion
}
