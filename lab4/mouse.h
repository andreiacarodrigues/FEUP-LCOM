#ifndef __KBD_H
#define __KBD_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

// Subscribes a keyboard interruption
int mouse_subscribe_int(void);

// Unsubscribes a keyboard interruption
int mouse_unsubscribe_int();
int mouse_set_stream_mode();
int mouse_enable_stream_mode();
int mouse_disable_stream_mode();
int mouse_read_inb();
int mouse_read_outb();
int mouse_read_cmd_response(unsigned long* response);
int mouse_write(unsigned char cmd);

#endif
