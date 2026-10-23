#include "Particle.h"
#include <cmath>
#include <algorithm>

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, float m, float d, Vector4 color, physx::PxSphereGeometry shape) 
	: 
	mass(m), 
	vel(vel),
	acc(acc),
	damping(std::clamp(d, 0.0f, 1.0f)),
	renderItem(nullptr)
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
	// Euler semi-implicito
	vel += acc * static_cast<float>(t);
	vel *= std::powf(damping, static_cast<float>(t));
	transform.p = transform.p + vel * static_cast<float>(t);
	
}