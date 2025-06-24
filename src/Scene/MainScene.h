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
	//void OnKeyboard(int key, int action) override;

private:
	void LoadShader();
	void LoadModel();

private:
	Shader* shader = nullptr;
	
	Model* sphere = nullptr;

	unsigned int matricesUBO = 0;
};