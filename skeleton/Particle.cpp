#include "GeneradorFuerzas.h"
#include "Particle.h"
#include <algorithm>
#include <cmath>

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
	transform = physx::PxTransform(d.pos_.x, d.pos_.y, d.pos_.z);

	if (d.visible_) {
		physx::PxShape* s = CreateShape(d.shape_.any());
		renderItem = new RenderItem(s, &transform, d.color_);
	}
}


Particle::Particle(Vector3D pos, Vector3D vel, Vector3D acc, float m, float d, Vector4 color, physx::PxSphereGeometry shape)
	: Particle(ParticleDesc().position(pos).velocity(vel).acceleration(acc).mass(m).damping(d).color(color).shape(shape))
{}

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
	d.visible(true); // Forzar visibilidad de la copia, aunque la original esté oculta
	return new Particle(d);
}


void Particle::update(double dt)
{
	if (!renderItem) { return; }

	age += dt;

	force = Vector3D(0, 0, 0);

	for (auto& f : fuerzasPropias)
	{
		if (f && f->isActive()) {
			force += f->getForce(*this);
		}
	}

	if (fuerzasSistema)
	{
		for (auto& f : *fuerzasSistema)
		{
			if (f && f->isActive()) {
				force += f->getForce(*this);
			}
		}
	}

	integrate(dt);
}

void Particle::integrate(double t)
{
	float dt = static_cast<float>(t);
	const float invMass = (mass > 0.0f) ? (1.0f / mass) : 0.0f;
	// Euler semi-implicito
	vel += (acc + force * invMass) * dt;
	vel *= std::powf(damping, dt);
	transform.p = transform.p + vel * dt;
}

void Particle::removeForce(GeneradorFuerzas* f) {
	auto it = std::find(fuerzasPropias.begin(), fuerzasPropias.end(), f);
	if (it != fuerzasPropias.end()) {
		fuerzasPropias.erase(it);
	}
}

void Particle::bindForces(std::vector<GeneradorFuerzas*>* fuerzas) {
	fuerzasSistema = fuerzas;
}