#pragma once

#include "Bitmap.h"
#include "Graphics.h"

typedef enum { MOVING_LEFT, MOVING_IDLE, MOVING_RIGHT } Movement ;

/** @defgroup Character Character
 * @{
 * Functions for manipulating the character
 */

typedef struct
{
	double x;
	double y;
	double horizontal_effect;
	int x_size;
	int y_size;
	int kick;
	int grounded;
	int vertical_speed;
	int anim_position;
	int coins;
	int hp;
	int score;
	int time;
	Movement player_movement;
} Player;

static int gravity = 1;
static int horizontal_speed = 6;
static double jump_power = -19.5;
static int ground_level = 576;
static int initial_position = 128;

/**
 * @brief Initializes a new player
 *
 * @param ix_size player horizontal size
 * @param iy_size player vertical size
 * @return pointer to the new player created
 */
Player* initialize_player(int x_size, int y_size);
/**
 * @brief Updates character movement
 *
 * @param change keyboard input to change character's movement
 * @param myPlayer pointer to player
 */
void update_movement(int change, Player * myPlayer);
/**
 * @brief Updates character position
 *
 * @param myPlayer pointer to player
 */
void update_postion(Player * myPlayer);
/**
 * @brief Makes the character jump
 *
 * @param myPlayer pointer to player
 */
void jump(Player * myPlayer);
