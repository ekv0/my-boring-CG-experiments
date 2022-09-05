#include "shader.hh"
#include <iostream>
#include <string>
#include <fstream>
#include <strstream>
#include <cstdlib>

using namespace std;
using namespace gl;

shader::shader(const char *vs_path,const char *fs_path)
{
	//read source and compile
	auto vertexSource = read_file(vs_path);
	GLuint vertexShader = compile(vertexSource.c_str(), GL_VERTEX_SHADER);

	auto fragmentSource = read_file(fs_path);
	GLuint fragmentShader = compile(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	//program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	//link
	glLinkProgram(id);
	GLboolean success(false);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char mess[512];
		glGetProgramInfoLog(id, 512, nullptr, mess);
		cerr << "link error" << endl << mess << endl;
		exit(-1);
	}

	//delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

string shader::read_file(const char *path) const
{
	ifstream is(path);
	if (!is) {
		cerr << "cannot open file " << path << endl;
		exit(-1);
	}
	ostrstream code;
	code << is.rdbuf();
	return code.str();
}

GLuint shader::compile(const GLchar * const source,gl::GLenum type) const
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	GLboolean success(false);
	glGetShaderiv(id,GL_COMPILE_STATUS,&success);
	if (!success) {
		char mess[512];
		glGetShaderInfoLog(id, 512, nullptr, mess);
		cerr << "compile error\n" << mess << endl;
		exit(-1);
	}

	return id;
}
