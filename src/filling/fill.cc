#include "fill.hh"

using namespace gl;
using namespace std;

GLint *fill_poly(initializer_list<point> il,size_t &cnt)
{
	if (il.size() <= 2) {
		std::cerr << "insufficient number of points for polygon" << std::endl;
		exit(-1);
	}
	set<point,decltype(lessf)*> points(lessf);

	//add edge points & find a point inside
	point start_p(il.begin()->x,il.begin()->y);
	points.insert(point(il.begin()->x,il.begin()->y));
	for (auto it = il.begin() + 1; it != il.end(); ++it) {
		start_p.x += it->x;
		start_p.y += it->y;
		add_edge_points(it,it-1,points);
		points.insert(point(it->x,it->y));
	}
	add_edge_points(il.begin(), il.end()-1, points);

	start_p.x /= il.size();
	start_p.y /= il.size();

	//filling
	deque<point> stack(1,start_p);
	while (!stack.empty()) {
		auto p = stack.front();
		stack.pop_front();
		auto retval = points.insert(p);
		if (retval.second) {
			//4-adjacency, counter-clockwise
			if (find(stack.begin(), stack.end(), p) == stack.end())
				stack.push_front(point(p.x+1,p.y));
			if (find(stack.begin(), stack.end(), p) == stack.end())
				stack.push_front(point(p.x,p.y+1));
			if (find(stack.begin(), stack.end(), p) == stack.end())
				stack.push_front(point(p.x-1,p.y));
			if (find(stack.begin(), stack.end(), p) == stack.end())
				stack.push_front(point(p.x,p.y-1));
		}
	}

	//conversion to array
	cnt = points.size();
	auto arr = new GLint[2*cnt];
	size_t i(0);
	for (const auto &p : points) {
		arr[i++] = p.x;
		arr[i++] = p.y;
	}
	return arr;
}
void add_edge_points(const initializer_list<point>::iterator &it1, const initializer_list<point>::iterator &it2,set<point,decltype(lessf)*> &points)
{
	size_t cnt;
	auto line = line_rasterizer(it1->x, it1->y, it2->x, it2->y, cnt);
	for (size_t i = 0; i < 2*cnt-2; i+=2) {
		points.insert(point(line[i],line[i+1]));
	}
	delete [] line;
}
