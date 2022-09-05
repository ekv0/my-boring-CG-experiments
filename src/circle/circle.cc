#include "circle.hh"
#include <cmath>
using namespace gl;

GLint *circle_rasterizer_mid_point(int x0,int y0,int r,size_t &cnt)
{
#define ADD_POINT(X,Y) points[i++]=X;points[i++]=Y;
	cnt = static_cast<int>(r / pow(2, 0.5)) * 8 + 4;
	//reserve for the possible extra 4 points
	auto points = new GLint[2*(cnt+4)];

	size_t i(0);
	//when points fall on axes
	ADD_POINT(x0,y0 + r);
	ADD_POINT(x0 + r,y0);
	ADD_POINT(x0,y0 - r);
	ADD_POINT(x0 - r,y0);

	float d(4.25f-r);
	GLint x(1),y((d<=0)? r : r-1);
	//when points fall between axes
	while (x < y) {
		//original point
		ADD_POINT(x + x0,y + y0);
		//reflect along y = x
		ADD_POINT(y + x0,x + y0);
		//along x axis
		ADD_POINT(x + x0,-y + y0);
		ADD_POINT(y + x0,-x + y0);
		//along y axis
		ADD_POINT(-x + x0,y + y0);
		ADD_POINT(-y + x0,x + y0);
		ADD_POINT(-x + x0,-y + y0);
		ADD_POINT(-y + x0,-x + y0);
		if (d <= 0) {
			d += 2 * x + 3;
			++x;
		}
		else {
			d += 2 * (x - y) + 5;
			++x;
			--y;
		}
	}
	//when the last point to calc. happens to fall on y = x
	if (x == y) {
		cnt += 4;
		ADD_POINT(x + x0,y + y0);
		ADD_POINT(x + x0,-y + y0);
		ADD_POINT(-x + x0,y + y0);
		ADD_POINT(-x + x0,-y + y0);
	}
	return points;
}
