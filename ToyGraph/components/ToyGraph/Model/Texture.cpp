/*
    ToyGraph Texture.cpp
    created on 2022.10.10
    refactored on 2022.10.17
*/
#include "ToyGraphCommon/EngineCompileOptions.h"
// stb image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ToyGraph/Model/Texture.h>

using namespace std;

/**
 * 从文件读取材质，并绑定到 OpenGL。
 * 
 * @return 材质id。
 */
GLuint TextureUtils::loadTextureFromFile(const string& filepath) {

    GLuint textureId;
    glGenTextures(1, &textureId);

    int width;
    int height;
    int nrComponents;
    unsigned char* data = stbi_load(
        filepath.c_str(), &width, &height, &nrComponents, 0
    );

    if (!data) {
        throw 1;
    }

    GLenum format;
    
    if (nrComponents == 1) {
        format = GL_RED;
    } else if (nrComponents == 3) {
        format = GL_RGB;
    } else if (nrComponents == 4) {
        format = GL_RGBA;
    } else {
        throw 2;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureId;
}
