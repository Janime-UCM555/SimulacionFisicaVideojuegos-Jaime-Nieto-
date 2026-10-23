#pragma once
#include "Particle.h"
class Proyectile : public Particle
{
private:
	Vector3D simGravity = GRAVITY;
	float realMass;
	Vector3D realVelocity;
public:
	Proyectile(Vector3D pos, Vector3D vel, Vector3D acc = Vector3D(0, 0, 0), float mass = 1.0f, float damping = 1.0f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), physx::PxSphereGeometry shape = physx::PxSphereGeometry(2.0f));
	~Proyectile() = default;

	//void ScaleMass(float mass);
	void ScaleVelocity(Vector3D velocity);

	/*void ScaleMassByFactor(float factor) {
		if (factor <= 0.0f) {
			throw std::invalid_argument("Factor must be greater than zero.");
		}
		float newMass = mass * factor;
		ScaleMass(newMass);
	}*/
	void ScaleVelocityByFactor(float factor) {
		if (factor <= 0.0f) {
			throw std::invalid_argument("Factor must be greater than zero.");
		}
		Vector3D newVelocity = vel * factor;
		ScaleVelocity(newVelocity);
	}

};