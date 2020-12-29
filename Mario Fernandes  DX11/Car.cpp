#include "Car.h"

Car::Car(camera* camera, Model* model, Timer* time, Input* input, TrackModel* Track)
{
	m_camera = camera;
	m_model = model;
	m_input = input;
	m_time = time;
	m_Track = Track;
	if (m_camera == NULL || m_model == NULL || m_input == NULL) 	DXTRACE_MSG("Failed to create Car");

	m_camera->SetPosition(0, 0, -10);

	m_Physics = new CarPhysics(time, 5,5);
	m_Physics->SetPos(XMVectorSet(0,0,-60,0));
}

Car::~Car()
{
}

void Car::Update()
{	
	
	if (m_input->IsKeyPressed(DIK_W)) m_Physics->Accelerate();
	if (m_input->IsKeyPressed(DIK_S)) m_Physics->Brake();
	if (m_input->IsKeyPressed(DIK_A)) m_Physics->Rotate(-8);
	if (m_input->IsKeyPressed(DIK_D)) m_Physics->Rotate(8);


	XMVECTOR tempPos = m_Physics->GetPos();

	m_Physics->Update();		
	frametime += m_time->GetDeltaTime();
	
	if (m_Track->CheckCollision(m_model)) {
		m_Physics->SetPos(tempPos);
		m_Physics->StopCar();
		frametime = 0;
	}
	
	m_model->LookAt_XZ(XMVectorGetX(m_Physics->GetPosition()) + XMVectorGetX(m_Physics->GetDirection()), XMVectorGetZ(m_Physics->GetPosition()) + XMVectorGetZ(m_Physics->GetDirection()));
	m_model->SetPos(m_Physics->GetPosition());

	//m_model->SetXPos(XMVectorGetX(m_Physics->GetPosition()));
	//m_model->SetYPos(XMVectorGetY(m_Physics->GetPosition()));
	//m_model->SetZPos(XMVectorGetZ(m_Physics->GetPosition()));	
	
	//m_model->SetXPos(m_model->GetXPos() + 10 * m_time->GetDeltaTime());
}

void Car::Draw(XMMATRIX* view, XMMATRIX* projection)
{

	
	XMVECTOR camerapos = m_Physics->GetPosition() - m_Physics->GetDirection() * m_CameraDistance;

	camerapos = XMVectorSetY(camerapos, m_CameraHight);

	m_camera->SetPosition(XMVectorGetX(camerapos), XMVectorGetY(camerapos), XMVectorGetZ(camerapos));
	
	if(m_LookCar)
	m_camera->LookAt_XYZ(XMVectorGetX(m_Physics->GetPosition()), XMVectorGetY(m_Physics->GetPosition()) + m_OffSetY, XMVectorGetZ(m_Physics->GetPosition()));
	else
	m_camera->LookAt_XZ(XMVectorGetX(m_Physics->GetPosition()), XMVectorGetZ(m_Physics->GetPosition()));
	
	*view = m_camera->GetViewMatrix();

	m_model->Draw(view, projection);
}
