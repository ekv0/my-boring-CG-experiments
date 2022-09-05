#include "point.hh"
bool lessf(const point &p1,const point &p2) {
	if (p1.x < p2.x) {
		return true;
	}
	else if (p1.x == p2.x) {
		if (p1.y < p2.y) {
			return true;
		}
		else if (p1.y == p2.y) {
			return p1.z < p2.z;
		}
		return false;
	}
	return false;
}

