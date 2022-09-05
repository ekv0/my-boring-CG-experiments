#include "shader.hh"
#include "line.hh"

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

	size_t cnt[6];
	GLint *lines[] = {
		line_rasterizer(960,0,960,1028,cnt[0]),
		line_rasterizer(0,514,1920,514,cnt[1]),
		line_rasterizer(960,514,1200,1028,cnt[2]),
		line_rasterizer(1920,800,960,514,cnt[3]),
		line_rasterizer(1920,230,960,514,cnt[4]),
		line_rasterizer(1200,0,960,514,cnt[5])
	};

	GLuint vao[6];
	glGenVertexArrays(6, vao);
	GLuint vbo[6];
	glGenBuffers(6,vbo);

	for (size_t i(0); i != 6; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT)*cnt[i]*2, lines[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2*sizeof(GL_INT),static_cast<void *>(0));
		glEnableVertexAttribArray(0);

		delete lines[i];
	}

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

		for (size_t i(0); i != 6; ++i) {
			//bind to different sets of points of lines
			glBindVertexArray(vao[i]);
			//draw as points as to simulate rasterization
			glDrawArrays(GL_POINTS, 0, cnt[i]);
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
