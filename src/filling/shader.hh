#ifndef SHADER_HH
#define SHADER_HH

#include <string>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

class shader
{
	public:
		gl::GLuint id;

		shader(const char *vs_path,const char *fs_path);
		void use() const;
		void setBool(const std::string &name, bool value) const;  
		void setInt(const std::string &name, int value) const;   
		void setFloat(const std::string &name, float value) const;
	private:
		std::string read_file(const char *path) const;
		gl::GLuint compile(const gl::GLchar * const source,gl::GLenum type) const;
};

inline void shader::use() const
{
	gl::glUseProgram(id);
}

inline void shader::setBool(const std::string &name, bool value) const
{         
	gl::glUniform1i(gl::glGetUniformLocation(id, name.c_str()), (int)value); 
}

inline void shader::setInt(const std::string &name, int value) const
{ 
	gl::glUniform1i(gl::glGetUniformLocation(id, name.c_str()), value); 
}

inline void shader::setFloat(const std::string &name, float value) const
{ 
	gl::glUniform1f(gl::glGetUniformLocation(id, name.c_str()), value); 
} 

#endif //SHADER_HH
