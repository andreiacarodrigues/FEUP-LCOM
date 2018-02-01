#include "Camara.h"

void adjust_camara(Player * myPlayer, Platform* Platforms[], int nr_platforms, Coin* Coins[],int nr_coins, Spike* Spikes[], int nr_spikes, DestructableBox* DBox[], int nr_dboxes, Enemy* Enemies[], int nr_enemies, Box* Boxes[], int nr_boxes, Flag* flag, Cloud* Clouds[], int nr_clouds)
{
	/* Reset the player to his initial position */
	offset = myPlayer->x - initial_position;
	myPlayer->x = initial_position;

	/* Move the other objects */

	unsigned int i = 0;

	// Enemies
	//myEnemy->x -= offset;
	//myEnemy->left_bound -= offset;
	//myEnemy->right_bound -= offset;

	// Platforms
	for(i = 0; i < nr_platforms; i++)
	{
		Platform* p = Platforms[i];
		p->x -= offset;
	}

	// Coins
	for(i = 0; i < nr_coins; i++)
	{
		Coin* c = Coins[i];
		c->x -= offset;
	}

	// Spikes
	for(i = 0; i < nr_spikes; i++)
	{
		Spike* s = Spikes[i];
		s->x -= offset;
	}

	// Destructable Boxes
	for(i = 0; i < nr_dboxes; i++)
	{
		DestructableBox* d = DBox[i];
		d->x -= offset;
	}

	// Boxes
	for(i = 0; i < nr_boxes; i++)
	{
		Box* b = Boxes[i];
		b->x -= offset;
	}

	// Enemies
	for(i = 0; i < nr_enemies; i++)
	{
		Enemy* e = Enemies[i];
		e->x -= offset;
		e->left_bound -= offset;
		e->right_bound -= offset;
	}

	// Clouds
	for(i = 0; i < nr_clouds; i++)
	{
		Cloud* c = Clouds[i];
		c->x -= offset;
	}

	//Flag
	flag->x -=offset;
}
