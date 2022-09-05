#include "shader.hh"
#include "clip.hh"

#include <iostream>
#include <cstdlib>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

using namespace std;
using namespace gl;

GLFWwindow *initWindow();

int main()
{
	auto w = initWindow();
	glbinding::initialize(glfwGetProcAddress);

	shader prog("trivial.vert","trivial.frag");

	const GLint rect[] = {
		600,200,
		1200,200,
		1200,800,
		600,800
	};
	const GLuint indices[] = {
		0,1,1,2,2,3,3,0
	};
	const point p1(300,300),p2(900,900);

	GLuint vao[2];
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);

	GLuint vbo[2];
	glGenBuffers(2,vbo);
	//rectangle
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2*sizeof(GL_INT),(void *)0);
	glEnableVertexAttribArray(0);
	//clipping
	const auto a = clip(point(rect[0],rect[1]), point(rect[4],rect[5]), p1, p2);
	bool empty(a.size() == 0);
	if (!empty) {
		const GLint points[] = {
			a[0].x,a[0].y,
			a[1].x,a[1].y
		};
		glBindVertexArray(vao[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2*sizeof(GL_INT),(void *)0);
		glEnableVertexAttribArray(0);
	}

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	while(!glfwWindowShouldClose(w))
	{
		if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(w, true);
		}

		glClearColor(0.7, 0.7, 0.7, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		prog.use();

		glBindVertexArray(vao[0]);
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

		if (!empty) {
			glBindVertexArray(vao[1]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

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

	auto w = glfwCreateWindow(1920, 1028, "tAsK", nullptr, nullptr);
	if (!w) {
		std::cerr << "window creation failed" << std::endl;
		exit(-1);
	}
	glfwMakeContextCurrent(w);

	return w;
}
