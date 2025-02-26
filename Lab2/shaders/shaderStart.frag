#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fPosLightSpace;


out vec4 fColor;

uniform vec3 lightDir;
uniform vec3 lightColor;



uniform bool isGlass;     
uniform float alpha;       

uniform bool pointLightEnabled;
uniform vec3 pointLightPosEye; 
uniform vec3 pointLightColor;

uniform bool pointTorchEnabled;
uniform vec3 pointTorchPosEye; 
uniform vec3 pointTorchColor;


uniform bool pointTorchEnabled2;
uniform vec3 pointTorchPosEye2; 
uniform vec3 pointTorchColor2;

uniform bool pointTorchEnabled3;
uniform vec3 pointTorchPosEye3; 
uniform vec3 pointTorchColor3;


uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;



uniform vec3 glassColor; 
uniform bool isTorch;
uniform bool isTorta;
uniform bool isTorta2;
uniform bool isTorta3;
float bias      = 0.005;



uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

vec3 pointLightDiffuse;
vec3 pointLightSpecular;
vec3 pointLightAmbient;

vec3 pointTorchDiffuse;
vec3 pointTorchSpecular;
vec3 pointTorchAmbient;

vec3 pointTorchDiffuse2;
vec3 pointTorchSpecular2;
vec3 pointTorchAmbient2;

vec3 pointTorchDiffuse3;
vec3 pointTorchSpecular3;
vec3 pointTorchAmbient3;

uniform bool fogEnabled; 
uniform vec4 fogColor;  
uniform float fogDensity= 0.15f;;

void computeDirectionalLightComponents() {
    vec3 cameraPosEye = vec3(0.0f); 
    vec3 normalEye = normalize(fNormal);
    vec3 lightDirN = normalize(lightDir);
    vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

    ambient = ambientStrength * lightColor;

    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;
}

void computePointLightComponents() {
    vec3 normalEye = normalize(fNormal);
    vec3 lightDirToPoint = pointLightPosEye - fPosEye.xyz; 
    float distance = length(lightDirToPoint);
    vec3 lightDirN = normalize(lightDirToPoint);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance));

    pointLightAmbient = ambientStrength * pointLightColor;

    pointLightDiffuse = max(dot(normalEye, lightDirN), 0.0f) * pointLightColor;

    vec3 viewDirN = normalize(-fPosEye.xyz);
    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    pointLightSpecular = specularStrength * specCoeff * pointLightColor;

    pointLightAmbient *= attenuation;
    pointLightDiffuse *= attenuation;
    pointLightSpecular *= attenuation;
}

void computePointTortaComponents() {
    vec3 normalEye = normalize(fNormal);
    vec3 lightDirToTorch = pointTorchPosEye - fPosEye.xyz; 
    float distance = length(lightDirToTorch);
    vec3 lightDirN = normalize(lightDirToTorch);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance));

    pointTorchAmbient = ambientStrength * pointTorchColor;

    pointTorchDiffuse = max(dot(normalEye, lightDirN), 0.0f) * pointTorchColor;

    vec3 viewDirN = normalize(-fPosEye.xyz);
    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    pointTorchSpecular = specularStrength * specCoeff * pointTorchColor;

    pointTorchAmbient *= attenuation;
    pointTorchDiffuse *= attenuation;
    pointTorchSpecular *= attenuation;
}

void computePointTortaComponents2() {
    vec3 normalEye = normalize(fNormal);
    vec3 lightDirToTorch2 = pointTorchPosEye2 - fPosEye.xyz; 
    float distance = length(lightDirToTorch2);
    vec3 lightDirN = normalize(lightDirToTorch2);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance));

    pointTorchAmbient2 = ambientStrength * pointTorchColor2;

    pointTorchDiffuse2 = max(dot(normalEye, lightDirN), 0.0f) * pointTorchColor2;

    vec3 viewDirN = normalize(-fPosEye.xyz);
    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    pointTorchSpecular2 = specularStrength * specCoeff * pointTorchColor2;

    pointTorchAmbient2 *= attenuation;
    pointTorchDiffuse2 *= attenuation;
    pointTorchSpecular2 *= attenuation;
}

void computePointTortaComponents3() {
    vec3 normalEye = normalize(fNormal);
    vec3 lightDirToTorch3 = pointTorchPosEye3 - fPosEye.xyz;
    float distance = length(lightDirToTorch3);
    vec3 lightDirN = normalize(lightDirToTorch3);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance));

    pointTorchAmbient3 = ambientStrength * pointTorchColor3;

    pointTorchDiffuse3 = max(dot(normalEye, lightDirN), 0.0f) * pointTorchColor3;

    vec3 viewDirN = normalize(-fPosEye.xyz);
    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    pointTorchSpecular3 = specularStrength * specCoeff * pointTorchColor3;

    pointTorchAmbient3 *= attenuation;
    pointTorchDiffuse3 *= attenuation;
    pointTorchSpecular3 *= attenuation;
}






float computeFog()
{
 
 float fragmentDistance = length(fPosEye.xyz);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

 return clamp(fogFactor, 0.0f, 1.0f);
}



void main() {
    vec3 finalAmbient = vec3(0.0);
    vec3 finalDiffuse = vec3(0.0);
    vec3 finalSpecular = vec3(0.0);


    computeDirectionalLightComponents();
    finalAmbient += ambient;
    finalDiffuse += diffuse;
    finalSpecular += specular;

    if (!isTorch && pointLightEnabled) {
        computePointLightComponents();
        finalAmbient += pointLightAmbient;
        finalDiffuse += pointLightDiffuse;
        finalSpecular += pointLightSpecular;
    }

     if (isTorta && pointTorchEnabled) {
        computePointTortaComponents();
        finalAmbient += pointTorchAmbient;
        finalDiffuse += pointTorchDiffuse;
        finalSpecular += pointTorchSpecular;
    }


    
     if (isTorta2 && pointTorchEnabled2) {
        computePointTortaComponents2();
        finalAmbient += pointTorchAmbient2;
        finalDiffuse += pointTorchDiffuse2;
        finalSpecular += pointTorchSpecular2;
    }

    
     if (isTorta && pointTorchEnabled3) {
        computePointTortaComponents3();
        finalAmbient += pointTorchAmbient3;
        finalDiffuse += pointTorchDiffuse3;
        finalSpecular += pointTorchSpecular3;
    }

    vec4 texColor = texture(diffuseTexture, fTexCoords);

    if (texColor.a < 0.1) {
        discard;
    }

    vec3 baseColor = texColor.rgb;
    finalAmbient *= baseColor;
    finalDiffuse *= baseColor;
    finalSpecular *= texture(specularTexture, fTexCoords).rgb;


    vec3 color = min((finalAmbient + 1.0f  * finalDiffuse) + 1.0f * finalSpecular, 1.0f);

    float finalAlpha = 1.0; 
   if (isGlass) {

     color = mix(color, glassColor, 0.5); 
    finalAlpha = alpha;

}
    if (fogEnabled) {
        float fogFactor = computeFog();
        color = mix(fogColor.rgb, color, fogFactor);
    }

    fColor = vec4(color, finalAlpha); 
}
