#version 460 core

struct Material {
    /**
     * 材料表面反射的颜色。
     */
    //vec3 ambient;

    /**
     * 漫射颜色。
     */
    //vec3 diffuse;

    /**
     * 使用 lighting map。
     */
    sampler2D diffuse;

    /**
     * 镜面反射颜色。
     */
    vec3 specular;

    /**
     * 镜面反射强度。
     */
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform Material material;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = (diff * material.diffuse) * light.diffuse;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    // 环境光。
    // vec3 ambient = material.ambient * light.ambient;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}
