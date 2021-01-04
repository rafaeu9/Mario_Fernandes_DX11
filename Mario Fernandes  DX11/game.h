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
#include "Car.h"
#include "TrackModel.h"

class Game
{
public:
	
	HRESULT Initialise(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, Input* Input);
	void Update();
	void Draw();
	void Finish();
	bool Finished = false;

	~Game();

public:
	Timer					m_Timer;
	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	Input*					m_input;
	int FPS = 0, deltaFPS = 0;
	float frametime = 0;
	float youtime = 0;

	Car* car1;
	camera* camera1;
	Text2D* g_2Dtext;
	Model* model0;
	Model* model2;
	Model* model3;
	bool check = false;
	Model* Check1;
	Model* Check2;
	TrackModel* m_track;
	reflectiveModel* model1;
	skyBox* SkyBox;
	ParticleGenerator* particle;
	ParticleGenerator* carParticle;
	ParticleGenerator* boostParticle;

	reflectiveModel* Boost1;
	reflectiveModel* Boost2;
	reflectiveModel* Boost3;
	
	void ReadInputStates();
	
};

