/*
    Model.cpp
    创建于 2022年10月16日。
*/

#include "ToyGraphCommon/EngineCompileOptions.h"
#include <ToyGraph/Model/Model.h>

#if TG_OS_IS_WINDOWS
    #include <ToyGraphCommon/CharEncodingConverter.h>
#endif

using namespace std;


/* ------------ internal. ------------ */



/* ------------ public. ------------ */

Model::Model(
    const string& filepath,     
    bool flipUVs, 
    CodePage binaryFileCp
) {
    this->loadModel(filepath, flipUVs, binaryFileCp);
}

void Model::draw(Shader& shader) {

    for (auto& mesh : this->meshes) {
        mesh.draw(shader);
    }
}

void Model::loadModel(
    const string& filepath, 
    bool flipUVs, 
    CodePage binaryFileCp
) {

    Assimp::Importer importer;

    /*
        从文件导入模型。

        triangulate: 令 importer 将可能存在的非三角形转换成三角形。

        flipUVs: 反转贴图的 y 轴。这样我们就不用自己弄了。
                 （实际效果与预期不符，暂时关闭...）

        其他可选：aiProcess_GenNormals
                aiProcess_SplitLargeMeshes
                aiProcess_OptimizeMeshes
    */
    // 注：importer 会在析构时释放 scene。

    unsigned int importerFlags = aiProcess_Triangulate;
    
    if (flipUVs) {
        importerFlags |= aiProcess_FlipUVs;
    }

    const aiScene* scene = importer.ReadFile(filepath, importerFlags);
    

    // 错误处理。
    if (!scene 
        || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        || !scene->mRootNode
    ) {
        this->errcode = ModelError::FAILED_TO_LOAD_MODEL;
        this->errmsg = importer.GetErrorString();
        return;
    }

    directory = filepath.substr(0, filepath.find_last_of('/'));

    this->processNode(scene->mRootNode, scene, binaryFileCp);
}


/* ------------ protected. ------------ */


void Model::processNode(aiNode* node, const aiScene* scene, CodePage binaryFileCp) {

    for (unsigned int idx = 0; idx < node->mNumMeshes; idx++) {
        aiMesh*& mesh = scene->mMeshes[node->mMeshes[idx]];
        this->processAndAppendMesh(mesh, scene, binaryFileCp);
    }

    for (unsigned int idx = 0; idx < node->mNumChildren; idx++) {
        this->processNode(node->mChildren[idx], scene, binaryFileCp);
    }

}

void Model::processAndAppendMesh(aiMesh* mesh, const aiScene* scene, CodePage binaryFileCp) {

    // 该方法实现性能优于教材《Learn OpenGL》的实现版本。

    this->meshes.emplace_back(); // 内部构造，避免拷贝。
    auto& tgMeshObj = this->meshes.back();

    vector<Vertex>& vertices = tgMeshObj.vertices;
    vector<GLuint>& indices = tgMeshObj.indices;
    vector<Texture>& textures = tgMeshObj.textures;

    for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
        
        vertices.emplace_back(); // 内部构造，避免拷贝。
        auto& vertex = vertices.back();

        vertex.position.x = mesh->mVertices[idx].x;
        vertex.position.y = mesh->mVertices[idx].y;
        vertex.position.z = mesh->mVertices[idx].z;

        vertex.normal.x = mesh->mNormals[idx].x;
        vertex.normal.y = mesh->mNormals[idx].y;
        vertex.normal.z = mesh->mNormals[idx].z;

        if (mesh->mTextureCoords[0]) {
            vertex.textureCoord.x = mesh->mTextureCoords[0][idx].x;
            vertex.textureCoord.y = mesh->mTextureCoords[0][idx].y;
        }

    }

    // indices
    for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
        aiFace& face = mesh->mFaces[faceIdx];
        for (unsigned int indicesIdx = 0; indicesIdx < face.mNumIndices; indicesIdx++) {
            indices.push_back(face.mIndices[indicesIdx]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        this->loadMaterialTextures(
            material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, 
            textures, binaryFileCp
        );

        this->loadMaterialTextures(
            material, aiTextureType_SPECULAR, TextureType::SPECULAR, 
            textures, binaryFileCp
        );

    }

    tgMeshObj.setupMesh();
}

void Model::loadMaterialTextures(
    aiMaterial *material, 
    aiTextureType aitype, 
    TextureType type, 
    vector<Texture>& container,
    CodePage binaryFileCp
) {
    
    auto& textures = container;

    unsigned int textureCount = material->GetTextureCount(aitype);

    for (unsigned int idx = 0; idx < textureCount; idx++) {
    
        aiString str;
        material->GetTexture(aitype, idx, &str);

        string filepath = directory;
        filepath += '/';

        string filename = str.C_Str();
        
#if TG_OS_IS_WINDOWS && TG_OS_ENCODING_IS_CHINESE_GB
        if (binaryFileCp == CodePage::UTF8) {
            filename = CharEncodingConverter::utf8ToGb(filename);
        }
#endif

        filepath += filename;

        Texture* prevLoadedTexture = Engine::getInstance().getLoadedTexture(filepath);

        if (prevLoadedTexture != nullptr) {

            textures.emplace_back(*prevLoadedTexture);

        } else {

            textures.emplace_back();
            Texture& texture = textures.back();

            try {

                texture.id = TextureUtils::loadTextureFromFile(filepath);

            } catch (...) {

                this->errcode = ModelError::FAILED_TO_LOAD_TEXTURE;
                this->errmsg = "failed to load texture: ";
                this->errmsg += directory;
                this->errmsg += '/';
                this->errmsg += str.C_Str();

                return;

            }

            texture.type = type;

            Engine::getInstance().texturesLoaded[filepath] = texture;
        }
    } // for (unsigned int idx = 0; idx < textureCount; idx++) 
} // function: void Model::loadMaterialTextures


