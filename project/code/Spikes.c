#include "Spikes.h"

Spike* new_spikes(double ix, double iy, int ix_size, int iy_size)
{
	Spike* spikes = (Spike*) malloc(sizeof(Spike));

	spikes->x = ix;
	spikes->y = iy;
	spikes->x_size = ix_size;
	spikes->y_size = iy_size;

	return spikes;
}

void check_collision_spikes(Player * myPlayer, Spike * s)
{

	/* Check Vertically */
	if((myPlayer->y < s->y - myPlayer->y_size) || myPlayer->y > (s->y + s->y_size + myPlayer->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < s->x - myPlayer->x_size || myPlayer->x > s->x + myPlayer->x_size)
		return;

	/* Collision */

	if(myPlayer->x > s->x)
		myPlayer->horizontal_effect = knockback_spike;
	else
		myPlayer->horizontal_effect = -knockback_spike;

	if(myPlayer->hp > 0)
		myPlayer->hp--;

	myPlayer->kick = 1;
	jump(myPlayer);

	return;
}
