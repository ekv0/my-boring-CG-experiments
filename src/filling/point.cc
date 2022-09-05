#include "point.hh"
bool lessf(const point &p1,const point &p2) {
	if (p1.x < p2.x) {
		return true;
	}
	else if (p1.x == p2.x) {
		return p1.y < p2.y;
	}
	return false;
}

