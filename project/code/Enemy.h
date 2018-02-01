#pragma once

#include "Character.h"
/** @defgroup Enemy Enemy
 * @{
 * Functions for manipulating the enemy
 */
typedef enum { LEFT, RIGHT } Enemy_Movement ;

typedef struct
{
	double x;
	double y;
	double left_bound;
	double right_bound;
	int x_size;
	int y_size;
	int active;
	int anim_position;
	Enemy_Movement enemy_movement;
} Enemy;

static int enemy_horizontal_speed = 4;
static int knockback_force = 10;
static int kill_height_offset = 15;
/**
 * @brief Initializes a new coin
 * @param init_position enemy initial x position
 * @param init_y enemy initial y position
 * @param left_bound enemy left bound
 * @param right_bound enemy right bound
 * @param x_size enemy horizontal size
 * @param y_size enemy vertical size
 * @return pointer to the new enemy created
 */
Enemy* initialize_enemy(double init_position, double init_y, double left_bound, double right_bound, int x_size, int y_size);
/**
 * @brief Updates the enemy position
 * @param myEnemy pointer to enemy
 */
void update_postion_enemy(Enemy * myEnemy);
/**
 * @brief  Detects collision between player and enemy
 * @param myPlayer pointer to player
 * @param myEnemy pointer to enemy
 */
void check_collision(Player * myPlayer, Enemy * myEnemy);
