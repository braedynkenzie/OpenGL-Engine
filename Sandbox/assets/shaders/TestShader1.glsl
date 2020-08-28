// Test Pulsing Colour Texture Shader

#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec4 v_Colour; 
out vec2 v_TexCoords; 
			
void main() {
	v_Colour = a_Colour;
	v_TexCoords = a_TexCoords;
	gl_Position =  u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec4 v_Colour;
in vec2 v_TexCoords; 

uniform vec4 u_PulseColour;
uniform float u_Blend;

uniform sampler2D u_Texture;

out vec4 FragColour;
			
void main() {
	vec4 albedo = texture(u_Texture, v_TexCoords);
	if(albedo.a == 0.0)
		discard;
	FragColour = u_Blend * albedo + (1 - u_Blend) * u_PulseColour;
}