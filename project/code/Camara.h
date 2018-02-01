#pragma once

#include "Character.h"
#include "Enemy.h"
#include "Platform.h"
#include "Coins.h"
#include "Spikes.h"
#include "Boxes.h"
#include "Flag.h"
#include "Clouds.h"

static int offset = 0;

/** @defgroup Camara Camara
 * @{
 * Functions for manipulating the camera
 */

/**
 * @brief Adjust objects positions according to the player movement
 *
 * @param myPlayer pointer to player
 * @param Platforms[] pointer to the platforms array
 * @param nr_platforms number of platforms
 * @param Coins[] pointer to the coins array
 * @param nr_coins number of coins
 * @param Spikes[] pointer to the spikes array
 * @param nr_spikes number of spikes
 * @param DBox[] pointer to the destructable boxes array
 * @param nr_dboxes number of destructable boxes
 * @param Enemies[] pointer to the enemies array
 * @param nr_enemies number of enemies
 * @param Boxes[] pointer to the boxes array
 * @param nr_boxes number of boxes
 * @param flag pointer to the flag
 * @param Clouds[] pointer to the clouds array
 * @param nr_clouds number of clouds
 *
 */
void adjust_camara(Player * myPlayer, Platform* Platforms[], int nr_platforms, Coin* Coins[],int nr_coins, Spike* Spikes[], int nr_spikes, DestructableBox* DBox[], int nr_dboxes, Enemy* Enemies[], int nr_enemies, Box* Boxes[], int nr_boxes, Flag* flag, Cloud* Clouds[], int nr_clouds);
