#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D textureDiffuse0;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.textureDiffuse0, TexCoords);
}
