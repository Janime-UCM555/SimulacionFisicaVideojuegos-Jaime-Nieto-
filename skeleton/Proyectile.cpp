#include "Proyectile.h"
#include <stdexcept>
#include <iostream>
Proyectile::Proyectile(Vector3D pos, Vector3D vel, Vector3D acc, float mass, float damping, Vector4 color, physx::PxSphereGeometry shape)
	: Particle(pos, vel, acc, mass, damping, color, shape)
	, simGravity(GRAVITY)
	, realMass(mass)
	, realVelocity(vel)
{
	Particle::acc += simGravity;
}

//void Proyectile::ScaleMass(float newMass)
//{
//	if (newMass <= 0.0f) {
//		throw std::invalid_argument("Mass must be greater than zero.");
//	}
//
//	// E = 1/2 m v^2 : E1 = E2 : 1/2 m1 v1^2 = 1/2 m2 v2^2 => m1 v1^2 = m2 v2^2 => v2 = sqrt(m1/m2) * v1 : m2 = (m1 * (v1)^2) / (v2)^2;
//	//vel = realVelocity * sqrt(realMass / newMass);
//
//	acc -= simGravity;
//
//	simGravity = GRAVITY * (realMass / newMass);
//	
//	acc += simGravity;
//
//	mass = newMass;
//
//	std::cout << "Masa escalada a: " << newMass << " Con velocidad: " << vel << " Con gravedad: " << simGravity << std::endl;
//}

void Proyectile::ScaleVelocity(Vector3D newVelocity)
{
	if (newVelocity.magnitude() <= 0.0f || vel.magnitude() <= 0.0f || realVelocity.magnitude() <= 0.0f) {
		throw std::invalid_argument("Velocity must be greater than zero.");
	}

	vel = newVelocity;

	// m2 = (m1 * (v1)^2) / (v2)^2
	mass = (realMass * realVelocity.magnitude() * realVelocity.magnitude()) / (newVelocity.magnitude() * newVelocity.magnitude());

	acc -= simGravity;

	// g / = G  => g = G * (v2^2) / (v1^2)
	simGravity = GRAVITY * (newVelocity.magnitude() * newVelocity.magnitude()) / ((realVelocity.magnitude() * realVelocity.magnitude()));

	acc += simGravity;

	std::cout << "Velocidad real escalada: " << newVelocity << " Con masa simulada: " << mass << " Con masa real: " << realMass << " Con gravedad: " << simGravity << std::endl;
}