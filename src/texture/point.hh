#ifndef POINT_HH
#define POINT_HH

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

class point
{
	public:
		gl::GLfloat x;
		gl::GLfloat y;
		gl::GLfloat z;
		point(float xx,float yy,float zz):x(xx),y(yy),z(zz) {}
};
bool lessf(const point &p1,const point &p2);

#endif //POINT_HH
