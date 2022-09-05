#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 M;
uniform bool should;

const int windowWidth = 1920;
const int windowHeight = 1028;
const mat4 M_ortho_proj = mat4(
	2.0f/(windowWidth-1),0.0f,0.0f,0.0f,
	0.0f,2.0f/(windowHeight-1),0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	-1.0f,-1.0f,-1.0f,1.0f
);

void main()
{
	if (should)
		gl_Position = M * vec4(aPos,1.0f);
	else
		gl_Position = vec4(aPos,1.0f);
	gl_Position = M_ortho_proj * gl_Position;
}
