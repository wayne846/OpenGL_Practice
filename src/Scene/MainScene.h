#pragma once

#include "Scene.h"
#include "../Utilty/Shader.h"
#include "../Utilty/RenderStructure.h"

class MainScene : public Scene {
public:
	MainScene();
	~MainScene();

	bool Initialize(float aspect) override;
	void Update(double dt) override;
	void Render() override;

	void OnResize(int width, int height) override;
	void OnKeyboard(int key, int action) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseMove(double x, double y) override;
	void OnScroll(double xoffset, double yoffset) override;

	void DrawGUI() override;

private:
	void LoadShader();
	void LoadMaterial();
	void LoadModel();

private:
	Shader* shader = nullptr;
	
	Model* sphere = nullptr;
	Material* sphereMat = nullptr;

	Material basic;
	Material brickwall;
	Material badlands;
	Material plaster;
	Material woodFloor;

	glm::vec3 lightDir = glm::vec3(-1, -1, 0);

	unsigned int matricesUBO = 0;

	float fps;
	float deltaTime;
	bool isLightRotate = true;;
};