#include "Boxes.h"

Box* new_box(double ix, double iy, int ix_size, int iy_size)
{
	Box* box = (Box*) malloc(sizeof(Box));

	box->x = ix;
	box->y = iy;
	box->x_size = ix_size;
	box->y_size = iy_size;

	return box;
}

DestructableBox* new_dbox(double ix, double iy, int ix_size, int iy_size)
{
	DestructableBox* box = (DestructableBox*) malloc(sizeof(DestructableBox));

	box->x = ix;
	box->y = iy;
	box->x_size = ix_size;
	box->y_size = iy_size;
	box->active = 1;

	return box;
}

void check_collision_box(Player * myPlayer, Box * b)
{
	/* Check Vertically */
	if((myPlayer->y < b->y - myPlayer->y_size) || myPlayer->y > (b->y + b->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < b->x - myPlayer->x_size || myPlayer->x > b->x + b->x_size)
		return;

	/* We have a collision */

	// A ou C
	if(myPlayer->x > b->x && myPlayer->x + myPlayer->x_size < b->x + b->x_size)
	{
		if(myPlayer->vertical_speed > 0)
		// A
		{
			myPlayer->y = b->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		// C
		{
			myPlayer->y = b->y + b->y_size;
		}

		myPlayer->vertical_speed = 0;
		return;
	}

	// B ou D

	if(myPlayer->y >= b->y && myPlayer->y + myPlayer->y_size <= b->y + b->y_size)
	{
		if(myPlayer->player_movement == 2)
		// B
		{
			myPlayer->x = b->x - myPlayer->x_size;
		}
		else if (myPlayer->player_movement == 0)
		// D
		{
			myPlayer->x = b->x + b->x_size;
		}

		return;
	}

	// E
	if(myPlayer->x < b->x && myPlayer->y < b->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = b->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - b->x) > (myPlayer->y + myPlayer->y_size - b->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = b->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = b->x - myPlayer->x_size;
		}
		return;
	}

	// F
	if(myPlayer->x > b->x && myPlayer->y < b->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = b->x + b->x_size;
			return;
		}

		if ((b->x + b->x_size - myPlayer->x) > (myPlayer->y + myPlayer->y_size - b->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = b->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = b->x + b->x_size;
		}
		return;
	}

	// G
	if(myPlayer->x < b->x && myPlayer->y > b->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = b->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - b->x) > (b->y + b->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = b->y + b->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = b->x - myPlayer->x_size;
		}
		return;
	}

	// H
	if(myPlayer->x > b->x && myPlayer->y > b->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = b->x + b->x_size;
			return;
		}

		if ((b->x + b->x_size - myPlayer->x) > (b->y + b->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = b->y + b->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = b->x + b->x_size;
		}
			return;
	}
}

void check_collision_dbox(Player * myPlayer, DestructableBox * db)
{
	/* Check Vertically */
	if((myPlayer->y < db->y - myPlayer->y_size) || myPlayer->y > (db->y + db->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < db->x - myPlayer->x_size || myPlayer->x > db->x + db->x_size)
		return;

	/* We have a collision */

	// A ou C
	if(myPlayer->x > db->x && myPlayer->x + myPlayer->x_size < db->x + db->x_size)
	{
		if(myPlayer->vertical_speed > 0)
		// A
		{
			myPlayer->y = db->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		// C
		{
			myPlayer->y = db->y + db->y_size;
		}

		myPlayer->vertical_speed = 0;
		return;
	}

	// B ou D

	if(myPlayer->y >= db->y && myPlayer->y + myPlayer->y_size <= db->y + db->y_size)
	{
		if(myPlayer->player_movement == 2)
		// B
		{
			myPlayer->x = db->x - myPlayer->x_size;
		}
		else if (myPlayer->player_movement == 0)
		// D
		{
			myPlayer->x = db->x + db->x_size;
		}

		return;
	}

	// E
	if(myPlayer->x < db->x && myPlayer->y < db->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = db->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - db->x) > (myPlayer->y + myPlayer->y_size - db->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = db->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = db->x - myPlayer->x_size;
		}
		return;
	}

	// F
	if(myPlayer->x > db->x && myPlayer->y < db->y)
	{
		if(myPlayer->vertical_speed < 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = db->x + db->x_size;
			return;
		}

		if ((db->x + db->x_size - myPlayer->x) > (myPlayer->y + myPlayer->y_size - db->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = db->y - myPlayer->y_size;
			myPlayer->grounded = 1;
			myPlayer->horizontal_effect = 0;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = db->x + db->x_size;
		}
		return;
	}

	// G
	if(myPlayer->x < db->x && myPlayer->y > db->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = db->x - myPlayer->x_size;
			return;
		}

		if ((myPlayer->x + myPlayer->x_size - db->x) > (db->y + db->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = db->y + db->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = -0.0001;
			myPlayer->x = db->x - myPlayer->x_size;
		}
		return;
	}

	// H
	if(myPlayer->x > db->x && myPlayer->y > db->y)
	{
		if(myPlayer->vertical_speed > 0)
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = db->x + db->x_size;
			return;
		}

		if ((db->x + db->x_size - myPlayer->x) > (db->y + db->y_size - myPlayer->y))
		{
			myPlayer->vertical_speed = 0;
			myPlayer->y = db->y + db->y_size;
		}
		else
		{
			myPlayer->vertical_speed = 0;
			myPlayer->horizontal_effect = 0.0001;
			myPlayer->x = db->x + db->x_size;
		}
			return;
	}
}

void check_pressed_dbox(int x, int y, int pressed, DestructableBox * db)
{
	if(!pressed)
		return;

	if(x > db->x && x < db->x + db->x_size)
		if(y > db->y && y < db->y + db->y_size)
			db->active = 0;

	return;
}
