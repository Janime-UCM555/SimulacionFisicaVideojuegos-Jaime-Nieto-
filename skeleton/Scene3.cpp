#include "Scene3.h"
#include <cctype>

namespace {
	const Vector4 ROJO(1.0f, 0.2f, 0.2f, 1.0f);
	const Vector4 VERDE(0.2f, 1.0f, 0.3f, 1.0f);
	const Vector4 AZUL(0.2f, 0.5f, 1.0f, 1.0f);
	const Vector4 AMARILLO(1.0f, 0.9f, 0.2f, 1.0f);
	const Vector4 VIOLETA(0.7f, 0.3f, 1.0f, 1.0f);
	const Vector4 CIAN(0.3f, 1.0f, 1.0f, 1.0f);

	// Sistema con un único emisor normal. Sigmas a 0 => trayectoria determinista.
	SistemaParticulas* crearEmisor(Vector3D pos, Vector3D vel, float masa, float radio, Vector4 color,
		Vector3D posSigma, Vector3D velSigma,
		Vector3D acc = Vector3D(0, 0, 0), double vida = 5.0, float rate = 15.0f)
	{
		auto* s = new SistemaParticulas();
		s->addGenerador(std::make_unique<GeneradorNormal>(
			ParticleDesc().position(pos).velocity(vel).acceleration(acc)
			.mass(masa).lifeTime(vida).color(color)
			.shape(physx::PxSphereGeometry(radio)),
			rate, posSigma, velSigma));
		return s;
	}
}

void Scene3::init() {
	// Generadores de fuerza primero: se actualizan antes que los sistemas
	gravedad = new GravityForce(Vector3D(0.0f, -10.0f, 0.0f));
	gravedadLigera = new GravityForce(Vector3D(0.0f, -2.0f, 0.0f));   // 2ª instancia: "flotabilidad"
	viento = new WindForce(Vector3D(20, 0, 0), Vector3D(40, 0, 0), Vector3D(20, 45, 20), 0.5f, 0.0f);
	torbellino = new WhirlwindForce(Vector3D(70, 0, 0), Vector3D(15, 40, 15), 0.3f, 1.0f);
	explosion = new ExplosionForce(Vector3D(90, 10, 0), 600.0f, 40.0f, 1.0f);   // ve = 0: radio fijo
	v_objects.push_back(gravedad);
	v_objects.push_back(gravedadLigera);
	v_objects.push_back(viento);
	v_objects.push_back(torbellino);
	v_objects.push_back(explosion);

	const Vector3D v0(0, 20, 0);

	// --- Actividad 1: gravedad como aceleración vs. como fuerza vs. gravedad ligera ---
	// A) CONTROL: aceleración constante, sin generadores de fuerza
	v_objects.push_back(crearEmisor(Vector3D(0, 0, 0), v0, 1.0f, 0.3f, ROJO,
		ZERO_VECTOR, ZERO_VECTOR, Vector3D(0, -10, 0)));
	// B) Misma g como GravityForce: debe coincidir con A
	auto* b = crearEmisor(Vector3D(10, 0, 0), v0, 1.0f, 0.3f, VERDE, ZERO_VECTOR, ZERO_VECTOR);
	b->addForce(gravedad);
	v_objects.push_back(b);
	// C) Segunda instancia (g = -2): sube mucho más
	auto* c = crearEmisor(Vector3D(20, 0, 0), v0, 1.0f, 0.3f, AZUL, ZERO_VECTOR, ZERO_VECTOR);
	c->addForce(gravedadLigera);
	v_objects.push_back(c);

	// --- Actividad 2: viento sobre masas distintas ---
	auto* ligera = crearEmisor(Vector3D(35, 0, 0), v0, 0.5f, 0.3f, AMARILLO, ZERO_VECTOR, Vector3D(0.5f, 0.5f, 0.5f));
	ligera->addForce(gravedad);
	ligera->addForce(viento);
	v_objects.push_back(ligera);

	auto* pesada = crearEmisor(Vector3D(45, 0, 0), v0, 10.0f, 0.6f, VIOLETA, ZERO_VECTOR, Vector3D(0.5f, 0.5f, 0.5f));
	pesada->addForce(gravedad);
	pesada->addForce(viento);
	v_objects.push_back(pesada);

	// --- Actividad 3: torbellino ---
	auto* tor = crearEmisor(Vector3D(70, 0, 0), Vector3D(0, 5, 0), 1.0f, 0.3f, CIAN,
		Vector3D(3, 0, 3), Vector3D(2, 1, 2));
	tor->addForce(torbellino);
	v_objects.push_back(tor);

	// --- Actividad 4: explosión sobre masas distintas (flotan sin gravedad) ---
	auto* zona = new SistemaParticulas();
	zona->addGenerador(std::make_unique<GeneradorUniforme>(
		ParticleDesc().position(Vector3D(90, 10, 0)).mass(1.0f).lifeTime(20.0)
		.color(ROJO).shape(physx::PxSphereGeometry(0.4f)),
		3.0f, Vector3D(8, 4, 8), Vector3D(0, 0, 0)));
	zona->addGenerador(std::make_unique<GeneradorUniforme>(
		ParticleDesc().position(Vector3D(90, 10, 0)).mass(10.0f).lifeTime(20.0)
		.color(AZUL).shape(physx::PxSphereGeometry(0.8f)),
		3.0f, Vector3D(8, 4, 8), Vector3D(0, 0, 0)));
	zona->addForce(explosion);
	v_objects.push_back(zona);
}

void Scene3::update(double dt) {
	for (auto* obj : v_objects) obj->update(dt);
}

void Scene3::keyPress(unsigned char key, const physx::PxTransform& cameraTransform) {
	switch (std::toupper(key)) {
	case 'G': gravedad->toggleActive(); break;
	case 'H': gravedadLigera->toggleActive(); break;
	case 'V': viento->toggleActive(); break;
	case 'T': torbellino->toggleActive(); break;
	case 'E': explosion->detonate(); break;
	default: break;
	}
}