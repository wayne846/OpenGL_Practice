#include "MainScene.h"

#include "../Utilty/Path.h"

#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <imgui.h>

MainScene::MainScene() {

}

MainScene::~MainScene() {

}

bool MainScene::Initialize(float aspect) {
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	LoadShader();
	LoadMaterial();
	LoadModel();

	camera.SetAspect(aspect);
	camera.SetPos(glm::vec3(0, 0, 2));
	camera.LookAt(glm::vec3(0, 0, 0));

	return true;
}

void MainScene::Update(double dt) {
	camera.Update();

	if (isLightRotate) {
		glm::vec4 light = glm::vec4(lightDir, 1);
		light = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0, 1, 0)) * light;
		lightDir = glm::vec3(light);
	}
	
	fps = 1.0 / dt;
	deltaTime = dt * 1000.0;
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

	// set light
	shader->setVec3("lightDir", lightDir);
	shader->setVec3("eyePos", camera.GetPos());

	sphere->Draw(*shader);
	glFlush();
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

void MainScene::LoadMaterial() {
	brickwall.baseColorMap.id = Texture::LoadTexture(Path::Texture::BRICKWALL, true);
	brickwall.useBaseColorMap = true;
	brickwall.normalMap.id = Texture::LoadTexture(Path::Texture::BRICKWALL_NORMAL, false);
	brickwall.useNormalMap = true;

	badlands.baseColorMap.id = Texture::LoadTexture(Path::Texture::BADLANDS, true);
	badlands.useBaseColorMap = true;
	badlands.normalMap.id = Texture::LoadTexture(Path::Texture::BADLANDS_NORMAL, false);
	badlands.useNormalMap = true;

	plaster.baseColorMap.id = Texture::LoadTexture(Path::Texture::PLASTER, true);
	plaster.useBaseColorMap = true;
	plaster.normalMap.id = Texture::LoadTexture(Path::Texture::PLASTER_NORMAL, false);
	plaster.useNormalMap = true;
	plaster.useRoughnessMap = Texture::LoadTexture(Path::Texture::PLASTER_ROUGHNESS, false);

	woodFloor.baseColorMap.id = Texture::LoadTexture(Path::Texture::WOODFLOOR, true);
	woodFloor.useBaseColorMap = true;
	woodFloor.normalMap.id = Texture::LoadTexture(Path::Texture::WOODFLOOR_NORMAL, false);
	woodFloor.useNormalMap = true;
	woodFloor.useRoughnessMap = Texture::LoadTexture(Path::Texture::WOODFLOOR_ROUGHNESS, false);
}

void MainScene::LoadModel() {
	sphere = new Model(Path::Model::SPHERE);
	Mesh& sphereMesh = sphere->GetMesh(0);
	sphereMat = &woodFloor;
	sphereMesh.SetMaterial(sphereMat);
}

#pragma region Event
/// <summary>
/// 視窗縮放事件
/// </summary>
void MainScene::OnResize(int width, int height) {
	camera.SetAspect((float)width / (float)height);
}

/// <summary>
/// 鍵盤按鈕事件
/// </summary>
void MainScene::OnKeyboard(int key, int action) {
	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_S:
	case GLFW_KEY_A:
	case GLFW_KEY_D:
	case GLFW_KEY_SPACE:
		camera.OnKeyboard(key, action);
		break;
	}
}

/// <summary>
/// 滑鼠按鈕事件
/// </summary>
/// <param name="button">0: 左鍵，1: 右鍵，2: 中鍵</param>
/// <param name="action">0: 放開，1: 按下</param>
/// <param name="mods"></param>
void MainScene::OnMouseButton(int button, int action, int mods) {
	camera.OnMouseButton(button, action);
}

/// <summary>
/// 滑鼠移動事件，原點在視窗左上角
/// </summary>
void MainScene::OnMouseMove(double x, double y) {
	camera.OnMouseMove(x, y);
}

/// <summary>
/// 滑鼠滾輪滾動事件
/// </summary>
void MainScene::OnScroll(double xoffset, double yoffset) {
	camera.OnScroll(xoffset, yoffset);
}
#pragma endregion

#pragma region GUI
void MainScene::DrawGUI() {
	ImGui::Text("FPS: %d (%.3fms)", (int)fps, deltaTime);
	ImGui::Checkbox("Light Rotate", &isLightRotate);

	Material* material = sphereMat;
	if (material != &Material::Default()) {
		ImGui::SeparatorText("Material Parameters:");
		ImGui::SliderFloat("Roughness", &material->roughness, 0.0f, 1.0f);
		ImGui::SliderFloat("Subsurface", &material->subsurface, 0.0f, 1.0f);
		ImGui::SliderFloat("Sheen", &material->sheen, 0.0f, 1.0f);
		ImGui::SliderFloat("Sheen Tint", &material->sheenTint, 0.0f, 1.0f);
		ImGui::SliderFloat("Anisotropic", &material->anisotropic, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular", &material->specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Tint", &material->specularTint, 0.0f, 1.0f);
		ImGui::SliderFloat("Metallic", &material->metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Clearcoat", &material->clearcoat, 0.0f, 1.0f);
		ImGui::SliderFloat("Clearcoat Gloss", &material->clearcoatGloss, 0.0f, 1.0f);
	}
	
}
#pragma endregion