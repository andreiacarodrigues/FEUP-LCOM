#include "Coins.h"

Coin* new_coin(double ix, double iy, int ix_size, int iy_size)
{
	Coin* coin = (Coin*) malloc(sizeof(Coin));

	coin->x = ix;
	coin->y = iy;
	coin->x_size = ix_size;
	coin->y_size = iy_size;
	coin->active = 1;

	return coin;
}

void coin_pickup(Player * myPlayer, Coin * c)
{
	/* Check Vertically */
	if((myPlayer->y < c->y - myPlayer->y_size) || myPlayer->y > (c->y + c->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < c->x - myPlayer->x_size || myPlayer->x > c->x + c->x_size)
		return;

	/* Destroy Coin and increment Score */
	c->active = 0;
	myPlayer->coins++;

	return;
}
