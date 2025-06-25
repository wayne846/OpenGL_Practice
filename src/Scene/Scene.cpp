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

void Scene::OnMouseButton(int button, int action, int mods) {
	std::cout << "Scene OnMouseButton: " << button << " " << action << " " << mods << std::endl;
}

void Scene::OnMouseMove(double x, double y) {
	std::cout << "Scene OnMouseMove: " << x << " " << y << std::endl;
}

void Scene::OnScroll(double xoffset, double yoffset) {
	std::cout << "Scene OnScroll: " << xoffset << " " << yoffset << std::endl;
}
