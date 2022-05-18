@echo off

echo Compiling shaders to SPIR-V...

C:/VulkanSDK/1.3.211.0/Bin/glslc.exe triangle.vert -o triangle_vert.spv
C:/VulkanSDK/1.3.211.0/Bin/glslc.exe triangle.frag -o triangle_frag.spv

echo Shaders compiled to SPIR-V successfully!