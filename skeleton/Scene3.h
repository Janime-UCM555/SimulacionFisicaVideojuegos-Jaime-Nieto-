#pragma once
#include "GeneradorFuerzas.h"
#include "Scene.h"
#include "SistemaParticulas.h"

class Scene3 : public Scene
{
public:
	Scene3(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& cameraTransform) override;

private:
	// No son dueños: los libera Scene::cleanup() desde v_objects
	GravityForce* gravedad = nullptr;
	GravityForce* gravedadLigera = nullptr;
	WindForce* viento = nullptr;
	WhirlwindForce* torbellino = nullptr;
	ExplosionForce* explosion = nullptr;
};