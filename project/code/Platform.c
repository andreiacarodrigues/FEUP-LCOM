#include "Platform.h"

Platform* new_platform(double ix, double iy, int ix_size, int iy_size)
{
	Platform* ret_platform = (Platform*) malloc(sizeof(Platform));

	ret_platform->x = ix;
	ret_platform->y = iy;
	ret_platform->x_size = ix_size;
	ret_platform->y_size = iy_size;

	return ret_platform;
}

void check_collision_platform(Player * myPlayer, Platform * p)
{
	/* Check Vertically */
	if((myPlayer->y < p->y - myPlayer->y_size) || myPlayer->y > (p->y + p->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < p->x - myPlayer->x_size || myPlayer->x > p->x + p->x_size)
		return;

	/* We have a collision */

	// A ou C
	if(myPlayer->x > p->x && myPlayer->x + myPlayer->x_size < p->x + p->x_size)
	{
		if(myPlayer->vertical_speed > 0)
		// A
		{
			myPlayer->y = p->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		// C
		{
			myPlayer->y = p->y + p->y_size;
		}

		myPlayer->vertical_speed = 0;
		return;
	}

	// B ou D

	if(myPlayer->y > p->y && myPlayer->y + myPlayer->y_size < p->y + p->y_size)
	{
		if(myPlayer->player_movement == 2)
		// B
		{
			myPlayer->x = p->x - myPlayer->x_size;
		}
		else if (myPlayer->player_movement == 0)
		// D
		{
			myPlayer->x = p->x + p->x_size;
		}

		return;
	}

	// E
	if(myPlayer->x < p->x && myPlayer->y < p->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = p->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - p->x) > (myPlayer->y + myPlayer->y_size - p->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = p->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = p->x - myPlayer->x_size;
		}
		return;
	}

	// F
	if(myPlayer->x > p->x && myPlayer->y < p->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = p->x + p->x_size;
			return;
		}

		if ((p->x + p->x_size - myPlayer->x) > (myPlayer->y + myPlayer->y_size - p->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = p->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = p->x + p->x_size;
		}
		return;
	}

	// G
	if(myPlayer->x < p->x && myPlayer->y > p->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = p->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - p->x) > (p->y + p->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = p->y + p->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = p->x - myPlayer->x_size;
		}
		return;
	}

	// H
	if(myPlayer->x > p->x && myPlayer->y > p->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = p->x + p->x_size;
			return;
		}

		if ((p->x + p->x_size - myPlayer->x) > (p->y + p->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = p->y + p->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = p->x + p->x_size;
		}
			return;
	}
}
