#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec2 inUV;

layout (binding = 0) uniform UniformBufferObject
{
	mat4 pvm;
	mat4 model;
	mat4 normal;
} ubo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;
layout(location = 2) out vec3 fragNorm;
layout(location = 3) out vec3 fragPos;

void main() {
	gl_Position = ubo.pvm * vec4(inPosition, 1.0);
	fragColor = inColor;
	fragUV = inUV;
	fragNorm = (ubo.normal * vec4(inNorm, 0.0)).xyz;
	fragPos = (ubo.model * vec4(inPosition, 1)).xyz;
}