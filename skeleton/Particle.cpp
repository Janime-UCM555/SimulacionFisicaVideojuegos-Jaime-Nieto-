#include "Particle.h"
#include <cmath>
#include <algorithm>

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, float d, Vector4 color, physx::PxSphereGeometry shape) : vel(vel), acc(acc), damping(d), renderItem(nullptr)
{
	std::clamp(damping, 0.0f, 1.0f);
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
	// Euler semi-implicito
	vel += acc * static_cast<float>(t);
	vel *= std::powf(damping, static_cast<float>(t));
	transform.p = transform.p + vel * static_cast<float>(t);
	
}