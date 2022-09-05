#version 330 core
layout (location = 0) in vec2 aPos;
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
	gl_Position = M_ortho_proj * vec4(aPos,0.0f,1.0f);
}
