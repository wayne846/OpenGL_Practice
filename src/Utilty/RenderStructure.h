#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "MathHelper.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
    glm::vec3 tangent;
	glm::vec2 texCoords;
};

class Transform {
public:
    glm::mat4 ToMatrix();

public:
    glm::vec3 position = MathHelper::VEC3_ZERO;
    glm::vec3 rotation = MathHelper::VEC3_ZERO;
    glm::vec3 scale = MathHelper::VEC3_ONE;

    Transform* parent = nullptr;
};

class Texture {
public:
    static unsigned int Texture::LoadTexture(const std::string path, bool gammaCorrection);

public:
	unsigned int id;
	std::string type;
};

struct Material {
    Texture diffuseMap;
    Texture normalMap;
    float roughness = 0.5f;
    float subsurface = 0.0f;
    float sheen = 0.0f;
    float sheenTint = 0.5f;
    float anisotropic = 0;
    float specular = 0.5;
    float specularTint = 0;
    float metallic = 0;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    Transform transform;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(Shader& shader);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};

class Model{
public:
    Model(std::string path);

    void Draw(Shader& shader);
    Mesh& GetMesh(int index);

public:

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};