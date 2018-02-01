#include "kbd.h"

static char twoBytes = 0;
static int hook_ID = 0;
static int timer_counter = 0;

//Correta
int kbd_subscribe_int(void)
{

	hook_ID = KBC_IRQ;

	// Subscribes a notification on every interrupt in the input KBC_IRQ; policy is enabled to exclusive (disables the IH handler of Minix)
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

// Correta
int kbd_unsubscribe_int() {

	//Disables interrupts on the IRQ line associated with the specified hook_id.
	if (sys_irqdisable(&hook_ID) != OK)
	{
		printf("\n sys_irqdisable() failure \n");
		return -1;
	}

	// Unsubscribes the interrupt notification associated with the specified hook_ID
	if (sys_irqrmpolicy(&hook_ID) != OK)
	{
		printf("\n sys_irqrmpolicy() failure \n");
		return -1;
	}

	return 0;
}

void kbd_display(long scancode)
{
	if(scancode == 0xE0)
	{
		twoBytes = 1;
		return;
	}

	if((char)scancode & BIT(7))
	// Break
	{
		if(twoBytes)
		{
			printf("Break Code: 0xE0%X \n \n", scancode);
			twoBytes = 0;
			return;
		}
		else
		{
			printf("Break Code: 0x%X \n \n", scancode);
			return;
		}
	}
	else
	// Make
	{
		if(twoBytes)
		{
			printf("Make Code: 0xE0%X \n", scancode);
			twoBytes = 0;
			return;
		}
		else
		{
			printf("Make Code: 0x%X \n", scancode);
			return;
		}
	}
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

int kbd_int_handler()
{
	long scancode;
	int control;

	control = kbd_int_handler_verify();

	if(control)
		return control;

	control = sys_inb(KBC_OUT_BUF, &scancode);

	if(control)
	{
		printf("Error reading from the kbc out buffer");
		return control;
	}
	else
	{
		if(scancode == ESC_KEYCODE)
		{
			kbd_display(scancode);
			return -1;
		}
		else
		{
			kbd_display(scancode);
			return 0;
		}
	}
}

void kbd_timer_int_handler(int *contador)
{
	// Increments timer_counter -> number of interruptions handled
	timer_counter++;
	*contador = timer_counter;
}

