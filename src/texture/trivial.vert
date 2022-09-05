#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 texColor;

uniform mat4 M;
uniform mat4 M_cam;

const int windowWidth = 1920;
const int windowHeight = 1028;
const float depth = 2000;
const float l = - (windowWidth - 1) / 2.0f;
const float r = (windowWidth - 1) / 2.0f;
const float b = - (windowHeight - 1) / 2.0f;
const float t = (windowHeight - 1) / 2.0f;
const float n = 0;
const float f = -depth;
const mat4 M_ortho_proj = mat4(
	vec4(2.0f/(r - l),0.0f,0.0f,0.0f),
	vec4(0.0f,2.0f/(t - b),0.0f,0.0f),
	vec4(0.0f,0.0f,2.0f/(n - f),0.0f),
	vec4((r + l)/(l - r),(t + b)/(b - t),(n + f)/(f - n),1.0f)
);

void main()
{
	gl_Position = M_ortho_proj * M_cam * vec4(aPos,1.0f);
	texColor = aColor;
}
