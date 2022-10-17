/*
    ģ�͡�
    ������ 2022��10��16�ա�
*/


#pragma once



// assimp library.
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <ToyGraph/Engine.h>

enum class ModelError {
    MODEL_OK,
    FAILED_TO_LOAD_MODEL,
    FAILED_TO_LOAD_TEXTURE
};


class Model {

public:
    Model(const std::string& filepath);

    void draw(class Shader& shader);

public:
    ModelError errcode = ModelError::MODEL_OK;
    std::string errmsg;

protected:

    std::vector<class Mesh> meshes;
    std::string directory;

    void loadModel(const std::string& filepath);

    void processNode(aiNode* node, const aiScene* scene);
    
    void processAndAppendMesh(aiMesh* mesh, const aiScene* scene);
    
    void loadMaterialTextures(
        aiMaterial *material, 
        aiTextureType aitype, 
        enum class TextureType type, 
        std::vector<Texture>& container
    );

};
