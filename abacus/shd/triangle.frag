#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec3 fragNorm;
layout(location = 3) in vec3 fragPos;
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

vec3 lightDir = vec3(1.0, 0.0, 0.1);
vec4 lightColor = vec4(1);
float ambientStrength = 0.2;
float specularStrength = 0.5;
vec3 camPos = vec3(5);

void main() {
    vec4 imageColor = texture(texSampler, fragUV);

    //ambient
    vec4 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(fragNorm);
    vec3 lightDir = normalize(lightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    //specular
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec4 specular = specularStrength * spec * lightColor;

    outColor = (ambient + diffuse + specular) * imageColor;
}