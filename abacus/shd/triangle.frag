#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {

    vec4 imageColor = texture(texSampler, fragUV);
    outColor = imageColor;
}