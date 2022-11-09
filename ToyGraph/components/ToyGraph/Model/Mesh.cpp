/*
    Mesh.cpp
    创建于 2022年10月16日。
*/
#include "ToyGraphCommon/EngineCompileOptions.h"
#include "ToyGraph/Model/Mesh.h"

using namespace std;

/* ------------ public. ------------ */

Mesh::Mesh(
    vector<Vertex>& vertices,
    vector<GLuint>& indices,
    vector<Texture>& textures
) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->setupMesh();
}

Mesh::Mesh() {

}

void Mesh::setupMesh() {
    if (initialized) {
        return;
    }

    initialized = true;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER, 
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        indices.size() * sizeof(GLuint),
        indices.data(),
        GL_STATIC_DRAW
    );

    // vertex positions.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0
    );

    // vertex normals.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal)
    );

    // vertex texture coords.
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, textureCoord)
    );

    glBindVertexArray(0);
}


void Mesh::draw(Shader& shader) {

    unsigned int diffuseCounter = 0;
    unsigned int specularCounter = 0;
    for (unsigned int idx = 0; idx < this->textures.size(); idx++) {
        const auto& texture = this->textures[idx];

        glActiveTexture(GL_TEXTURE0 + idx);
        string number;
        string name;

        switch (texture.type) {
            case TextureType::DIFFUSE: {
                name = "textureDiffuse";
                number = to_string(diffuseCounter++);
                break;
            }

            case TextureType::SPECULAR: {
                name = "textureSpecular";
                number = to_string(specularCounter++);
                break;
            }
            
            default: {
                return; // 无法绘制。
            }
        }

        shader.setInt("material." + name + number, idx);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    // draw mesh.
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

}

/* ------------ protected. ------------ */

