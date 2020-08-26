// Flat Colour Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ViewProjectionMatrix *  u_ModelMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColour;

uniform vec4 u_Colour;

void main()
{
	FragColour = u_Colour;
} 