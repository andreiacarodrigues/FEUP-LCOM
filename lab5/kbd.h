#ifndef __KBD_H
#define __KBD_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

// Subscribes a keyboard interruption
int kbd_subscribe_int(void);

// Unsubscribes a keyboard interruption
int kbd_unsubscribe_int();

// Displays a scancode
void kbd_display(long scancode);

// Handles the keybord interruptions
int kbd_int_handler();

#endif
