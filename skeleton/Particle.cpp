#include "Particle.h"
Particle::Particle(Vector3D pos, Vector3D vel, Vector4 color, physx::PxSphereGeometry shape) : vel(vel)
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
	transform.p += vel * static_cast<float>(t);
}