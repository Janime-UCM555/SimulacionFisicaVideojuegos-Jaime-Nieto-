#pragma once
#include "Scene.h"
class Scene0 : public Scene
{
	public:
	Scene0(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void update(double dt) override {
		
	}
};

