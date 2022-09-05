#include "texture.hh"

using namespace gl;
using namespace std;

GLfloat *rasterize_with_texture(const unsigned char *image,int width,int height,size_t &cnt)
{
	//rasterize a sphere with radius 300 and center $\vec{o}$
	const float r(300);
	//control the grainularities in 2 directions
	const float delta_phi(glm::radians(0.2f));
	const float delta_theta(glm::radians(0.2f));

	map<point, rgb, decltype(lessf)*> points(lessf);
	for (float phi = 0; phi < glm::radians(360.0f); phi += delta_phi) {
		for (float theta = 0; theta < glm::radians(180.0f); theta += delta_theta) {
			const point p(
				r * sin(theta) * cos(phi),
				r * sin(theta) * sin(phi),
				r * cos(theta)
			);
			//map to texture coordinate from spherical coord.
			points[p] = map_to_tex(image,width,height,theta,phi);
		}
	}

	cnt = points.size();
	auto retval = new GLfloat[6 * cnt];
	size_t i(0);

#define ADD(M) retval[i++]=M
#define ADD_POINT(X,Y,Z,R,G,B) ADD(X);ADD(Y);ADD(Z);ADD(R);ADD(G);ADD(B);
	
	for (const auto &p : points) {
		ADD_POINT(p.first.x, p.first.y, p.first.z, p.second.r, p.second.g, p.second.b);
	}

	return retval;
}
rgb map_to_tex(const unsigned char *image,int width,int height,float theta,float phi)
{
	//$A\cdot \theta_{max} = \text{height}$
	const int A(height / glm::radians(180.0f));
	//$B\cdot \frac{\phi_{max}}{2} = \text{width}$
	const int B(width / glm::radians(180.0f));

	const int row = static_cast<int>(A * theta) % height;
	const int col = static_cast<int>(B * phi) % width;
	const int pos = 3 * (row * width + col);
	return rgb(image[pos]/255.0,image[pos+1]/255.0,image[pos+2]/255.0);
}
