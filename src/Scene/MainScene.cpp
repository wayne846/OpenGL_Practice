#include "MainScene.h"

#include "../Utilty/Path.h"

#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MainScene::MainScene() {

}

MainScene::~MainScene() {

}

bool MainScene::Initialize(float aspect) {
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	LoadShader();
	LoadModel();

	camera.SetAspect(aspect);
	camera.LookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	return true;
}

void MainScene::Update(double dt) {

}

void MainScene::Render() {
	glClearColor(0.0, 0.0, 0.0, 1); //black screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();

	// set ubo
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetProjectionMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	sphere->Draw(*shader);
	glFlush();
}

void MainScene::OnResize(int width, int height) {
	camera.SetAspect((float)width / (float)height);
}

void MainScene::LoadShader() {
	Path::Converter* pathConverter = Path::Converter::GetInstance();
	shader = new Shader(Path::Shader::STANDARD_VERT.c_str(), Path::Shader::STANDARD_FRAG.c_str());

	// set uniform block
	std::vector<Shader*> shaders = { shader };
	for (int i = 0; i < shaders.size(); i++) {
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaders[i]->ID, "Matrices");
		glUniformBlockBinding(shaders[i]->ID, uniformBlockIndex, 0);
	}

	// set UBO
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));

}

void MainScene::LoadModel() {
	sphere = new Model(Path::Model::SPHERE);
}