#pragma once
#include "Scene.h"
class Scene2 : public Scene
{
public:
	Scene2(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void update(double dt) override;
};

