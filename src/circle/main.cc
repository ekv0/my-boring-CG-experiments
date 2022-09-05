#include "shader.hh"
#include "circle.hh"

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
	GLint *circles[] = {
		circle_rasterizer_mid_point(0, 0, 90, cnt[0]),
		circle_rasterizer_mid_point(0, 0, 160, cnt[1]),
		circle_rasterizer_mid_point(0, 0, 260, cnt[2]),
		circle_rasterizer_mid_point(1920, 1028, 400, cnt[3]),
		circle_rasterizer_mid_point(600, 800, 200, cnt[4]),
		circle_rasterizer_mid_point(960, 514, 300, cnt[5])
	};

	GLuint vao[6];
	glGenVertexArrays(6, vao);
	GLuint vbo[6];
	glGenBuffers(6,vbo);

	for (size_t i(0); i != 6; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT)*cnt[i]*2, circles[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2*sizeof(GL_INT),static_cast<void *>(0));
		glEnableVertexAttribArray(0);

		delete circles[i];
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
			//bind to different sets of points of circles
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
