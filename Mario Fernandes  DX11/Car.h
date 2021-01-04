#pragma once
#include "CarPhysics.h"
#include "camera.h"
#include "TrackModel.h"
#include "Input.h"
#include "ParticleGenerator.h"

class Car
{
public:

	Car(camera* camera, Model* model, Timer* time, Input* input, ParticleGenerator* particle, ParticleGenerator* Bparticle);
	~Car();

	void Update();
	void Draw(XMMATRIX* view, XMMATRIX* projection);
	CarPhysics* m_Physics;

private:

	float m_CameraDistance = 20;
	float m_CameraHight = 5;
	bool m_LookCar = true;
	float m_OffSetY = 3;
	float frametime = 0;
	float extraSpeed = 0.5;
	bool boosting = false;

	Input* m_input;

	camera* m_camera;
	Model* m_model;
	
	Timer* m_time;


	ParticleGenerator* m_particle;
	ParticleGenerator* m_Bparticle;
};

