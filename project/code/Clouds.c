#include "Clouds.h"

Cloud* new_cloud(double ix, double iy)
{
	Cloud* cloud = (Cloud*) malloc(sizeof(Cloud));

	cloud->x = ix;
	cloud->y = iy;

	return cloud;
}
