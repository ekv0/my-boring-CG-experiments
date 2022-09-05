#include "shader.hh"
#include "point.hh"
#include "fill.hh"

#include <iostream>

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

	size_t cnt;
	const auto poly = fill_poly({
			point(840,600),
			point(900,540),
			point(840,700),
			point(740,540)
	},cnt);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	GLuint vbo;
	glGenBuffers(1,&vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT)*cnt*2, poly, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2*sizeof(GL_INT),static_cast<void *>(0));
	glEnableVertexAttribArray(0);
	delete [] poly;

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

		glBindVertexArray(vao);
		//draw as points as to simulate rasterization
		glDrawArrays(GL_POINTS, 0, cnt);

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
