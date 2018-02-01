#include "test3.h"
#include "i8254.h"
#include "timer.h"

// Faz ciclo while que lê até que scancode seja o sinal de break code da tecla ESC
int kbd_test_scan(unsigned short ass) //<- 0 para C; diferente de  0 para assembly
{
	int control = kbd_subscribe_int();

	if (control < 0)
	{
		printf("\n kbd_subscribe_int() failure \n");
		return -1;
	}

	int ipc_status, r;
	int irq_set = control; //irq_set is a bitmask with the bits corresponding to the hook_id values used in the sys_irqsetpolicy() set to 1
	message msg;
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
				if (msg.NOTIFY_ARG & irq_set)
				{ /* subscribed interrupt */

					int handlerStatus;

					if(ass)
					// Código em ASM
					{
						if(kbd_int_handler_verify())
							return 1;

						long assScancode;
						assScancode = kbd_int_handler_asm();

						if(assScancode == ESC_KEYCODE)
							running = 0;

						kbd_display(assScancode);
					}
					else
					// Código em C
					{
						handlerStatus = kbd_int_handler();

						if(handlerStatus)
							return handlerStatus;

						if(handlerStatus == -1)
						{
							running = 0;
						}
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
	if (kbd_unsubscribe_int() != 0)
	{
		printf("\n kbd_unsubscribe_int() failure \n");
		return -1;
	}

	printf("Operation sucessfull \n");
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds)
{
	unsigned int i = 0;
	unsigned long led_status;

	//All the leds are off at start
	unsigned int scroll_lock = 0;
	unsigned int num_lock = 0;
	unsigned int caps_lock = 0;
	// Status of all leds
	unsigned long switch_leds;

    for(i; i < n; i++) //até chegar ao final do array vai percorrer todos os elementos
    {
    	if(sys_outb(KBC_OUT_BUF, KBD_SWITCH_LEDS))
    	{
    		printf("Error uploading the led status to the keyboard outbuffer \n");
    		return 1;
    	}
    	if(sys_inb(KBC_OUT_BUF, &led_status))
    	{
    		printf("Error reading the led status \n");
    		return 1;
    	}

    	if(led_status == LEDS_RESEND)
    	{
    		printf("Error - latest byte must be written again \n");
    		i--;
    	}
    	if(led_status == LEDS_ERROR)
    	{
    		printf("Error - the entire sequence must be restarted \n");
    		i = 0;
    	}

    	switch(leds[i])
    	{
    	//Scroll Lock
    	case 0:
    		if(scroll_lock)
    		{
    			printf("Scroll Lock Off \n");
    			scroll_lock = 0;
    		}
    		else
    		{
    			printf("Scroll Lock On \n");
    			scroll_lock = BIT(0);
    		}
    		break;
    	// Num Lock
    	case 1:
    		if(num_lock)
    		{
    		   	printf("Numeric Lock Off \n");
    			num_lock = 0;
    		}
    		else
    		{
    			printf("Numeric Lock On \n");
    			num_lock = BIT(1);
    		}
    		break;
    	// Caps Lock
    	case 2:
    		if(caps_lock)
    	   	{
    	       	printf("Caps Lock Off \n");
    	    	caps_lock = 0;
    	   	}
    	   	else
    	   	{
    	    	printf("Caps Lock On \n");
    	   		caps_lock = BIT(2);
    	   	}
    		break;
    	default:
    		{
    			printf("Error - leds argument must be between 0 and 2 \n");
    			break;
    		}
    	}
    	switch_leds = scroll_lock | num_lock | caps_lock;

    	if(sys_outb(KBC_OUT_BUF, switch_leds))
    	{
    		printf("Error uploading the led status to the keyboard outbuffer \n");
    		return 1;
    	}

    	if(sys_inb(KBC_OUT_BUF, &led_status))
		{
		    printf("Error reading the led status \n");
		    return 1;
		}

    	tickdelay(micros_to_ticks(1000000)); // esperar 1 segundo
    }

    printf("Operation sucessfull \n");
}

int kbd_test_timed_scan(unsigned short n)
{
	int control_kbd = kbd_subscribe_int();

	if (control_kbd < 0)
	{
		printf("\n kbd_subscribe_int() failure \n");
		return -1;
	}

	int control_timer = timer_subscribe_int();

		if (control_timer < 0)
		{
			printf("\n timer_subscribe_int() failure \n");
			return -1;
		}


		int ipc_status, r;
		int irq_set_kbd = control_kbd;
		int irq_set_timer = BIT(control_timer);
		message msg;
		int running = 1;

		int counter = 0;

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
					if (msg.NOTIFY_ARG & irq_set_kbd)
					{ /* subscribed interrupt */
						counter = 0;

						int handlerStatus;

						handlerStatus = kbd_int_handler();

						if(handlerStatus)
							return handlerStatus;

						if(handlerStatus == -1)
						{
							running = 0;
						}
					}

					if (msg.NOTIFY_ARG & irq_set_timer)
					{
						if(counter > n*TIMER_DEFAULT_VALUE)
						{
							running = 0;
							printf("Operation timed out. \n");
						}
						else
						{
							kbd_timer_int_handler(&counter);
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
		if (kbd_unsubscribe_int() != 0)
		{
			printf("\n kbd_unsubscribe_int() failure \n");
			return -1;
		}

		printf("Operation sucessfull \n");
		return 0;
}
