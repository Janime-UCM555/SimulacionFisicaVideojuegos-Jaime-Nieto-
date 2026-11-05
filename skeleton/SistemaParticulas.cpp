#include "SistemaParticulas.h"

SistemaParticulas::~SistemaParticulas() {
	for (auto& p : particulas) {
		delete p; // Liberar memoria de cada partícula
	}
	particulas.clear();
}

void SistemaParticulas::addGenerador(std::unique_ptr<GeneradorParticulas> g) {
	generadores.push_back(std::move(g));
}

void SistemaParticulas::addParticula(Particle* p) {
	p->bindForces(&fuerzas);
	particulas.push_back(p);
}

void SistemaParticulas::setGeneradorActivo(size_t index, bool activo) {
	if (index < generadores.size()) {
		generadores[index]->setActive(activo);
	}
}

void SistemaParticulas::toggleGeneradorActivo(size_t index) {
	if (index < generadores.size()) {
		generadores[index]->toggleActive();
	}
}

void SistemaParticulas::setGeneradorActivoTodos(bool activo) {
	for (auto& g : generadores) {
		g->setActive(activo);
	}
}

void SistemaParticulas::toggleGeneradorActivoTodos() {
	for (auto& g : generadores) {
		g->toggleActive();
	}
}

void SistemaParticulas::update(double dt) {
	// Generar nuevas partículas desde cada generador activo
	for (auto& g : generadores) {
		auto nuevasParticulas = g->generarParticulas(dt);
		for (auto* p : nuevasParticulas) {
			p->bindForces(&fuerzas);
			particulas.push_back(p);
		}
	}
	// Actualizar todas las partículas existentes
	for (auto it = particulas.begin(); it != particulas.end();) {
		Particle* p = *it;
		p->update(dt);
		// Comprobar si la partícula ha expirado o está fuera del rango
		if (p->isExpired() || fueraDeRango(p->getPosition())) {
			delete p; // Liberar memoria de la partícula
			it = particulas.erase(it); // Eliminar del vector y obtener el siguiente iterador
		}
		else {
			++it; // Avanzar al siguiente elemento
		}
	}
}