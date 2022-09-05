#ifndef FILL_HH
#define FILL_HH

#include "line.hh"
#include "point.hh"

#include <cstdlib>
#include <iostream>
#include <set>
#include <deque>
#include <initializer_list>
#include <algorithm>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

gl::GLint *fill_poly(std::initializer_list<point> il,size_t &cnt);
void add_edge_points(const std::initializer_list<point>::iterator &it1, const std::initializer_list<point>::iterator &it2,std::set<point,decltype(lessf)*> &points);

#endif //FILL_HH
