#include "Flag.h"

Flag* new_flag(double ix, double iy)
{
	Flag* flag = (Flag*) malloc(sizeof(Flag));

	flag->x = ix;
	flag->y = iy;

	return flag;
}


