#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Texture {
public:
    static unsigned int Texture::LoadTexture(char const* path, bool gammaCorrection);

public:
	unsigned int id;
	std::string type;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(Shader& shader);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};

class Model{
public:
    Model(std::string path)
    {
        LoadModel(path);
    }
    void Draw(Shader& shader);

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};