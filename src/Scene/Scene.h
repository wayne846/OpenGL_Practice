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

	virtual bool Initialize(float aspect);
	virtual void Update(double dt);
	virtual void Render();

	virtual void OnResize(int width, int height);
	virtual void OnKeyboard(int key, int action);
	virtual void OnMouseButton(int button, int action, int mods);
	virtual void OnMouseMove(double x, double y);
	virtual void OnScroll(double xoffset, double yoffset);

	virtual void DrawGUI();

protected:
	Camera camera;
};


