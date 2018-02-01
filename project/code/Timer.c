#include <minix/syslib.h>
#include <minix/drivers.h>

#include "Timer.h"
#include"i8254.h"

// Static Variables

static int hook_ID;
static int timer_counter = INICIAL_TIMER_COUNT;

// Static auxiliar function for timer_set_square
static unsigned long binary_to_BCD(unsigned long binary)
{
	unsigned long invertedBCD = 0;
	unsigned long BCD = 0;
	while (binary != 0)
	{
		invertedBCD <<= 4;
		invertedBCD |= binary % 10;
		binary /= 10;
	}
	while(invertedBCD != 0)
	{
		BCD <<= 4;
		BCD |= invertedBCD % (1 << 4);
		invertedBCD /= (1 << 4);
	}
	return BCD;
}

int timer_set_square(unsigned long timer, unsigned long freq)
{
	// Boundarie Checks
	if (timer < 0 || timer > 2) // < 0 will never happen but is here to safeguard future changes
	{
		printf("The timer must be between 0 and 2 \n");
		return 1;
	}

	int minValue = TIMER_FREQ/MAX_DIVIDER;

	if (freq < minValue || freq > TIMER_FREQ)
	{
		printf("The frequency must be between %d and the timer default frequency \n", minValue);
		return 1;
	}

	// Declare timer and control variables

	unsigned long myTimer = TIMER_0 + timer;

	// Calculate the new freq

	unsigned int n = TIMER_FREQ / freq;

	// Determine if it's binary or bcd

	if (sys_outb(TIMER_CTRL, TIMER_RB_SEL(timer) | TIMER_RB_CMD | TIMER_RB_COUNT_))
	{
		printf("Error uploading the readback settings to the control \n");
		return 1;
	}

	unsigned long readLong;

	if(sys_inb(myTimer, &readLong))
	{
		printf("Error reading from the timer \n");
		return 1;
	}

	int isBCD = ((readLong & TIMER_BCD) == TIMER_BCD);

	// Check BCD/Binary Word Boundaries

	if(isBCD && freq > MAX_BCD_IN_WORD)
	{
		printf("The frequency in BCD must be lower then 9999 \n");
		return 1;
	}

	if(!isBCD && freq > MAX_BINARY_IN_WORD)
	{
		printf("The frequency exceeds the max binary word \n");
		return 1;
	}

	// Get timer control word

	unsigned long sCounter = 0;

	switch (myTimer)
	{
	case 0:
		sCounter = TIMER_SEL0;
		break;
	case 1:
		sCounter = TIMER_SEL1;
		break;
	case 2:
		sCounter = TIMER_SEL2;
		break;
	}

	// Get LSB and MSB

	unsigned char lsb = n & 0xFF;
	unsigned char msb = n >> 8;

	// Processe the frequency in the right mode

	if (isBCD)
	// BCD
	{
		if (sys_outb(TIMER_CTRL, sCounter | TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_BCD))
		{
			printf("Error uploading the settings to the control word \n");
			return 1;
		}

		unsigned char BCDlsb = binary_to_BCD(freq) & 0xFF;
		unsigned char BCDmsb = binary_to_BCD(freq) >> 8;

		if (sys_outb(myTimer, BCDlsb))
		{
			printf("Error uploading the LSB \n");
			return 1;
		}

		int failed = (sys_outb(myTimer, BCDmsb));

		if(failed)
		{
			printf("Error uploading the MSB \n");
			return 1;
		}
		else
		{
			printf("Set square sucessfull \n");
			return 0;
		}

	} else
	// Binary
	{
		if (sys_outb(TIMER_CTRL, sCounter | TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_BIN))
		{
			printf("Error uploading the settings to the control word \n");
			return 1;
		}

		if (sys_outb(myTimer, lsb))
		{
			printf("Error uploading the LSB \n");
			return 1;
		}

		int failed = (sys_outb(myTimer, msb));

		if(failed)
		{
			printf("Error uploading the MSB \n");
			return 1;
		}
		else
		{
			printf("Set square sucessfull \n");
				return 0;
		}
	}
}

int timer_subscribe_int(void) {
	// hook_ID is the bit position where we want to place the interruption in TIMER0_IRQ
	hook_ID = INICIAL_HOOK_ID;

	// Subscribes a notification on every interrupt in the input TIMER0_IRQ; policy is enabled to automatic
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_ID) != OK)
	{
		printf("\n sys_irqsetpolicy() failure \n");
		return -1;
	}
	if (sys_irqenable(&hook_ID) != OK)
	{
		printf("\n sys_irqenable() failure \n");
		return -1;
	}
	return TIMER0_IRQ;
}

int timer_unsubscribe_int() {

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

void timer_int_handler() {
	// Increments timer_counter -> number of interruptions handled

	timer_counter++;
}

