#ifndef CLIP_HH
#define CLIP_HH

#include <vector>
#include "point.hh"

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

std::vector<point> clip(const point &lb,const point &rt,const point &p1,const point &p2);

#endif //CLIP_HH
