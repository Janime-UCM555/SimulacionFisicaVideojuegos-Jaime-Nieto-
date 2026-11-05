#pragma once
#include "Particle.h"
#include "UpdatableObj.h"
#include <algorithm>
#include <cmath>

class GeneradorFuerzas : public UpdatableObj
{
protected:
	bool active = true;
	float elapsedTime = 0.0f;

public:
	virtual ~GeneradorFuerzas() = default;

	void reset() { elapsedTime = 0.0f; }

	void setActive(bool a) { if (a && !active) { reset(); } active = a; }
	void toggleActive() { setActive(!active); }
	bool isActive() const { return active; }

	virtual Vector3D getForce(const Particle& p) const = 0;

	void update(double dt) override { if (active) { elapsedTime += dt; } }
};

class GravityForce : public GeneradorFuerzas
{
private:
	Vector3D gravity;

public:
	explicit GravityForce(const Vector3D& g = GRAVITY) : gravity(g) {}

	Vector3D getForce(const Particle& p) const override { return gravity * p.getMass(); }
};

class WindForce : public GeneradorFuerzas
{
protected:
	virtual Vector3D getWindVelocity(const Vector3D& p) const { return velocidadViento; }

	bool isWithinWindZone(const Vector3D& p) const {
		Vector3D pos = p - centro;
		return pos.x >= -altura.x && pos.x <= altura.x &&
			pos.y >= 0 && pos.y <= altura.y &&
			pos.z >= -altura.z && pos.z <= altura.z;
	}

	Vector3D velocidadViento, centro, altura;
	float k1, k2;

public:
	WindForce(const Vector3D& v, const Vector3D& c, const Vector3D& h, float k1 = 0.5f, float k2 = 0.0f)
		: velocidadViento(v), centro(c), altura(h), k1(k1), k2(k2) {}

	virtual	~WindForce() = default;

	Vector3D getForce(const Particle& p) const override
	{
		const Vector3D& pos = p.getPosition();
		if (!isWithinWindZone(pos))
		{
			return Vector3D(0, 0, 0);
		}
		const Vector3D& windVelocity = getWindVelocity(pos) - p.getVelocity();
		return windVelocity * k1 + windVelocity * (windVelocity.magnitude() * k2);
	}
};

class WhirlwindForce : public WindForce
{
private:
	Vector3D getWindVelocity(const Vector3D& p) const override
	{
		const Vector3D r = p - centro;
		return Vector3D(-r.z, 50.0f - r.y, r.x) * k;
	}
	float k;

public:
	WhirlwindForce(const Vector3D& c, const Vector3D& h, float k = 0.3f, float k1 = 1.0f, float k2 = 0.0f)
		: WindForce(Vector3D(0, 0, 0), c, h, k1, k2), k(k) {}
};

class ExplosionForce : public GeneradorFuerzas
{
private:
	float radioActual() const {
		if (ve <= 0.0f) return R;
		return min(R, ve * static_cast<float>(elapsedTime));
	}
	Vector3D c;
	float K, R, tau, ve;

public:
	ExplosionForce(Vector3D center, float K, float R, float tau, float ve = 0.0f)
		: c(center), K(K), R(R), tau(tau), ve(ve) {
		active = false;
	}

	/// Detona: reinicia el tiempo y activa la explosión.
	void detonate() { elapsedTime = 0.0; active = true; }

	Vector3D getForce(const Particle& p) const override {
		if (!active) return Vector3D(0, 0, 0);
		const Vector3D d = p.getPosition() - c;
		const float r2 = d.dot(d);
		const float radio = radioActual();
		if (r2 >= radio * radio) return Vector3D(0, 0, 0);
		const float decay = std::exp(-static_cast<float>(elapsedTime) / tau);
		return d * (K / max(r2, 1.0f) * decay);   // max evita dividir por ~0 en el centro
	}

	void update(double dt) override {
		GeneradorFuerzas::update(dt);
		if (active && elapsedTime > 5.0 * tau) active = false;   // a partir de ~4 tau ya se ha desvanecido
	}
};