#include "Character.h"

Player* initialize_player(int x_size, int y_size)
{
	Player* myPlayer = (Player*) malloc(sizeof(Player));

	myPlayer->x = initial_position;
	myPlayer->y = ground_level;

	myPlayer->horizontal_effect = 0;
	myPlayer->player_movement = 1;

	myPlayer->x_size = x_size;
	myPlayer->y_size = y_size;

	myPlayer->anim_position = 0;

	myPlayer->kick = 0;
	myPlayer->grounded = 0;
	myPlayer->vertical_speed = 0;

	myPlayer->coins = 0;
	myPlayer->hp = 3;
	myPlayer->score = 0;
	myPlayer->time = 0;

	return myPlayer;
}

void update_movement(int change, Player * myPlayer)
{
	myPlayer->player_movement = change;

	return;
}

void update_postion(Player * myPlayer)
{
	/* Horizontal */

	if(myPlayer->horizontal_effect == 0)
	{
		switch(myPlayer->player_movement)
		{
		// Moving left
		case 0:
			// Update position
			myPlayer->x -= horizontal_speed;
			// Update animation
			if(myPlayer->anim_position <= 0)
				myPlayer->anim_position--;
			if(myPlayer->anim_position == -6 || myPlayer->anim_position > 0)
				myPlayer->anim_position = -1;
			break;
		// Moving Idle
		case 1:
			// Update animation
			myPlayer->anim_position = 0;
			break;
		// Moving Right
		case 2:
			// Update position
			myPlayer->x += horizontal_speed;
			// Update animation
			if(myPlayer->anim_position >= 0)
				myPlayer->anim_position++;
			if(myPlayer->anim_position == 6 || myPlayer->anim_position < 0)
				myPlayer->anim_position = 1;
		}
	}

	myPlayer->x += myPlayer->horizontal_effect;

	/* Vertical */
	myPlayer->vertical_speed += gravity;
	myPlayer->y += myPlayer->vertical_speed;

	if(myPlayer->y > ground_level)
	{
		myPlayer->y = ground_level;
		myPlayer->vertical_speed = 0;
		myPlayer->grounded = 1;
		myPlayer->horizontal_effect = 0;
	}
	else
		myPlayer->grounded = 0;

	return;
}

void jump(Player * myPlayer)
{
	if(myPlayer->grounded || myPlayer->kick)
	{
		myPlayer->kick = 0;
		myPlayer->vertical_speed = jump_power;
	}
}
