#pragma once
#include "Scene.h"
class Scene0 : public Scene
{
public:
	Scene0(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void update(double dt) override {}
private:
	virtual void cleanup() override;
	std::vector<RenderItem*> m_renderItems; // Lista de RenderItems asociados a la escena
	std::vector<physx::PxTransform> m_transforms; // Lista de actores físicos asociados a la escena
};