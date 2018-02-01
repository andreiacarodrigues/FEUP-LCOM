#pragma once

#include "Character.h"
/** @defgroup Coins Coins
 * @{
 * Functions for manipulating the coins
 */
typedef struct
{
	double x;
	double y;
	int x_size;
	int y_size;
	int active;
} Coin;
/**
 * @brief Initializes a new coin
 * @param ix coin x
 * @param iy coin y
 * @param ix_size coin horizontal size
 * @param iy_size coin vertical size
 * @return pointer to the new coin created
 */
Coin* new_coin(double ix, double iy, int ix_size, int iy_size);
/**
 * @brief Detects collision between player and coin and collects it
 * @param myPlayer pointer to player
 * @param c pointer to coin
 */
void coin_pickup(Player * myPlayer, Coin * c);
