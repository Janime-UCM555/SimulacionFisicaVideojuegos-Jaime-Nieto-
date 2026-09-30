#include "Particle.h"
#include <cmath>
Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, Vector4 color, physx::PxSphereGeometry shape) : vel(vel), acc(acc), renderItem(nullptr)
{
	physx::PxShape* s = CreateShape(shape);
	transform = physx::PxTransform(pos.x, pos.y, pos.z);
	renderItem = new RenderItem(s, &transform, color);
}

Particle::~Particle()
{
	if (renderItem)
	{
		renderItem->release();
		renderItem = nullptr;
	}
}

void Particle::integrate(double t)
{
	if(!renderItem){ return; }
	vel += acc * static_cast<float>(t);
	transform.p = transform.p + vel * static_cast<float>(t);
	// + (acc * (1/2) * std::pow(static_cast<float>(t),2));
}