// ToyGraph Texture.cpp
// created on 2022.10.10

#include <ToyGraph/Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

Texture::Texture(const char* path) {
    this->load(path);
}

TextureError Texture::load(const char* path) {
    
    this->errmsg = "";

    if (this->textureId < 0) {
        glGenTextures(1, &this->textureId);
    }

    unsigned char* data = stbi_load(
        path, &width, &height, &nrComponents, 0
    );

    if (data) {
    
        switch (nrComponents) {
            case 1: {
                format = GL_RED;
                break;
            }

            case 3: {
                format = GL_RGB;
                break;
            }

            case 4: {
                format = GL_RGBA;
                break;
            }

            default: {
                errmsg = "bad format. nrComponents is ";
                errmsg += to_string(nrComponents);
                this->textureId = -1;

                return errcode = TextureError::BAD_FORMAT;
            }
        };

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
            0, format, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    } else {
        errmsg = "failed to load texture. path is: ";
        errmsg += path;
        this->textureId = -1;
        return errcode = TextureError::FAILED_TO_LOAD_TEXTURE;
    }

    return this->errcode = TextureError::TEXTURE_OK;
}

GLuint TextureUtils::loadTexture(const char* path) {
    Texture texture(path);
    return texture.getId();
}
