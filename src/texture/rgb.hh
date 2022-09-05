#ifndef RGB_HH
#define RGB_HH

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

class rgb
{
	public:
		gl::GLfloat r;
		gl::GLfloat g;
		gl::GLfloat b;
		rgb(float rr,float gg,float bb) : r(rr),g(gg),b(bb) {}
		rgb() = default;
};

#endif //RGB_HH
