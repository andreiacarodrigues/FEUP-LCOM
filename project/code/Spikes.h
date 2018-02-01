#pragma once

#include "Character.h"
/** @defgroup Platform Platform
 * @{
 * Functions for managing the spikes
 */
typedef struct
{
	double x;
	double y;
	int x_size;
	int y_size;
} Spike;

static int knockback_spike = 10;
/**
 * @brief Creates new spikes
 * @param ix spikes x
 * @param iy spikes y
 * @param ix_size spikes horizontal size
 * @param iy_size spikes vertical size
 * @return pointer to the new spikes created
 */
Spike* new_spikes(double ix, double iy, int ix_size, int iy_size);
/**
 * @brief Checks collision betweet the player and the spikes
 * @param myPlayer pointer to player
 * @param s pointer to spikes
 */
void check_collision_spikes(Player * myPlayer, Spike * s);
