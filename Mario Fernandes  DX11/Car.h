#pragma once
#include "camera.h"
#include "TrackModel.h"
#include "CarPhysics.h"
#include "Input.h"

class Car
{
public:

	Car(camera* camera, Model* model, Timer* time, Input* input, TrackModel* Track);
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

	Input* m_input;

	camera* m_camera;
	Model* m_model;
	
	Timer* m_time;

	TrackModel* m_Track;
};

