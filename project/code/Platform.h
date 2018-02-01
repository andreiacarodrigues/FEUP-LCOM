#pragma once

#include "Character.h"
/** @defgroup Platform Platform
 * @{
 * Functions for manipulating the platforms
 */
typedef struct
{
	double x;
	double y;
	int x_size;
	int y_size;
} Platform;
/**
 * @brief Initializes a new platform
 * @param ix platform x
 * @param iy platform y
 * @param ix_size platform horizontal size
 * @param iy_size platform vertical size
 * @return pointer to the new platform created
 */
Platform* new_platform(double ix, double iy, int ix_size, int iy_size);
/**
 * @brief Checks if the player and the platform are colliding
 * @param myPlayer pointer to player
 * @param p pointer to platform
 */
void check_collision_platform(Player * myPlayer, Platform * p);
