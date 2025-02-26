#version 410 core

in vec3 fNormal;
in vec4 fPosEye;

out vec4 fColor;

// Point light properties
uniform vec3 pointLightPosEye;      // Position of the light in eye space
uniform vec3 pointLightColor;       // Color of the point light
uniform float constantAttenuation;  // Constant attenuation factor
uniform float linearAttenuation;    // Linear attenuation factor
uniform float quadraticAttenuation; // Quadratic attenuation factor

// Material properties
uniform vec3 ambientColor;    // Ambient material color
uniform vec3 diffuseColor;    // Diffuse material color
uniform vec3 specularColor;   // Specular material color
uniform float shininess;      // Shininess factor

void main()
{
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(pointLightPosEye - fPosEye.xyz);
    float distance = length(pointLightPosEye - fPosEye.xyz);

    // Attenuation calculation
    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);

    // Ambient component
    vec3 ambient = ambientColor * pointLightColor * attenuation;

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseColor * pointLightColor * diff * attenuation;

    // Specular component
    vec3 viewDir = normalize(-fPosEye.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularColor * pointLightColor * spec * attenuation;

    // Combine components
    vec3 finalColor = ambient + diffuse + specular;
    fColor = vec4(finalColor, 1.0f);
}
