#include "mouse.h"
#include "timer.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>

#define N_TRIES 100

static unsigned char packet[PACKET_SIZE];
static unsigned int counter_packets = 0;
static int counter_bytes = 0;

void print_config()
{
	unsigned int i = 0;

	printf("\n");
	printf("Byte 1 = 0x%X \n", packet[0]);
	printf("Byte 2 = 0x%X  \n", packet[1]);
	printf("Byte 3 = 0x%X  \n", packet[2]);

	if(packet[0] & STATUS_LEFT_BUTTON){printf("Left Button pressed. \n");}
		else{printf("Left Button released. \n");}
	if(packet[0] & STATUS_MIDDLE_BUTTON){printf("Middle Button pressed. \n");}
		else{printf("Middle Button released. \n");}
	if(packet[0] & STATUS_RIGHT_BUTTON){printf("Right Button pressed. \n");}
		else{printf("Right Button released. \n");}
	if(packet[0] & STATUS_REMOTE){printf("Remote mode enabled. \n");}
		else{printf("Remote mode disabled. \n");}
	if(packet[0] & STATUS_ENABLE){printf("Data reporting enabled. \n");}
		else{printf("Data reporting disabled. \n");}
	if(packet[0] & STATUS_ENABLE){printf("Remote mode enabled. \n");}
		else{printf("Stream mode enabled. \n");}
	if(packet[1] == 0){printf("Resolution = 1 unit per mm. \n");}
	else if(packet[1] == 1){printf("Resolution = 2 unit per mm. \n");}
		else if(packet[1] == 2){printf("Resolution = 4 unit per mm. \n");}
			else if(packet[1] == 3){printf("Resolution = 8 unit per mm. \n");}
				else printf("Resolution = unknown. \n");
	printf("Sample rate = %d Hz \n" , packet[2]);

	return;
}
void display_packet()
{
	//Prints the bytes
	printf("B1=Ox%X  ", packet[0]);
	printf("B2=Ox%X  ", packet[1]);
	printf("B3=Ox%X  ", packet[2]);

	//Prints the left, middle and right buttons
	if(packet[0] & LEFT_BUTTON){printf("LB=%d  ", 1);}
		else{printf("LB=%d  ", 0);}
	if(packet[0] & MIDDLE_BUTTON){printf("MB=%d  ", 1);}
		else{printf("MB=%d  ", 0);}
	if(packet[0] & RIGHT_BUTTON){printf("RB=%d  ", 1);}
		else{printf("RB=%d  ", 0);}

	//Prints the X and Y Overflows
	if(packet[0] & X_OVFL){printf("XOV=%d  ", 1);}
		else{printf("XOV=%d  ", 0);}
	if(packet[0] & Y_OVFL){printf("YOV=%d  ", 1);}
		else{printf("YOV=%d  ", 0);}

	//Prints the X and Y intervals
	if(packet[0] & X_SIGN)
	{
		short xPrint = packet[1] | 0xFF00;
		printf("X=%d  ", xPrint);
	}
	else
		{printf("X=%d  ", packet[1]);}

	if(packet[0] & Y_SIGN)
	{
		short yPrint = packet[2] | 0xFF00;
		printf("Y=%d  ", yPrint);
	}
	else
		{printf("Y=%d  ", packet[2]);}

	printf("\n");
	return;
}

int packets_int_handler()
{
	unsigned long control;

	//if(mouse_read_cmd_response(&control)) //all the printf's already made in previous functions
		//return 1;

	sys_inb(KBC_OUT_BUF, &control);

	if((counter_bytes == 0) && (((control & BIT(3)) == 0) || control == 0xFA))//se for o byte n 0
	{
		return 2;
	}
	packet[counter_bytes] = control;
	counter_bytes++;
	if(counter_bytes == 3)
	{
		counter_packets++;
		counter_bytes = 0;
		display_packet();

	}
	return 0;

}



int test_packet(unsigned short cnt){
	int control = mouse_subscribe_int();
	if (control < 0)
	{
		printf("\n mouse_subscribe_int() failure \n");
		return -1;
	}

	counter_packets = 0;
	if(mouse_set_stream_mode())
		return 1;
	if(mouse_enable_stream_mode())
		return 1;

	int ipc_status, r;
	int irq_set = control; //irq_set is a bitmask with the bits corresponding to the hook_id values used in the sys_irqsetpolicy() set to 1
	message msg;

	while (counter_packets < cnt)
	{
		/* Get a request message. */
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0)
		{
			printf("driver_receive() failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set)
				{ /* subscribed interrupt */
					unsigned int data = packets_int_handler();
					if(data == 1)
					{
						printf("Error getting the mouse packets \n");
						return 1;
					}
					else
					{
						if(data == 2)
							continue;
					}


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
		if(mouse_disable_stream_mode())
		{
			return 1;
		}

		if (mouse_unsubscribe_int() != 0)
		{
			printf("\n kbd_unsubscribe_int() failure \n");
			return 1;
		}

	mouse_read_outb();
	printf("Operation sucessfull \n");

	return 0;
}



int test_async(unsigned short idle_time) {

	int control_m = mouse_subscribe_int();

	if (control_m < 0)
	{
		printf("\n mouse_subscribe_int() failure \n");
		return -1;
	}

	int control_timer = timer_subscribe_int();

	if (control_timer < 0)
	{
		printf("\n timer_subscribe_int() failure \n");
		return -1;
	}

	counter_packets = 0;

	if(mouse_set_stream_mode())
		return 1;
	if(mouse_enable_stream_mode())
		return 1;

	int ipc_status, r;
	int irq_set_m = control_m;
	int irq_set_timer = BIT(control_timer);
	message msg;
	int counter = 0;
	int running = 1;


	while (running)
	{
		/* Get a request message. */
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0)
		{
			printf("driver_receive() failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_m)
				{ /* subscribed interrupt */
					counter = 0;
					unsigned int data = packets_int_handler();
					if(data == 1)
					{
						printf("Error getting the mouse packets \n");
						return 1;
					}
					else
					{
						if(data == 2)
							continue;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer)
				{
					if(counter > idle_time*TIMER_DEFAULT_VALUE)
					{
						running = 0;
						printf("Operation timed out. \n");
					}
					else
					{
						counter++;
					}
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
		if(mouse_disable_stream_mode())
		{
			return 1;
		}

		if (mouse_unsubscribe_int() != 0)
		{
			printf("\n kbd_unsubscribe_int() failure \n");
			return 1;
		}

	mouse_read_outb();
	printf("Operation sucessfull \n");

	return 0;
}

int test_config(void)
{
	int control = mouse_subscribe_int();
	unsigned long controlReg;

	if (control < 0)
	{
		printf("\n mouse_subscribe_int() failure \n");
		return -1;
	}

	if(mouse_disable_stream_mode())
		return 1;

	if(mouse_write(MOUSE_STATUS_REQ))
	{
		printf("Error writing the status register \n");
		return 1;
	}

	unsigned int i = 0;
	for(i ; i < PACKET_SIZE ; i++)
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

		if (sys_inb(KBC_STAT_REG, &controlReg))
		{
			printf("Error getting a response from the outbuffer \n");
			return 1;
		}

		long unsigned int lui = (long unsigned int) packet[i];
		long unsigned int * ref = &lui;

		if (sys_inb(KBC_OUT_BUF, ref) != OK)
		{
			return 1;
		}

		packet[i] = (unsigned char) lui;

		if (controlReg & BIT(5))
		{
			break;
		}

	}
	print_config();

	if(mouse_disable_stream_mode())
		return 1;

	if (mouse_unsubscribe_int() != 0)
	{
		printf("\n kbd_unsubscribe_int() failure \n");
		return 1;
	}

	mouse_read_outb();

	printf("Operation sucessfull \n");

	return 0;
}

int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}
