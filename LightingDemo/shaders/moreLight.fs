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
    //vec3 specular;
    sampler2D specular;

    /**
     * 镜面反射强度。
     */
    float shininess;
};


uniform Material material;

struct Light {
    vec3 position;

    vec3 direction;

    float cutOff;
    float outerCutOff;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

/* 平行光源。 */

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

in vec2 TexCoords;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

/* 点光源。 */

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (
        light.constant + light.linear * distance + light.quadratic * distance * distance
    );

    // result
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

uniform Light light;


out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;


uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
/*
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (
        light.constant + light.linear * distance + light.quadratic * (distance * distance)
    );

    vec3 lightDir = normalize(light.position - FragPos); // normalize(lightPos - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    //if (theta > light.cutOff) {
    // diffuse light
    vec3 norm = normalize(Normal);
    

    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = (diff * material.diffuse) * light.diffuse;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = (spec * material.specular) * light.specular;
    vec3 specular = light.specular * spec * (texture(material.specular, TexCoords)).rgb;

    // 环境光。
    // vec3 ambient = material.ambient * light.ambient;
    vec3 ambient = light.ambient * (texture(material.diffuse, TexCoords)).rgb;

    // ambient *= attenuation;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;


    FragColor = vec4(result, 1.0);
    //} else {
        //FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    //}

*/

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // directional lighting
    vec3 result = calcDirLight(dirLight, norm, viewDir);

    // point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);

}
