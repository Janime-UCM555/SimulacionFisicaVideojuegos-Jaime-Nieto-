#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
static constexpr Vector3D GRAVITY {0.0f, -9.8f, 0.0f};
class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D acc = Vector3D (0,0,0), float mass = 1.0f, float damping = 0.99f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), physx::PxSphereGeometry shape = physx::PxSphereGeometry(2.0f));
	virtual ~Particle();
	Vector3D getPosition() const { return Vector3D(transform.p); }
	Vector3D getVelocity() const { return vel; }

	void integrate(double t);

protected:
	float mass;
	Vector3D vel;
	Vector3D acc;
	physx::PxTransform transform;

private:
	float damping;
	RenderItem* renderItem;
};