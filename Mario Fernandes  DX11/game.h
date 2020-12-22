#pragma once
#include <dinput.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

using namespace DirectX;

#include <list>

#include "timer.h"
#include "camera.h"
#include "Text2D.h"
#include "Model.h"
#include "skyBox.h"
#include "reflectiveModel.h"
#include "ParticleGenerator.h"
#include "Input.h"

class Game
{
public:
	
	HRESULT Initialise(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, Input* Input);
	void Update();
	void Draw();


	~Game();

public:
	Timer					m_Timer;
	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	Input*					m_input;

	camera* camera1;
	Text2D* g_2Dtext;
	Model* model0;
	reflectiveModel* model1;
	skyBox* SkyBox;
	ParticleGenerator* particle;

	
	void ReadInputStates();
};

