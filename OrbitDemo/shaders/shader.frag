#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureDiffuse1;

uniform vec3 colorFilter;

void main()
{    
    FragColor = texture(textureDiffuse1, TexCoords) * vec4(colorFilter, 1.0);
}