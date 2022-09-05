#ifndef LINE_HH
#define LINE_HH

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

void mid_point(int ref_beg,int ref_end,int beg,int end,gl::GLint *points,bool asc,bool reversed);
gl::GLint *line_rasterizer(int x0,int y0,int x1,int y1,size_t &cnt, void f(int,int,int,int,gl::GLint*,bool,bool) = mid_point);
void swapp(int &a,int &b);

#endif //LINE_HH
