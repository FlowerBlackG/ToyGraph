/*
    Model.cpp
    ������ 2022��10��16�ա�
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
        ���ļ�����ģ�͡�

        triangulate: �� importer �����ܴ��ڵķ�������ת���������Ρ�

        flipUVs: ��ת��ͼ�� y �ᡣ�������ǾͲ����Լ�Ū�ˡ�
                 ��ʵ��Ч����Ԥ�ڲ�������ʱ�ر�...��

        ������ѡ��aiProcess_GenNormals
                aiProcess_SplitLargeMeshes
                aiProcess_OptimizeMeshes
    */
    // ע��importer ��������ʱ�ͷ� scene��

    unsigned int importerFlags = aiProcess_Triangulate;
    
    if (flipUVs) {
        importerFlags |= aiProcess_FlipUVs;
    }

    const aiScene* scene = importer.ReadFile(filepath, importerFlags);
    

    // ������
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

    // �÷���ʵ���������ڽ̲ġ�Learn OpenGL����ʵ�ְ汾��

    this->meshes.emplace_back(); // �ڲ����죬���⿽����
    auto& tgMeshObj = this->meshes.back();

    vector<Vertex>& vertices = tgMeshObj.vertices;
    vector<GLuint>& indices = tgMeshObj.indices;
    vector<Texture>& textures = tgMeshObj.textures;

    for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
        
        vertices.emplace_back(); // �ڲ����죬���⿽����
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


