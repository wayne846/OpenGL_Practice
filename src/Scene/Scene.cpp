#include <Windows.h>
#include <iostream>

#include "Scene.h"

Scene::Scene() {

}

Scene::~Scene() {

}

bool Scene::Initialize(float aspect) {
	return true;
}

void Scene::Update(double dt) {
	
}

void Scene::Render() {
	glClearColor(0.0, 0.0, 0.0, 1); //black screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void Scene::OnResize(int width, int height) {
	std::cout << "Scene Resize: " << width << " " << height << std::endl;
}

void Scene::OnKeyboard(int key, int action) {
	std::cout << "Scene OnKeyboard: " << key << " " << action << std::endl;
}
