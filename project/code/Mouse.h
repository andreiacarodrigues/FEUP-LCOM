#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
/** @defgroup Mouse Mouse
 * @{
 * Functions for handling the mouse interruptions
 */
typedef struct {
	int x, y;
	int x_Sign, y_Sign;
	int delta_X, delta_Y;
	double speed;

	int byte_counter;
	unsigned long packet[3];

	int left_button_pressed;
	int right_button_pressed;

	int left_button_released;
	int right_button_released;

	int hasBeenUpdated;
	int draw;
} Mouse;
/**
 * @brief Subscribes a keyboard interruption
 * @return irq bit if sucessfull, -1 if not
 */
int mouse_subscribe_int(void);
/**
 * @brief Unsubscribes a keyboard interruption
 * @return 0 if sucessfull, -1 if not
 */
int mouse_unsubscribe_int();
/**
 * @brief Set stream mode
 * @return 0 if sucessfull, -1 if not
 */
int mouse_set_stream_mode();
/**
 * @brief Enables stream mode
 * @return 0 if sucessfull, 1 if not
 */
int mouse_enable_stream_mode();
/**
 * @brief Disables stream mode
 * @return 0 if sucessfull, 1 if not
 */
int mouse_disable_stream_mode();
/**
 * @brief Reads from input buffer
 * @return 0 if sucessfull, 1 if not
 */
int mouse_read_inb();
/**
 * @brief Reads from output buffer
 * @return 0 if sucessfull, 1 if not
 */
int mouse_read_outb();
/**
 * @brief Reads the command response
 * @param response pointer to the response
 * @return 0 if sucessfull, 1 if not
 */
int mouse_read_cmd_response(unsigned long* response);
/**
 * @brief Writes the command to mouse
 * @param cmd command to execute
 * @return 0 if sucessfull, 1 if not
 */
int mouse_write(unsigned char cmd);
/**
 * @brief Handles the mouse interruptions and updates it
 */
void update_mouse();
/**
 * @brief Get mouse
 * @return pointer to mouse
 */
Mouse* getMouse();
/**
 * @brief Creates new mouse
 * @return pointer to new mouse
 */
Mouse* newMouse();

#endif
