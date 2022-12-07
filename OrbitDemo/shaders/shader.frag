#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D textureDiffuse0;

    float shininess;
};

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

uniform Light light;

uniform Material material;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{    

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
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.textureDiffuse0, TexCoords));

    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = (spec * material.specular) * light.specular;
    //vec3 specular = light.specular * spec * (texture(material.specular, TexCoords)).rgb;

    // »·¾³¹â¡£
    // vec3 ambient = material.ambient * light.ambient;
    vec3 ambient = light.ambient * (texture(material.textureDiffuse0, TexCoords)).rgb;

    diffuse *= intensity;
    //specular *= intensity;

    vec3 res = ambient + diffuse;
    FragColor = vec4(res, 1.0);
    
    //FragColor = texture(material.textureDiffuse0, TexCoords);
}
