#include "Enemy.h"

Enemy* initialize_enemy(double init_position, double init_y, double left_bound, double right_bound, int x_size, int y_size)
{
	Enemy* myEnemy = (Enemy*) malloc(sizeof(Enemy));

	myEnemy->x = init_position;
	myEnemy->y = init_y;

	myEnemy->left_bound = left_bound;
	myEnemy->right_bound = right_bound;

	myEnemy->enemy_movement = 1;

	myEnemy->x_size = x_size;
	myEnemy->y_size = y_size;

	myEnemy->active = 1;
	myEnemy->anim_position = 0;

	return myEnemy;
}

void update_postion_enemy(Enemy * myEnemy)
{

	/* Update Position */
	if(myEnemy->enemy_movement == RIGHT)
	// Right
	{
		myEnemy->x += enemy_horizontal_speed;
		// Update animation
		if(myEnemy->anim_position >= 0)
			myEnemy->anim_position++;
		if(myEnemy->anim_position == 6 || myEnemy->anim_position < 0)
			myEnemy->anim_position = 1;
	}
	else
		// Left
	{
		myEnemy->x -= enemy_horizontal_speed;
		// Update animation
		if(myEnemy->anim_position <= 0)
			myEnemy->anim_position--;
		if(myEnemy->anim_position == -6 || myEnemy->anim_position > 0)
			myEnemy->anim_position = -1;
	}

	/* Update Direction */
	if(myEnemy->x > myEnemy->right_bound || myEnemy->x < myEnemy->left_bound)
		if(myEnemy->enemy_movement == RIGHT)
			myEnemy->enemy_movement = LEFT;
		else
			myEnemy->enemy_movement = RIGHT;

}

void check_collision(Player * myPlayer, Enemy * myEnemy)
{

	/* Check Vertically */
	if((myPlayer->y < myEnemy->y - myPlayer->y_size) || myPlayer->y > (myEnemy->y + myEnemy->y_size + myPlayer->y_size))
		return;

	/* Check Horizontally */
	if(myPlayer->x < myEnemy->x - myPlayer->x_size || myPlayer->x > myEnemy->x + myPlayer->x_size)
		return;

	/* Collision */

	// Kill Jump
	if((myPlayer->y >= myEnemy->y - myPlayer->y_size) && (myPlayer->y < myEnemy->y - myEnemy->y_size + kill_height_offset))
	{
		myEnemy->active = 0;
	}
	// Knock Back
	else
	{
		if(myPlayer->x > myEnemy->x)
			myPlayer->horizontal_effect = knockback_force;
		else
			myPlayer->horizontal_effect = -knockback_force;

		if(myPlayer->hp > 0)
			myPlayer->hp--;
	}

	myPlayer->kick = 1;
	jump(myPlayer);

	return;
}
