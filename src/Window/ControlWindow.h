#pragma once

#include <Scene/Scene.h>

class ControlWindow
{
public:
	ControlWindow();

	auto Initialize() -> bool;
	void Display();

private:
	bool showDemoWindow;

private:
	Scene* targetScene;

public:
	void SetTargetScene(Scene* scene) {
		targetScene = scene;
	}
};

