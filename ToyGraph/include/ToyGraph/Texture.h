// ToyGraph Texture.h
// created on 2022.10.10


#pragma once

#include <string>

#include <ToyGraph/Engine.h>


enum class TextureError {
    TEXTURE_OK,
    BAD_FORMAT,
    FAILED_TO_LOAD_TEXTURE
};

class Texture {

public:
    Texture(const char* path);

    TextureError load(const char* path);

    GLuint getId() { return this->textureId; }

public:
    TextureError errcode = TextureError::TEXTURE_OK;
    std::string errmsg = "";


protected:
    GLuint textureId = -1;
    int width = 0;
    int height = 0;
    int nrComponents = 0;
    GLenum format;


};

class TextureUtils {

public:
    static GLuint loadTexture(const char* path);

private:
    TextureUtils() = default;

};
