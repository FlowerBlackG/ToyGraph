#version 460 core

struct Material {
    /**
     * ���ϱ��淴�����ɫ��
     */
    //vec3 ambient;

    /**
     * ������ɫ��
     */
    //vec3 diffuse;

    /**
     * ʹ�� lighting map��
     */
    sampler2D diffuse;

    /**
     * ���淴����ɫ��
     */
    //vec3 specular;
    sampler2D specular;

    /**
     * ���淴��ǿ�ȡ�
     */
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

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {

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

    // �����⡣
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


}
