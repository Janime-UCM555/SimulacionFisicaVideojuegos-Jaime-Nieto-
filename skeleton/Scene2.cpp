#include "Particle.h"
#include "Scene2.h"
#include "SistemaParticulas.h"

void Scene2::init() {

	// 1) FUENTE: punto casi fijo, velocidad normal centrada hacia arriba
	auto* fuente = new SistemaParticulas();
	fuente->addGenerador(std::make_unique<GeneradorNormal>(
		ParticleDesc()
		.position({ 0, 0, 0 })
		.velocity({ 0, 25, 0 })
		.acceleration(GRAVITY)
		.lifeTime(4.0)
		.color({ 0.2f, 0.5f, 1.0f, 1.0f })
		.shape(physx::PxSphereGeometry(0.25f)),
		120.0f,                            // partículas por segundo
		Vector3D(0.1f, 0.0f, 0.1f),        // posición casi sin variabilidad
		Vector3D(2.5f, 2.0f, 2.5f)));      // sigma de la velocidad
	v_objects.push_back(fuente);

	// 2) MANGUERA: chorro casi horizontal con poca dispersión, velocidad uniforme
	auto* manguera = new SistemaParticulas();
	manguera->addGenerador(std::make_unique<GeneradorUniforme>(
		ParticleDesc()
		.position({ -40, 5, 0 })
		.velocity({ 30, 12, 0 })
		.acceleration(GRAVITY)
		.lifeTime(5.0)
		.color({ 0.9f, 0.2f, 0.2f, 1.0f })
		.shape(physx::PxSphereGeometry(0.2f)),
		150.0f,
		Vector3D(0, 0, 0),                 // punto fijo
		Vector3D(1.0f, 1.0f, 1.0f)));      // velocidad entre (v-1, v+1) por componente
	v_objects.push_back(manguera);

	// 3) NIEBLA: posiciones muy variables, velocidades pequeñas, partículas
	//    grandes en número pero transparentes. Sin gravedad (ver nota).
	auto* niebla = new SistemaParticulas();
	niebla->addGenerador(std::make_unique<GeneradorNormal>(
		ParticleDesc()
		.position({ 0, 3, -30 })
		.velocity({ 0, 0.2f, 0 })
		.lifeTime(8.0)
		.damping(0.9f)
		.color({ 0.85f, 0.85f, 0.9f, 0.15f })
		.shape(physx::PxSphereGeometry(0.6f)),
		60.0f,
		Vector3D(15.0f, 2.0f, 8.0f),       // posición muy variable
		Vector3D(0.5f, 0.2f, 0.5f)));      // velocidad pequeña
	v_objects.push_back(niebla);
}

void Scene2::update(double dt) {
	for (auto& obj : v_objects) {
		obj->update(dt);
	}
}