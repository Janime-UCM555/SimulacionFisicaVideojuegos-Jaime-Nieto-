#include "Particle.h"
#include <cmath>
#include <algorithm>

Particle::Particle(const ParticleDesc& d)
	: mass(d.mass_)
	, vel(d.vel_)
	, acc(d.acc_)
	, lifeTime(d.lifeTime_)
	, age(0.0)
	, damping(std::clamp(d.damping_, 0.0f, 1.0f))
	, renderItem(nullptr)
	, desc(d)
{
	if (d.visible_) {
		physx::PxShape* s = CreateShape(d.shape_.any());
		transform = physx::PxTransform(d.pos_.x, d.pos_.y, d.pos_.z);
		renderItem = new RenderItem(s, &transform, d.color_);
	}
}


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

Particle* Particle::clone() const
{
	ParticleDesc d = desc;
	d.visible(true);
	return new Particle(d);
}


void Particle::update(double dt)
{
	if (!renderItem) { return; }
	age += dt;
	integrate(dt);
}

void Particle::integrate(double t)
{
	// Euler semi-implicito
	vel += acc * static_cast<float>(t);
	vel *= std::powf(damping, static_cast<float>(t));
	transform.p = transform.p + vel * static_cast<float>(t);
	
}