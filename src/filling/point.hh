#ifndef POINT_HH
#define POINT_HH

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

class point
{
	public:
		gl::GLint x;
		gl::GLint y;
		point(int xx,int yy):x(xx),y(yy) {}
		bool operator==(const point &another) const {
			return x == another.x && y == another.y;
		}
};
bool lessf(const point &p1,const point &p2);

#endif //POINT_HH
