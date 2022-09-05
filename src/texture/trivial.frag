#version 330 core
in vec3 texColor;
out vec4 color;
void main()
{
	color = vec4(texColor,1.0f);
}
