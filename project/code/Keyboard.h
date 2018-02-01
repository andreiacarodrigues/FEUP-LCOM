#ifndef __KEYBOARD_H
#define __KEYBOARD_H
/** @defgroup Keyboard Keyboard
 * @{
 * Functions for handling the keyboard interruptions
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "Character.h"
/**
 * @brief Subscribes a keyboard interruption
 * @return irq bit if sucessfull, -1 if not
 */
int kbd_subscribe_int(void);
/**
 * @brief  Unsubscribes a keyboard interruption
 * @return 0 if sucessfull, -1 if not
 */
int kbd_unsubscribe_int();
/**
 * @brief  Displays a scancode
 * @param scancode key scancode
 */
void kbd_display(long scancode);
/**
 * @brief Handles the keybord interruptions
 * @param myPlayer pointer to player
 * @param game_active 1 if program is in game mode, 0 if not
 * @return 0 if sucessfull, -1 if not
 */

int kbd_int_handler(Player * myPlayer, int game_active);

#endif
