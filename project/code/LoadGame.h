#include "GameLoop.h"
/** @defgroup LoadGame LoadGame
 * @{
 * Functions for loading game objects
 */
/**
 * @brief Loads enemies
 * @param Enemies[] pointer to enemies array
 */
void loadEnemies(Enemy* Enemies[]);
/**
 * @brief Loads platforms
 * @param Platforms[] pointer to platforms array
 */
void loadPlatforms(Platform* Platforms[]);
/**
 * @brief Loads spikes
 * @param Spikes[] pointer to spikes array
 */
void loadSpikes(Spike* Spikes[]);
/**
 * @brief Loads coins
 * @param Coins[] pointer to coins array
 */
void loadCoins(Coin* Coins[]);
/**
 * @brief Loads destructable boxes
 * @param DBox[] pointer to destructable boxes array
 */
void loadDBoxes(DestructableBox* DBox[]);
/**
 * @brief Loads boxes
 * @param Boxes[] pointer to boxes array
 */
void loadBoxes(Box* Boxes[]);
/**
 * @brief Loads clouds
 * @param Clouds[] pointer to clouds array
 */
void loadClouds(Cloud* Clouds[]);
