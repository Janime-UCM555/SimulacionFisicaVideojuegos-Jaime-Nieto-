#include "Scene1.h"
#include "Particle.h"



void Scene1::init() {
    v_particles.push_back(new Particle(
        Vector3(0, 20, 0),    // posición inicial
        Vector3(0, 10, 0),    // velocidad constante
		Vector3D(0, -9.8, 0),    // aceleración
		0.9f    // damping
    ));
}

void Scene1::update(double dt) {
	for(auto& particle : v_particles)
    particle->integrate(dt);
}