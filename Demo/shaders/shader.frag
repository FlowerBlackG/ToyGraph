#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform mat4 colorTransform;

void main() {
	FragColor = mix(
		texture(texture1, TexCoord),
		texture(texture2, TexCoord), 0.2
	);

	//FragColor = vec4(lightColor * objectColor, 1.0);

}
