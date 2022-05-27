#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec3 fragNorm;
layout(location = 3) in vec3 fragPos;
layout(set = 1, binding = 1) uniform sampler2D texSampler;

layout (set = 0, binding = 0) uniform CamUBO
{
    float time;
	vec3 pos;
} cam;

layout(location = 0) out vec4 outColor;

vec3 lightPos = vec3(0, 4, 0);
float lightIntensity = 1.2;
float lightRadius = 5;
vec3 lightColor = vec3(1);
float ambientStrength = 0.1;
float specularStrength = 0.5;

void main() {
    vec4 imageColor = texture(texSampler, fragUV) * vec4(fragColor, 1);

    lightPos.x = sin(cam.time);
    lightPos.z = cos(cam.time);

    // attenuation
    float dist = distance(fragPos, lightPos);
    float att = lightIntensity * clamp(lightRadius - sqrt(dist * dist), 0, 1);
    vec3 radiance = att * lightColor;

    //ambient
    vec3 ambient = ambientStrength * radiance;

    //diffuse
    vec3 norm = normalize(fragNorm);
    vec3 lightDir = normalize(lightPos - fragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * radiance;

    //specular
    vec3 viewDir = normalize(cam.pos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * radiance;

    outColor = vec4(ambient + diffuse + specular, 1) * imageColor;
}