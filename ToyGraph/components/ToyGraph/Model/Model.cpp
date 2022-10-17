/*
    Model.cpp
    创建于 2022年10月16日。
*/

#include <ToyGraph/Model/Model.h>

using namespace std;


/* ------------ internal. ------------ */



/* ------------ public. ------------ */

Model::Model(const string& filepath) {
    this->loadModel(filepath);
}

void Model::draw(Shader& shader) {
    for (auto& mesh : this->meshes) {
        mesh.draw(shader);
    }
}


/* ------------ protected. ------------ */

void Model::loadModel(const string& filepath) {
    Assimp::Importer importer;

    /*
        从文件导入模型。

        triangulate: 令 importer 将可能存在的非三角形转换成三角形。

        flipUVs: 反转贴图的 y 轴。这样我们就不用自己弄了。

        其他可选：aiProcess_GenNormals
                aiProcess_SplitLargeMeshes
                aiProcess_OptimizeMeshes
    */
    // 注：importer 会在析构时释放 scene。
    const aiScene* scene = importer.ReadFile(
        filepath, aiProcess_Triangulate | aiProcess_FlipUVs
    );
    

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

    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {

    for (unsigned int idx = 0; idx < node->mNumMeshes; idx++) {
        aiMesh*& mesh = scene->mMeshes[node->mMeshes[idx]];
        this->processAndAppendMesh(mesh, scene);
    }

    for (unsigned int idx = 0; idx < node->mNumChildren; idx++) {
        this->processNode(node->mChildren[idx], scene);
    }

}

void Model::processAndAppendMesh(aiMesh* mesh, const aiScene* scene) {

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
            material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, textures
        );

        this->loadMaterialTextures(
            material, aiTextureType_SPECULAR, TextureType::SPECULAR, textures
        );

    }

    tgMeshObj.setupMesh();
}

void Model::loadMaterialTextures(
    aiMaterial *material, 
    aiTextureType aitype, 
    TextureType type, 
    vector<Texture>& container
) {
    
    auto& textures = container;

    unsigned int textureCount = material->GetTextureCount(aitype);

    for (unsigned int idx = 0; idx < textureCount; idx++) {
    
        aiString str;
        material->GetTexture(aitype, idx, &str);
        
        

        string filepath = directory;
        filepath += '/';
        filepath += str.C_Str();

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


