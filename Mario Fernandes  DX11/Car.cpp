#include "Car.h"

/// <summary>
/// Initialise car
/// </summary>
/// <param name="camera"></param>
/// <param name="model"></param>
/// <param name="time"></param>
/// <param name="input"></param>
/// <param name="particle"></param>
/// <param name="Bparticle"></param>
Car::Car(camera* camera, Model* model, Timer* time, Input* input, ParticleGenerator* particle, ParticleGenerator* Bparticle)
{
	m_camera = camera;
	m_model = model;
	m_input = input;
	m_time = time;
	m_particle = particle;
	m_Bparticle = Bparticle;
	if (m_camera == NULL || m_model == NULL || m_input == NULL) 	DXTRACE_MSG("Failed to create Car");

	m_camera->SetPosition(0, 0, -10);
	//m_particle->SetZPos(-5);
	m_Physics = new CarPhysics(time, 10,10);
	
	
}

Car::~Car()
{
}

void Car::Update()
{	
	
	if (m_input->IsKeyPressed(DIK_W)) m_Physics->Accelerate();
	if (m_input->IsKeyPressed(DIK_S)) m_Physics->Brake();
	if (m_input->IsKeyPressed(DIK_A)) m_Physics->Rotate(-15);
	if (m_input->IsKeyPressed(DIK_D)) m_Physics->Rotate(15);	
	if (m_input->IsKeyPressed(DIK_LSHIFT)) {
		m_Physics->Boost(extraSpeed); boosting = true;
	}

	Vector3 tempPos = m_Physics->GetPosition();

	m_Physics->Update();		
	frametime += m_time->GetDeltaTime();
	//
	//if (m_Track->CheckCollision(m_model)) {
	//	m_Physics->SetPos(tempPos);
	//	m_Physics->StopCar();
	//	frametime = 0;
	//}

	Vector3 partpos = m_Physics->GetPosition() - m_Physics->GetDirection() * 7;

	m_particle->SetPos(partpos);
	m_Bparticle->SetPos(partpos);
	
	m_model->LookAt_XZ(m_Physics->GetPosition().x + m_Physics->GetDirection().x, m_Physics->GetPosition().z + m_Physics->GetDirection().z);

	m_model->SetPos(m_Physics->GetPosition());
}

/// <summary>
/// Draw car
/// </summary>
/// <param name="view"></param>
/// <param name="projection"></param>
void Car::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	
	
	Vector3 camerapos = m_Physics->GetPosition() - m_Physics->GetDirection() * m_CameraDistance;	

	camerapos.y = m_CameraHight;

	//Update camera position
	m_camera->SetPosition(camerapos.x, camerapos.y, camerapos.z);
	
	//Update camera rotation
	if(m_LookCar)
	m_camera->LookAt_XYZ(m_Physics->GetPosition().x,m_Physics->GetPosition().y + m_OffSetY, m_Physics->GetPosition().z);
	else
	m_camera->LookAt_XZ(m_Physics->GetPosition().x, m_Physics->GetPosition().z );
	
	*view = m_camera->GetViewMatrix();

	

	m_model->Draw(view, projection);

	m_particle->Draw(view, projection, XMFLOAT3(m_camera->GetX(), m_camera->GetY(), m_camera->GetZ()));

	//Draw boost particles
	if (boosting)
	{
		m_Bparticle->Draw(view, projection, XMFLOAT3(m_camera->GetX(), m_camera->GetY(), m_camera->GetZ()));
		boosting = !boosting;
	}
}
