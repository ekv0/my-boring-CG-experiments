#include "line.hh"
using namespace gl;

GLint *line_rasterizer(int x0,int y0,int x1,int y1,size_t &cnt, void f(int,int,int,int,GLint*,bool,bool))
{
	if (x0 == x1) {
		if (y1 < y0)
			swapp(y0, y1);
		cnt = y1 - y0 + 1;
		auto points = new GLint[2*cnt];
		size_t i(0);
		for (int y(y0); y <= y1; ++y) {
			points[i++] = x0;
			points[i++] = y;
		}
		return points;
	}
	else if (x0 > x1) {
		swapp(x0,x1);
		swapp(y0,y1);
	}

	bool asc(true);
	if (y1 - y0 < 0) {
		asc = false;
	}

	GLint *points(nullptr);
	if (abs(y1 - y0) <= x1 - x0) {
		cnt = x1 - x0 + 1;
		points = new GLint[2*cnt];
		f(x0, x1, y0, y1, points, asc, false);
	}
	else {
		cnt = abs(y1 - y0) + 1;
		points = new GLint[2*cnt];
		if (asc) 
			f(y0, y1, x0, x1, points, asc, true);
		else
			f(y1, y0, x1, x0, points, asc, true);
	}
	return points;
}
void swapp(int &a,int &b)
{
	int tem(a);
	a = b;
	b = tem;
}
void mid_point(int ref_beg,int ref_end,int beg,int end,GLint *points,bool asc,bool reversed)
{
	int A(beg - end);
	int B(ref_end - ref_beg);
	int C(ref_beg * end - beg * ref_end);
	float d;
	if (asc) {
		d = A * (ref_beg + 1) + B * (beg + 0.5) + C;
	}
	else {
		d = A * (ref_beg + 1) + B * (beg - 0.5) + C;
	}

	size_t i(0);
	while (ref_beg <= ref_end) {
		if (!reversed) {
			points[i++] = ref_beg++;
			points[i++] = beg;
		}
		else {
			points[i++] = beg;
			points[i++] = ref_beg++;
		}
		if (d < 0) {
			if (asc) {
				++beg;
				d += A + B;
			}
			else {
				d += A;
			}
		}
		else {
			if (asc)
				d += A;
			else {
				--beg;
				d += A - B;
			}
		}
	}
}
