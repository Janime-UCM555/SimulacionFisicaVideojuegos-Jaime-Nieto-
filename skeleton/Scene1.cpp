#include "Scene1.h"
#include "Particle.h"
#include "Proyectile.h"
#include <iostream>



void Scene1::init() {
  //  v_particles.push_back(new Particle(
  //      Vector3(0, 20, 0),    // posición inicial
  //      Vector3(0, 10, 0),    // velocidad
		//GRAVITY,    // aceleración
  //      1.0f    // masa
  //  ));
	
}

void Scene1::update(double dt) {
	for(auto& particle : v_particles)
    particle->integrate(dt);
}

void Scene1::keyPress(unsigned char key, const physx::PxTransform& cameraTransform) {
	// Manejo de teclas específicas para la escena 1
	switch (key) {
	//case 'M':
	//	if (!v_particles.empty()) {
	//		static_cast<Proyectile*>(v_particles.back())->ScaleMassByFactor(2.0f);
	//	}
	//	break;
	//case 'm':
	//	if (!v_particles.empty()) {
	//		static_cast<Proyectile*>(v_particles.back())->ScaleMassByFactor(0.5f);
	//	}
	//	break;
	case 'N':
		if (!v_particles.empty()) {
			static_cast<Proyectile*>(v_particles.back())->ScaleVelocityByFactor(2.0f);
		}
		break;
	case 'n':
		if (!v_particles.empty()) {
			static_cast<Proyectile*>(v_particles.back())->ScaleVelocityByFactor(0.5f);
		}
		break;
	case 'p': 
		{ // Bala
		v_particles.push_back(new Proyectile(
			cameraTransform.p + cameraTransform.q.rotate(Vector3(0, 0, -1)) * 10.0f,    // posición inicial
			cameraTransform.q.rotate(Vector3(0, 0, -1)) * 250.0f,    // velocidad
			Vector3D(0, 0, 0),    // aceleración
			0.008f    // masa
			));
		static_cast<Proyectile*>(v_particles.back())->ScaleVelocity(cameraTransform.q.rotate(Vector3D(0,0,-30.0f)));
		std::cout << "Proyectil lanzado desde la cámara con posición: " << v_particles.back()->getPosition() << " y velocidad: " << v_particles.back()->getVelocity() << std::endl;
		break;
		}
	case 'P':
	{	//Bola de billar
		v_particles.push_back(new Proyectile(
			cameraTransform.p + cameraTransform.q.rotate(Vector3(0, 0, -1)) * 10.0f,    // posición inicial
			cameraTransform.q.rotate(Vector3(0, 0, -1)) * 8.0f,    // velocidad
			Vector3D(0, 0, 0),    // aceleración
			156.0f    // masa
			, 0.99f    // damping
			, Vector4(0.0f, 0.0f, 0.0f, 1.0f)
			, physx::PxSphereGeometry(5.73f)
		));
		static_cast<Proyectile*>(v_particles.back())->ScaleVelocity(cameraTransform.q.rotate(Vector3D(0, 0, -15.0f)));
		std::cout << "Proyectil lanzado desde la cámara con posición: " << v_particles.back()->getPosition() << " y velocidad: " << v_particles.back()->getVelocity() << std::endl;
		break;
	}
	default:
		// Para otras teclas, no hacemos nada específico en esta escena
		break;
	}
}