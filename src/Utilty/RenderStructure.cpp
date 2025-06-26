#include "RenderStructure.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "MathHelper.h"

#pragma region Transform
/// <summary>
/// 將Transform轉換成Model Matrix，應用的順序是：縮放->旋轉->平移->父級
/// </summary>
glm::mat4 Transform::ToMatrix() {
    glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 r;
    glm::mat4 rx = glm::rotate(glm::mat4(1.0f), MathHelper::DegToRad(rotation.x), glm::vec3(1, 0, 0));
    glm::mat4 ry = glm::rotate(glm::mat4(1.0f), MathHelper::DegToRad(rotation.y), glm::vec3(0, 1, 0));
    glm::mat4 rz = glm::rotate(glm::mat4(1.0f), MathHelper::DegToRad(rotation.z), glm::vec3(0, 0, 1));
    r = rz * ry * rx;

    glm::mat4 m = t * r * s;
    if (parent != nullptr) {
        m = parent->ToMatrix() * m;
    }
    return m;
}
#pragma endregion

#pragma region Texture
/// <summary>
/// 讀取貼圖，傳回該貼圖的id
/// </summary>
/// <param name="path">檔案路徑</param>
/// <param name="gammaCorrection">true: srgb，false: rgb</param>
/// <returns></returns>
unsigned int Texture::LoadTexture(const std::string path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#pragma endregion

#pragma region Mesh
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;

    SetupMesh();
}

/// <summary>
/// 設定VAO、VBO等資訊
/// </summary>
void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex tangent
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

/// <summary>
/// 將網格畫出來
/// </summary>
void Mesh::Draw(Shader& shader) {
    // set model matrix
    shader.setMat4("model", transform.ToMatrix());

    // set material
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("material.texture_diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, material.diffuseMap.id);
    glActiveTexture(GL_TEXTURE1);
    shader.setInt("material.texture_normal", 1);
    glBindTexture(GL_TEXTURE_2D, material.normalMap.id);
    shader.setFloat("material.roughness", material.roughness);
    shader.setFloat("material.subsurface", material.subsurface);
    shader.setFloat("material.sheen", material.sheen);
    shader.setFloat("material.sheenTint", material.sheenTint);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
#pragma endregion

#pragma region Model
Model::Model(std::string path)
{
    LoadModel(path);
}

/// <summary>
/// 把模型的所有網格畫出來
/// </summary>
void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

/// <summary>
/// 回傳網格的參考
/// </summary>
Mesh& Model::GetMesh(int index) {
    if (index < 0 || index >= meshes.size()) {
        std::cout << "ERROR: index out of range\n";
    }
    return meshes[index];
}

/// <summary>
/// 讀取模型，只有節點資料，不包含材質
/// </summary>
/// <param name="path">檔案路徑</param>
void Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        // vertex
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normal and tangent
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
        }

        // texCoords
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}
#pragma endregion