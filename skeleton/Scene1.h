#pragma once
#include "Scene.h"
class Scene1 : public Scene
{
public:
	Scene1(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void update(double dt) override;
};

