#ifndef __GAMELOOP_H
#define __GAMELOOP_H

#include <minix/syslib.h>
#include <minix/drivers.h>
/** @defgroup GameLoop GameLoop
 * @{
 * Function that has the game loop
 */
#include "Graphics.h"
#include "i8042.h"
#include "i8254.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Bitmap.h"
#include "Character.h"
#include "Mouse.h"
#include "Enemy.h"
#include "Camara.h"
#include "Platform.h"
#include "Coins.h"
#include "Boxes.h"
#include "Spikes.h"
#include "RTC.h"
#include "ScoreManager.h"
#include "Flag.h"
#include "Clouds.h"

#define TWO_MINUTES 7200
/**
 * @brief Initializes the game
 * @param mode video memory mode
 */
void startGame(unsigned short mode);

#endif
