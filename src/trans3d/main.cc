#include "shader.hh"
#include <iostream>
#include <cmath>
//disable inclusion of the development environment header
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
//glbinding
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace gl;

GLFWwindow *initWindow();
glm::mat4 gen_M(const glm::vec3 &center);
glm::mat4 gen_M_cam(const glm::vec3 &center);

const unsigned windowWidth(1920);
const unsigned windowHeight(1028);

int main()
{
	auto wd = initWindow();
	glbinding::initialize(glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);  

	shader prog("trivial.vert","trivial.frag");

	const GLfloat left(-300),right(300),bottom(-250),top(250),near(-100),far(-600);
	const GLfloat vert[] = {
		left,bottom,near,
		right,bottom,near,
		right,top,near,
		left,top,near,

		left,bottom,far,
		right,bottom,far,
		right,top,far,
		left,top,far,
	};
	auto center = glm::vec3((left+right)/2,(top+bottom)/2,(near+far)/2);
	const GLuint indices[] = {
		0,1,1,2,2,3,3,0,	//near plane
		4,5,5,6,6,7,7,4,	//far plane
		0,4,7,3,	//left plane
		1,5,6,2		//right plane
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT),(void *)0);
	glEnableVertexAttribArray(0);

	//generate transformation matrix to be tested
	const auto M = gen_M(center);

	//updating uniform requires using program first
	prog.use();
	prog.setMat4("M", M);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(wd))
	{
		if (glfwGetKey(wd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(wd, true);
		}

		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//get new camera transformation
		const auto M_cam = gen_M_cam(center);

		prog.use();
		prog.setMat4("M_cam", M_cam);

		//orig. cuboid
		prog.setBool("should", false);
		glBindVertexArray(vao);
		//draw lines according to indices of points
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		//cuboid transformed by M
		prog.setBool("should", true);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(wd);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
}
glm::mat4 gen_M(const glm::vec3 &center)
{
	//scale x & y coord. down to 0.3 then rotate -45.0 deg. along z axis
	const auto x_shift = center.x;
	const auto y_shift = center.y;
	const float translation[] = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-x_shift,-y_shift,0.0f,1.0f
	};
	auto M = glm::make_mat4(translation);

	const float angle = glm::radians(-45.0);
	const float rotation[] = {
		cos(angle),sin(angle),0.0f,0.0f,
		-sin(angle),cos(angle),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	M = glm::make_mat4(rotation) * M;

	const float x_scale(0.3);
	const float y_scale(0.3);
	const float scaling[] = {
		x_scale,0.0f,0.0f,0.0f,
		0.0f,y_scale,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	M = glm::make_mat4(scaling) * M;

	const float reverse_translation[] = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		x_shift,y_shift,0.0f,1.0f
	};
	return glm::make_mat4(reverse_translation) * M;
}
glm::mat4 gen_M_cam(const glm::vec3 &center)
{
	//eye position
	const float radius = 500;
	const auto time = glfwGetTime();
	//eye rotates along a circle whose plane is perpendicular to y axis
	const auto e = glm::vec3(
		center.x + radius*sin(time),
		center.y + 200,
		center.z + radius*cos(time)
	);
	//gaze direction
	const auto target = center;
	const auto g = target - e;	//always gaze at the center of the cuboid
	//view-up vector
	const auto t = glm::vec3(0,1,0);
	//construct $(\vec{u},\vec{v},\vec{w})$ basis
	const auto w = - glm::normalize(g);
	const auto u = glm::normalize(glm::cross(t, w));
	const auto v = glm::cross(w, u);
	//matrix to align $(\vec{u},\vec{v},\vec{w})$ to $(\vec{x},\vec{y},\vec{z})$
	glm::mat4 M_cam_rot(
		glm::vec4(u,0),	//first col
		glm::vec4(v,0),	//second col
		glm::vec4(w,0),	//third col
		glm::vec4(0,0,0,1)	//translation col
	);
	M_cam_rot = glm::transpose(M_cam_rot);
	//matrix to translate $\vec{e}$ to $\vec{o}$
	const glm::mat4 M_cam_transl(
		glm::vec4(1,0,0,0),
		glm::vec4(0,1,0,0),
		glm::vec4(0,0,1,0),
		glm::vec4(-e,1)
	);
	//construct M_cam
	return M_cam_rot * M_cam_transl;
}
GLFWwindow *initWindow()
{
	if (!glfwInit()) {
		std::cerr << "init failed." << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto w = glfwCreateWindow(windowWidth, windowHeight, "tAsK", nullptr, nullptr);
	if (!w) {
		std::cerr << "window creation failed" << std::endl;
		exit(-1);
	}
	glfwMakeContextCurrent(w);

	return w;
}
