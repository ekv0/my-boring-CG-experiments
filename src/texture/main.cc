#include "shader.hh"
#include "point.hh"
#include "rgb.hh"
#include "texture.hh"

#include <iostream>
//disable inclusion of the development environment header
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
//glbinding
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
//image loading
#include <stb/stb_image.h>

using namespace std;
using namespace gl;

GLFWwindow *initWindow();
glm::mat4 gen_M_cam(const glm::vec3 &center);

const unsigned windowWidth(1920);
const unsigned windowHeight(1028);

int main()
{
	auto wd = initWindow();
	glbinding::initialize(glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);

	shader prog("trivial.vert","trivial.frag");

	//load image data
	int width,height,channel;
	const auto image = stbi_load("wall.jpg", &width, &height, &channel, 0);

	//rasterize a sphere with texture
	size_t cnt;
	const auto vert = rasterize_with_texture(image,width,height,cnt);
	stbi_image_free(image);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*cnt*6, vert, GL_STATIC_DRAW);
	delete [] vert;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT),(void *)0);
	glEnableVertexAttribArray(0);
	//every 3D point comes with RGB color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT),(void *)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(wd))
	{
		if (glfwGetKey(wd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(wd, true);
		}

		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//get new camera transformation, center at $\vec{o}$
		const auto M_cam = gen_M_cam(glm::vec3(0,0,0));

		prog.use();
		prog.setMat4("M_cam", M_cam);

		glBindVertexArray(vao);
		//draw as points as to simulate the process of texturing
		glDrawArrays(GL_POINTS, 0, cnt);

		glfwSwapBuffers(wd);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
}
glm::mat4 gen_M_cam(const glm::vec3 &center)
{
	//eye position
	const float radius = 400;
	const auto time = glfwGetTime();
	//eye rotates along a circle whose plane is perpendicular to y axis
	const auto e = glm::vec3(
		center.x + radius*sin(time),
		center.y,
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
