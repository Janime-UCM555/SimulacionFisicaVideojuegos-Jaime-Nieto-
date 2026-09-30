#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D acc = Vector3D (0,0,0), Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), physx::PxSphereGeometry shape = physx::PxSphereGeometry(2.0f));
	~Particle();

	void integrate(double t);

private:
	Vector3D vel;
	Vector3D acc;
	physx::PxTransform transform;
	RenderItem* renderItem;
};

