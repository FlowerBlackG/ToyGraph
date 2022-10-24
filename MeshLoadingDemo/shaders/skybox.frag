#version 450 core
out vec4 FragColor;

int vec3 TexCoords;
uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);
}
