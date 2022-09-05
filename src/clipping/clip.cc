#include "clip.hh"

using namespace std;
using namespace gl;

vector<point> clip(const point &lb,const point &rt,const point &p1,const point &p2)
{
	const auto dx = p2.x - p1.x;
	const auto dy = p2.y - p1.y;
	const GLint Ps[] = {
		-dx,dx,-dy,dy
	};
	const GLint Qs[] = {
		p1.x - lb.x,
		rt.x - p1.x,
		p1.y - lb.y,
		rt.y - p1.y
	};
	if (Ps[0] == 0 || Ps[2] == 0) {
		for (const auto q : Qs) {
			if (q >= 0) {
				if (Ps[0] == 0) {
					const point p1_(Ps[0],lb.y);
					const point p2_(Ps[0],rt.y);
					return {p1_,p2_};
				}
				else {
					const point p1_(lb.x,Ps[2]);
					const point p2_(rt.x,Ps[2]);
					return {p1_,p2_};
				}
			}
			else {
				return {};
			}
		}
	}
	GLfloat u1(0),u2(1);
	for (size_t i(0); i != 4; ++i) {
		GLfloat temp;
		if (Ps[i] < 0 && (temp = static_cast<GLfloat>(Qs[i]) / Ps[i]) > u1) {
			u1 = temp;
		}
		else if (Ps[i] > 0 && (temp = static_cast<GLfloat>(Qs[i]) / Ps[i]) < u2) {
			u2 = temp;
		}
	}
	if (u1 >= u2) {
		return {};
	}
	const point p1_(p1.x + dx * u1,p1.y + dy * u1);
	const point p2_(p1.x + dx * u2,p1.y + dy * u2);
	return {p1_,p2_};
}

