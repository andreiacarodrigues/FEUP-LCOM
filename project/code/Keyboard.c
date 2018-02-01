#include "Keyboard.h"

static char twoBytes = 0;
static int hook_ID = 0;
static int horizontal = 1;

int kbd_subscribe_int(void)
{
	hook_ID = KBC_IRQ;

	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_ID) != OK)
	{
		printf("\n sys_irqsetpolicy() failure");
		return -1;
	}
	if (sys_irqenable(&hook_ID) != OK)
	{
		printf("\n sys_irqenable() failure");
		return -1;
	}
	return BIT(KBC_IRQ);
}

int kbd_unsubscribe_int() {

	if (sys_irqdisable(&hook_ID) != OK)
	{
		printf("\n sys_irqdisable() failure \n");
		return -1;
	}

	if (sys_irqrmpolicy(&hook_ID) != OK)
	{
		printf("\n sys_irqrmpolicy() failure \n");
		return -1;
	}

	return 0;
}

int kbd_int_handler_verify()
{
	int control;
	long controlReg;

	control = sys_inb(KBC_STAT_REG, &controlReg);

	if(control)
	{
		printf("Error reading from the kbc stat reg");
		return control;
	}

	if(!(controlReg & OUTPUT_BUFFER))
	{
		printf("No data available for reading");
		return 1;
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

	return 0;
}

int kbd_int_handler(Player * myPlayer, int game_active)
{
	long scancode;
	int control;

	control = kbd_int_handler_verify();

	if(control)
		return control;

	control = sys_inb(KBC_OUT_BUF, &scancode);

	if(control)
		return control;

	if(scancode == ESC_KEYCODE)
		return -1;

	if(!game_active)
		return 0;

	switch(scancode)
	{
	case LEFT_ARROW_MAKECODE:
		horizontal = 0;
		update_movement(horizontal, myPlayer);
		return 0;
		break;

	case LEFT_ARROW_BREAKCODE:
		if(horizontal == 0)
			horizontal = 1;
		update_movement(horizontal, myPlayer);
		return 0;
		break;

	case RIGHT_ARROW_MAKECODE:
		horizontal = 2;
		update_movement(horizontal, myPlayer);
		return 0;
		break;

	case RIGHT_ARROW_BREAKCODE:
		if(horizontal == 2)
			horizontal = 1;
		update_movement(horizontal, myPlayer);
		return 0;
		break;

	case SPACE_MAKECODE:
		jump(myPlayer);
		return 0;
		break;

	default:
		return 0;
		break;
	}

}
