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
public:
	Vector3*    position;         //origin in world space
	Vector3*    rotation;          //rotation/scale component of model matrix
	Vector3*    scale;
};

class SphereCollider : Collider {
public:

	float radius = 0;

	SphereCollider() {};
	SphereCollider(float i_radius, Vector3* i_position) {
		radius = i_radius;
		position = i_position;
	};

	bool CheckCollision(SphereCollider* collision)
	{
		if (collision == this) return false;

		float distance = radius + collision->radius;
		if (distance > sqrt(pow(position->x - collision->position->x, 2) + pow(position->y - collision->position->y, 2) + pow(position->z - collision->position->z, 2)))
			return true;
		else
			return false;
	}

	bool CheckCollisionYZ(SphereCollider* collision)
	{
		if (collision == this) return false;

		float distance = radius + collision->radius;
		if (distance > sqrt(pow(position->y - collision->position->y, 2) + pow(position->z - collision->position->z, 2)))
			return true;
		else
			return false;
	}

};

class BoxCollider : Collider {
public:
	
	
	Vector3 min, max;

	BoxCollider() {};
	BoxCollider(Vector3* i_position, Vector3 i_min, Vector3 i_max) {
		position = i_position;
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
		Vector3 collisionPos, temp;
		collisionPos.x = XMVectorGetX(collision->GetBoundingSphereWorldSpacePosition());
		collisionPos.y = XMVectorGetY(collision->GetBoundingSphereWorldSpacePosition());
		collisionPos.z = XMVectorGetZ(collision->GetBoundingSphereWorldSpacePosition());

		temp = collisionPos;

		float distance = collision->GetBoundingSphereRadius();

		temp = *position - temp;
		temp *= distance;
		temp += collisionPos;


		if (temp.x >= position->x + min.x && temp.x <= position->x + max.x &&
			temp.y >= position->y + min.y && temp.y <= position->y + max.y &&
			temp.z >= position->z + min.z && temp.z <= position->z + max.z)
			return true;
		else
			return false;


	}

};
