#pragma once

#include "Character.h"

/** @defgroup Boxes Boxes
 * @{
 * Functions for manipulating the boxes and destructable boxes
 */

typedef struct
{
	double x;
	double y;
	int x_size;
	int y_size;
} Box;

typedef struct
{
	double x;
	double y;
	int x_size;
	int y_size;
	int active;
} DestructableBox;

/**
 * @brief Creates a new box
 *
 * @param ix box x
 * @param iy box y
 * @param ix_size box horizontal size
 * @param iy_size box vertical size
 * @return pointer to the new box created
 */
Box* new_box(double ix, double iy, int ix_size, int iy_size);

/**
 * @brief Creates a new destructable box
 *
 * @param ix destructable box  x
 * @param iy destructable box y
 * @param ix_size destructable box horizontal size
 * @param iy_size destructable box vertical size
 * @return pointer to the new destructable box created
 */
DestructableBox* new_dbox(double ix, double iy, int ix_size, int iy_size);

/**
 * @brief Checks box collisions
 *
 * @param myPlayer pointer to player
 * @param b pointer to box
 */
void check_collision_box(Player * myPlayer, Box * b);

/**
 * @brief Checks destructable box collisions
 *
 * @param myPlayer pointer to player
 * @param db pointer to destructable box
 */
void check_collision_dbox(Player * myPlayer, DestructableBox * db);

/**
 * @brief Checks if box was destroyed
 *
 * @param x mouse x
 * @param y mouse y
 * @param pressed 1 if left button of mouse was pressed, 0 if not
 * @param db pointer to destructable box
 */
void check_pressed_dbox(int x, int y, int pressed, DestructableBox * db);
