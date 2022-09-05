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

const unsigned windowWidth(1920);
const unsigned windowHeight(1028);

int main()
{
	auto w = initWindow();
	glbinding::initialize(glfwGetProcAddress);

	shader prog("trivial.vert","trivial.frag");

	const GLfloat vert[] = {
		400,300,0,
		1520,300,0,
		960,800,0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT),(void *)0);
	glEnableVertexAttribArray(0);

	//transformation to be tested
	const auto x_shift = (vert[0] + vert[3])/2;
	const auto y_shift = (vert[1] + vert[7])/2;
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
	M = glm::make_mat4(reverse_translation) * M;

	//updating uniform requires using program first
	prog.use();
	prog.setMat4("M", M);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(w))
	{
		if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(w, true);
		}

		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		prog.use();

		//orig. triangle
		prog.setBool("should", false);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//triangle transformed by M
		prog.setBool("should", true);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
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
