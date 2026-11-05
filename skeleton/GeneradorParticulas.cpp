#include "GeneradorParticulas.h"
GeneradorParticulas::GeneradorParticulas(ParticleDesc d, float r, Vector3D posS, Vector3D velS)
	: modelo(std::make_unique<Particle>(d))
	, rate(r)
	, posSpread(posS)
	, velSpread(velS)
	, active(true)
	, acumulador(0.0)
	, mt(std::random_device{}())
{}

float GeneradorNormal::normal(float media, float desviacion) {
	// Genera un número aleatorio con distribución normal (gaussiana) usando la media y desviación estándar proporcionadas.
	if (desviacion <= 0) return media;
	std::normal_distribution<float> dist(media, desviacion);
	return dist(mt);
}

float GeneradorUniforme::uniforme(float min, float max) {
	// Genera un número aleatorio con distribución uniforme entre min y max.
	if (max <= min) return min;
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}

std::vector<Particle*> GeneradorParticulas::generarParticulas(double dt) {
	std::vector<Particle*> nuevasParticulas;
	if (!active) return nuevasParticulas;
	acumulador += dt * rate; // Acumula el tiempo multiplicado por la tasa de generación
	int numNuevas = static_cast<int>(acumulador); // Número de partículas a generar
	acumulador -= numNuevas; // Resta las partículas generadas del acumulador
	for (int i = 0; i < numNuevas; ++i) {
		Particle* p = modelo->clone();
		p->setPosition(generarPosicion());
		p->setVelocity(generarVelocidad());
		nuevasParticulas.push_back(p);
	}
	return nuevasParticulas;
}

GeneradorNormal::GeneradorNormal(ParticleDesc d, float r, Vector3D posS, Vector3D velS)
	: GeneradorParticulas(d, r, posS, velS) {}

GeneradorUniforme::GeneradorUniforme(ParticleDesc d, float r, Vector3D posS, Vector3D velS)
	: GeneradorParticulas(d, r, posS, velS) {}

Vector3D GeneradorNormal::generarPosicion() {
	const Vector3D basePos = modelo->getPosition();
	return Vector3D(normal(basePos.x, posSpread.x), normal(basePos.y, posSpread.y), normal(basePos.z, posSpread.z));
}

Vector3D GeneradorNormal::generarVelocidad() {
	const Vector3D baseVel = modelo->getVelocity();
	return Vector3D(normal(baseVel.x, velSpread.x), normal(baseVel.y, velSpread.y), normal(baseVel.z, velSpread.z));
}

Vector3D GeneradorUniforme::generarPosicion() {
	const Vector3D basePos = modelo->getPosition();
	return Vector3D(uniforme(basePos.x - posSpread.x, basePos.x + posSpread.x),
		uniforme(basePos.y - posSpread.y, basePos.y + posSpread.y),
		uniforme(basePos.z - posSpread.z, basePos.z + posSpread.z));
}

Vector3D GeneradorUniforme::generarVelocidad() {
	const Vector3D baseVel = modelo->getVelocity();
	return Vector3D(uniforme(baseVel.x - velSpread.x, baseVel.x + velSpread.x),
		uniforme(baseVel.y - velSpread.y, baseVel.y + velSpread.y),
		uniforme(baseVel.z - velSpread.z, baseVel.z + velSpread.z));
}