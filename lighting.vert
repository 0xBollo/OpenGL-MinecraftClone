#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 modelMat;
uniform mat4 transformMat;



void main()
{
	FragPos = vec3(modelMat * vec4(aPos, 1.0f)); 
	Normal = aNormal;
	TexCoords = aTexCoords;
	gl_Position = transformMat * vec4(aPos, 1.0); 
}                      