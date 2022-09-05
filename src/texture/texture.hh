#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "rgb.hh"
#include "point.hh"

#include <cmath>
#include <map>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

gl::GLfloat *rasterize_with_texture(const unsigned char *image,int width,int height,size_t &cnt);
rgb map_to_tex(const unsigned char *image,int width,int height,float theta,float phi);

#endif //TEXTURE_HH
