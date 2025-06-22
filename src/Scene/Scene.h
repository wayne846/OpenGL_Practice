#pragma once

#include <array>
#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class Scene {
public:
	Scene();
	~Scene();

	virtual bool Initialize();
	virtual void Update(double dt);
	virtual void Render();

	virtual void OnResize(int width, int height);
	virtual void OnKeyboard(int key, int action);

private:
	Camera camera;

};


