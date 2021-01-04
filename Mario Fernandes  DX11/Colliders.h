#pragma once
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

using namespace DirectX;

#include "MathMario.h"
#include "Model.h"

class SphereCollider;
class BoxCollider;
class TriangleCollider;

struct Collider{
	Vector3    position = {0,0,0};         //origin in world space
	Vector3    rotation = { 0,0,0 };          //rotation/scale component of model matrix
	Vector3    scale;
};

class SphereCollider : Collider {
public:

	float radius = 0;

	SphereCollider() {};
	SphereCollider(float i_radius, Vector3 i_position) {
		radius = i_radius;
		position = i_position;
	};

	bool CheckCollision(SphereCollider* collision)
	{
		if (collision == this) return false;

		float distance = radius + collision->radius;
		if (distance > sqrt(pow(position.x - collision->position.x, 2) + pow(position.y - collision->position.y, 2) + pow(position.z - collision->position.z, 2)))
			return true;
		else
			return false;
	}

	bool CheckCollisionYZ(SphereCollider* collision)
	{
		if (collision == this) return false;

		float distance = radius + collision->radius;
		if (distance > sqrt(pow(position.y - collision->position.y, 2) + pow(position.z - collision->position.z, 2)))
			return true;
		else
			return false;
	}

};

class BoxCollider : Collider {
public:
	
	
	Vector3 min, max;

	BoxCollider() {};
	BoxCollider(Vector3 i_min, Vector3 i_max) {
		min = i_min;
		max = i_max;
	};

	//bool CeckCollision(SphereCollider* collision)
	//{
	//	if (collision == this) return false;

	//	float distance = radius + collision->radius;
	//	if (distance > sqrt(pow(position.x - collision->position.x, 2) + pow(position.y - collision->position.y, 2) + pow(position.z - collision->position.z, 2)))
	//		return true;
	//	else
	//		return false;
	//}

	bool CeckCollision(Model* collision)
	{
		float x1 = XMVectorGetX(collision->GetBoundingSphereWorldSpacePosition());
		float y1 = XMVectorGetY(collision->GetBoundingSphereWorldSpacePosition());
		float z1 = XMVectorGetZ(collision->GetBoundingSphereWorldSpacePosition());

		float distance = collision->GetBoundingSphereRadius();


	}

};
