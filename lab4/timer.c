#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
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

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if (timer < 0 || timer > 2)
	{
		printf("The timer must be between 0 and 2 \n");
		return 1;
	}

	if (sys_outb(TIMER_CTRL, TIMER_RB_COUNT_ | TIMER_RB_SEL(timer) | TIMER_RB_CMD))
	{
		printf("Error uploading the readback settings to the control \n");
		return 1;
	}

	unsigned long myTimer = TIMER_0 + timer;

	unsigned long readLong;
	int control;

	control = sys_inb(myTimer, &readLong);

	*st = (unsigned char) readLong;

	return control;
}

int timer_display_conf(unsigned char conf) {
	printf("Hexadecimal value: %x \n", conf);

	if (conf & BIT(0))
		printf("Timer working in Binary. \n");
	else
		printf("Timer working in BCD. \n");

	unsigned int opMode = (conf & (BIT(1) | BIT(2) | BIT(3))) >> 1;

	switch (opMode) {
	case 0:
		printf("Timer operating mode 0, interrupt on terminal count. \n");
		break;
	case 1:
		printf("Timer operating mode 1, hardware retriggerable one-shot \n");
		break;
	case 2:
	case 6:
		printf("Timer operating mode 2, rate generator \n");
		break;
	case 3:
	case 7:
		printf("Timer operating mode 3, square wave. \n");
		break;
	case 4:
		printf("Timer operating mode 4, software triggered strobe \n");
		break;
	case 5:
		printf(
				"Timer operating mode 5, hardware triggered strobe(retriggerable) \n");
		break;
	}

	unsigned int ToA = (conf & (BIT(4) | BIT(5))) >> 4;

	switch (ToA) {
	case 1:
		printf("Timer type of acess: LSB. \n");
		break;
	case 2:
		printf("Timer type of acess: MSB. \n");
		break;
	case 3:
		printf("Timer type of acess: LSB followed by MSB. \n");
		break;
	}

	unsigned int out = (conf & BIT(7)) >> 7;

	switch (out) {
	case 0:
		printf("Out value: 0 \n \n");
		break;
	case 1:
		printf("Out value: 1 \n \n");
		break;
	}

	printf("\n");

	return 0;
}

int timer_test_square(unsigned long freq) {
	int control;
	control = timer_set_square(0, freq);
	return control;
}

int timer_test_int(unsigned long time) {
	int control = timer_subscribe_int();

	if (control < 0) {
		printf("\n timer_subscribe_int() failure \n");
		return -1;
	}
	int ipc_status, r;
	int irq_set = BIT(control); //irq_set is a bitmask with the bits corresponding to the hook_id values used in the sys_irqsetpolicy() set to 1
	message msg;

	int contador = 0;

	while (timer_counter <= time*TIMER_DEFAULT_VALUE)
	{
		/* Get a request message. */
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();

					if(timer_counter % TIMER_DEFAULT_VALUE == 0)
						printf("Elapsed time in seconds: %d \n", (timer_counter)/TIMER_DEFAULT_VALUE);
			}
			break;
		default:
			break; /* no other notifications expected: do nothing */
			}
		}
		else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	if (timer_unsubscribe_int() != 0) {
		printf("\n timer_unsubscribe_int() failure \n");
		return -1;
	}

	printf("Operation sucessfull \n");
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char configuration;
	int control;

	control = timer_get_conf(timer, &configuration);
	if (control)
	{
		printf("Error getting the configurations \n");
		return control;
	}

	control = timer_display_conf(configuration);
	if (control)
	{
		printf("Error displaying the configurations \n");
		return control;
	}

	return 0;
}
