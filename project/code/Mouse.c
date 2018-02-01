#include "Mouse.h"
#include "i8042.h"

#define N_TRIES 100

Mouse* mouse = NULL;
Mouse mouse_previous_state;

static int counter_bytes = 0;
static unsigned int hook_ID = MOUSE_IRQ; // hook_ID = 12
static unsigned char packet[PACKET_SIZE];

int mouse_subscribe_int()
{
	unsigned int hook_temporary = hook_ID;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_ID) != OK)
			return -1;

	if (sys_irqenable(&hook_ID) != OK)
		return -1;

	return BIT(hook_temporary);
}

int mouse_unsubscribe_int() {

	if (sys_irqdisable(&hook_ID) != OK)
		return -1;

	if (sys_irqrmpolicy(&hook_ID) != OK)
		return -1;

	return 0;
}

int mouse_set_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_MODE))
		return 1;

	return 0;
}

int mouse_enable_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_ENABLE))
		return 1;

	return 0;
}

int mouse_disable_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_DISABLE))
		return 1;

	return 0;
}

Mouse* newMouse()
{
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 0;
	mouse->y = 0;
	mouse->speed = 1.4;

	mouse->byte_counter = 0;

	mouse->left_button_pressed = 0;
	mouse->right_button_pressed = 0;

	mouse->left_button_released = 0;
	mouse->right_button_released = 0;

	mouse->hasBeenUpdated = 0;
	mouse->draw = 1;

	mouse_previous_state = *mouse;

	return mouse;
}

Mouse* getMouse()
{
	if (!mouse)
		mouse = newMouse();

	return mouse;
}

void update_mouse()
{
	Mouse* mouse = getMouse();

	sys_inb(KBC_OUT_BUF, &mouse->packet[mouse->byte_counter]);

	if((mouse->byte_counter == 0) && (((mouse->packet[mouse->byte_counter] & BIT(3)) == 0) || mouse->packet[mouse->byte_counter] == 0xFA))
		return;

	mouse->byte_counter++;

	if(mouse->byte_counter == 3)
	{
		mouse->byte_counter = 0;

		/* Buttons Current State */
		mouse->left_button_pressed = mouse->packet[0] & LEFT_BUTTON;
		mouse->right_button_pressed = mouse->packet[0] & RIGHT_BUTTON;

		/* X and Y signs */
		mouse->x_Sign = mouse->packet[0] & X_SIGN;
		mouse->y_Sign = mouse->packet[0] & Y_SIGN;
		mouse->delta_X = mouse->packet[1];
		mouse->delta_Y = mouse->packet[2];

		/* X and Y delta movements */
		if(mouse->x_Sign)
		{	mouse->delta_X |= (-1 << 8);	}

		if(mouse->y_Sign)
		{	mouse->delta_Y |= (-1 << 8);	}

		/* Comparing buttons current state with the previous state */
		if (mouse_previous_state.left_button_pressed != 0 && mouse->left_button_pressed == 0)
			mouse->left_button_released = 1;
		if (mouse_previous_state.right_button_pressed != 0	&& mouse->right_button_pressed == 0)
			mouse->right_button_released = 1;

		/* Updating the X coordinate */
		if (mouse->delta_X != 0)
		{
			if((mouse->x += mouse->speed * mouse->delta_X) < 0)
				mouse->x = 0;
			else if (mouse->x >= getHorResolution())
				mouse->x = getHorResolution() - 1;
		}

		/* Updating the Y coordinate */
		if (mouse->delta_Y != 0)
		{
			if((mouse->y -= mouse->speed * mouse->delta_Y) < 0)
				mouse->y = 0;
			else if (mouse->y >= getVerResolution())
				mouse->y = getVerResolution() - 1;
		}

		/* Saves current state as previous state */
		mouse->delta_X = mouse->x - mouse_previous_state.x;
		mouse->delta_Y = mouse->y - mouse_previous_state.y;
		mouse_previous_state = *mouse;

		mouse->hasBeenUpdated = 1;
		mouse->draw = 1;
	}
}

int mouse_read_inb()
{
	unsigned long controlReg;
	unsigned int i = 0;

	for(i; i < N_TRIES; i++)
	{
		if(sys_inb(KBC_STAT_REG, &controlReg))
		{
			printf("Error checking the status register \n");
			return 1;
		}

		if(!(controlReg & INPUT_BUFFER))
			return 0;

		tickdelay(micros_to_ticks(20000));
	}
	return 1;
}

int mouse_read_outb()
{
	unsigned long controlReg;
	unsigned int i = 0;

	for(i; i < N_TRIES; i++)
	{
		if(sys_inb(KBC_STAT_REG, &controlReg))
		{
			printf("Error checking the status register \n");
			return -1;
		}

		if(!(controlReg & OUTPUT_BUFFER))
		{
			return 0;
		}

		if(controlReg & PARITY)
		{
			printf("Parity error");
			return 1;
		}

		if(controlReg & TIMEOUT)
		{
			printf("Timeout error");
			return 1;
		}

		tickdelay(micros_to_ticks(20000));
	}
	return 0;
}

int mouse_read_cmd_response(unsigned long* response)
{
	unsigned int j = 0;

	for(j; j < N_TRIES; j++)
	{
		unsigned long control = mouse_read_outb();

		if(control < 0)
		{
			return 1;
		}
		else
		{
			if(control == 0)
				break;
		}
	}

	if (sys_inb(KBC_OUT_BUF, response))
	{
		printf("Error getting a response from the outbuffer \n");
		return 1;
	}
	return 0;
}

int mouse_write(unsigned char cmd)
{
	unsigned long control;
	unsigned int i = 0;

	for(i; i < N_TRIES; i++)
	{
		if(mouse_read_inb()) // tries to read the in buffer
		{
			printf("Error reading the stat register \n");
			return 1;
		}

		if(sys_outb(KBC_CTRL_REG, KBC_WRITE_MOUSE)) // colocar 0xD4 no ctrl reg para ativar o rato
		{
			printf("Error uploading the command 0xD4 to the control register \n");
			return 1;
		}

		if(mouse_read_inb()) // tries to read the in buffer
		{
			printf("Error reading the stat register \n");
			return 1;
		}

		if(sys_outb(KBC_IN_BUF, cmd)) // tries to write the command we want on the ctrl register
		{
			printf("Error uploading the command %h to the in buffer \n", cmd);
			return 1;
		}

		if (sys_inb(KBC_OUT_BUF, &control))
		{
			printf("Error getting a response from the outbuffer \n");
			return 1;
		}
		if(control == MOUSE_ACK)
		{
			return 0;
		}
	}

	return 1;
}
