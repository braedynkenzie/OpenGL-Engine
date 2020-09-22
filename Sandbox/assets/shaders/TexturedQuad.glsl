// Textured Quad Shader

#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjectionMatrix;
//uniform mat4 u_ModelMatrix;

out vec4 v_Colour; 
out vec2 v_TexCoords; 
out float v_TexIndex; 
out float v_TilingFactor; 
			
void main() {
	v_Colour = a_Colour;
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	//gl_Position =  u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
	gl_Position =  u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec4 v_Colour; 
in vec2 v_TexCoords; 
in float v_TexIndex; 
in float v_TilingFactor; 

uniform sampler2D u_Textures[32];

out vec4 FragColour;

void main() {
	
	vec4 resultColour = v_Colour; 

	switch(int(v_TexIndex))
	{
		case 0:  resultColour *= texture(u_Textures[0],  v_TexCoords* v_TilingFactor); break;
		case 1:  resultColour *= texture(u_Textures[1],  v_TexCoords* v_TilingFactor); break;
		case 2:  resultColour *= texture(u_Textures[2],  v_TexCoords* v_TilingFactor); break;
		case 3:  resultColour *= texture(u_Textures[3],  v_TexCoords* v_TilingFactor); break;
		case 4:  resultColour *= texture(u_Textures[4],  v_TexCoords* v_TilingFactor); break;
		case 5:  resultColour *= texture(u_Textures[5],  v_TexCoords* v_TilingFactor); break;
		case 6:  resultColour *= texture(u_Textures[6],  v_TexCoords* v_TilingFactor); break;
		case 7:  resultColour *= texture(u_Textures[7],  v_TexCoords* v_TilingFactor); break;
		case 8:  resultColour *= texture(u_Textures[8],  v_TexCoords* v_TilingFactor); break;
		case 9:  resultColour *= texture(u_Textures[9],  v_TexCoords* v_TilingFactor); break;
		case 10: resultColour *= texture(u_Textures[10], v_TexCoords* v_TilingFactor); break;
		case 11: resultColour *= texture(u_Textures[11], v_TexCoords* v_TilingFactor); break;
		case 12: resultColour *= texture(u_Textures[12], v_TexCoords* v_TilingFactor); break;
		case 13: resultColour *= texture(u_Textures[13], v_TexCoords* v_TilingFactor); break;
		case 14: resultColour *= texture(u_Textures[14], v_TexCoords* v_TilingFactor); break;
		case 15: resultColour *= texture(u_Textures[15], v_TexCoords* v_TilingFactor); break;
		case 16: resultColour *= texture(u_Textures[16], v_TexCoords* v_TilingFactor); break;
		case 17: resultColour *= texture(u_Textures[17], v_TexCoords* v_TilingFactor); break;
		case 18: resultColour *= texture(u_Textures[18], v_TexCoords* v_TilingFactor); break;
		case 19: resultColour *= texture(u_Textures[19], v_TexCoords* v_TilingFactor); break;
		case 20: resultColour *= texture(u_Textures[20], v_TexCoords* v_TilingFactor); break;
		case 21: resultColour *= texture(u_Textures[21], v_TexCoords* v_TilingFactor); break;
		case 22: resultColour *= texture(u_Textures[22], v_TexCoords* v_TilingFactor); break;
		case 23: resultColour *= texture(u_Textures[23], v_TexCoords* v_TilingFactor); break;
		case 24: resultColour *= texture(u_Textures[24], v_TexCoords* v_TilingFactor); break;
		case 25: resultColour *= texture(u_Textures[25], v_TexCoords* v_TilingFactor); break;
		case 26: resultColour *= texture(u_Textures[26], v_TexCoords* v_TilingFactor); break;
		case 27: resultColour *= texture(u_Textures[27], v_TexCoords* v_TilingFactor); break;
		case 28: resultColour *= texture(u_Textures[28], v_TexCoords* v_TilingFactor); break;
		case 29: resultColour *= texture(u_Textures[29], v_TexCoords* v_TilingFactor); break;
		case 30: resultColour *= texture(u_Textures[30], v_TexCoords* v_TilingFactor); break;
		case 31: resultColour *= texture(u_Textures[31], v_TexCoords* v_TilingFactor); break;
	}

	if(resultColour.a == 0.0)
		discard;

	FragColour = resultColour;

	// Debugging
	//FragColour = vec4(v_TexCoords, 0.0, 1.0);
	//FragColour = vec4(v_TexIndex / 10.0, v_TexIndex / 10.0, v_TexIndex / 10.0, 1.0);
	//FragColour = vec4(v_TilingFactor / 10.0, v_TilingFactor / 10.0, v_TilingFactor / 10.0, 1.0);
	//FragColour = v_Colour;
	//FragColour = vec4(1.0);
}