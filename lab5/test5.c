#include "test5.h"
#include "vbe.h"
#include "i8254.h"
#include "i8042.h"
#include "video_gr.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include "timer.h"
#include "xpm.h"
#include <stdint.h>
#include <sys/types.h>



#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES			  768
#define BITS_PER_PIXEL	  8

void *test_init(unsigned short mode, unsigned short delay) {
	
	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return NULL;
	}

	char *video_mem;
	video_mem = vg_init(mode);
	if(video_mem == NULL)
	{
		printf("test_init(): vg_init() failure \n");
		return NULL;
	}

	int control_timer = timer_subscribe_int();

	if (control_timer < 0)
	{
		printf("test_init(): timer_subscribe_int() failure \n");
		return NULL;
	}

	// para termos o endere�o voltamos a fazer vbe_get_mode_info()
	vbe_mode_info_t mode_info;
	if(vbe_get_mode_info(mode, &mode_info))
		return NULL;

	int ipc_status, r;
	int irq_set_timer = BIT(control_timer);
	message msg;
	int counter = 0;

	while (counter < delay * TIMER_DEFAULT_VALUE)
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
				if (msg.NOTIFY_ARG & irq_set_timer)
				{
					counter++;
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

	if (timer_unsubscribe_int() != 0)
	{
		printf("test_init(): timer_unsubscribe_int() failure \n");
		return NULL;
	}

	if(vg_exit())
	{
		printf("test_init(): vg_exit() failure \n");
		return NULL;
	}
	printf("Physical address of the video ram: 0x%X\n", mode_info.PhysBasePtr);
	printf("Operation sucessfull \n");
	return;
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	//Inicializes the 0x105 graphic mode
	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return 1;
	}

	char *video_mem;
	video_mem = vg_init(MODE_105);
	if(video_mem == NULL)
	{
		printf("test_init(): vg_init() failure \n");
		return 1;
	}

	int control = kbd_subscribe_int();

	if (control < 0)
	{
		printf("\n kbd_subscribe_int() failure \n");
		return -1;
	}

	vg_draw_square(x, y, size, color);

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
					long scancode;
					scancode = kbd_int_handler();

					if(scancode == -1)
						running = 0;
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
		return 1;
	}

	if(vg_exit())
	{
		printf("test_init(): vg_exit() failure \n");
		return 1;
	}

	printf("Operation sucessfull \n");
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	//Inicializes the 0x105 graphic mode
	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return 1;
	}

	char *video_mem;
	video_mem = vg_init(MODE_105);
	if(video_mem == NULL)
	{
		printf("test_init(): vg_init() failure \n");
		return 1;
	}

	vg_draw_line(xi, yi, xf, yf, color);

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

					long scancode;
					scancode = kbd_int_handler();
					if(scancode == -1)
						running = 0;
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
		return 1;
	}

	if(vg_exit())
	{
		printf("test_init(): vg_exit() failure \n");
		return 1;
	}
	
	printf("Operation sucessfull \n");
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[])
{
	//Inicializes the 0x105 graphic mode
	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return 1;
	}

	char *video_mem;
	video_mem = vg_init(MODE_105);
	if(video_mem == NULL)
	{
		printf("test_init(): vg_init() failure \n");
		return 1;
	}

	vbe_mode_info_t mode_info;

	if (vbe_get_mode_info(MODE_105, &mode_info))
	{
		printf("Error getting vbe mode");
		return 1;
	}

	int width, height;
	char* pixmap = read_xpm(xpm, &width, &height);

	vg_draw_pixmap(xi, yi, pixmap, width, height);

	free(pixmap);

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


					long scancode;
					scancode = kbd_int_handler();
					if(scancode == -1)
						running = 0;
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
		return 1;
	}

	if(vg_exit())
	{
		printf("test_init(): vg_exit() failure \n");
		return 1;
	}
	
	printf("Operation sucessfull \n");
	return 0;
	
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time)
{
	if(hor != 0 && hor != 1)
		return 1;

	//Inicializes the 0x105 graphic mode
	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return 1;
	}

	char *video_mem;
	video_mem = vg_init(MODE_105);
	if(video_mem == NULL)
	{
		printf("test_init(): vg_init() failure \n");
		return 1;
	}

	vbe_mode_info_t mode_info;

	if (vbe_get_mode_info(MODE_105, &mode_info))
	{
		printf("Error getting vbe mode");
		return 1;
	}

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

	int width, height;
	char* pixmap = read_xpm(xpm, &width, &height);
	double speed;

	start_dbl_buffer(width, height);

	double xdbl = xi;
	double ydbl = yi;

	speed = (double) (delta) / time;
	double moved = 0;

	int ipc_status, r;
	int irq_set_kbd = control_kbd; //irq_set is a bitmask with the bits corresponding to the hook_id values used in the sys_irqsetpolicy() set to 1
	int irq_set_timer = BIT(control_timer);
	message msg;
	int running = 1;
	int timer_counter = 0;

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

					// Esc to exit
					long scancode;
					scancode = kbd_int_handler();
					if(scancode == -1)
						running = 0;
				}
				if ((msg.NOTIFY_ARG & irq_set_timer) && (timer_counter <= TIMER_DEFAULT_VALUE * time))
				{
					// Drawing routine
					printf("rebenta A \n");
					vg_double_buffer_clear(width, height);
					printf("rebenta B \n");
					vg_draw_pixmap_dbl(pixmap, width, height);
					printf("rebenta C \n");
					buffer_to_screen(width, height);
					printf("rebenta D \n");

					// Update Coorinates
					if(hor)
					{
						xdbl = xdbl + speed;
						xi = (int) xdbl;
					}
					else
					{
						ydbl = ydbl + speed;
						yi = (int) ydbl;
					}

					moved += speed;
					if(moved > delta)
						running = 0;


					timer_counter++;
					if(timer_counter > TIMER_DEFAULT_VALUE * time)
						running = 0;
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

	free(pixmap);

	if (kbd_unsubscribe_int() != 0)
	{
		printf("\n kbd_unsubscribe_int() failure \n");
		return 1;
	}

	if (timer_unsubscribe_int() != 0)
	{
		printf("\n timer_unsubscribe_int() failure \n");
		return -1;
	}

	if(vg_exit())
	{
		printf("test_init(): vg_exit() failure \n");
		return 1;
	}
	
	printf("Operation sucessfull \n");

	return 0;
}					

int test_controller()
{

	if(lm_init() == NULL)
	{
		printf("test_init(): lm_init() failure \n");
		return NULL;
	}

	vbe_info_block_t controller_info;
	if(vbe_get_controller_info(&controller_info))
		return NULL;


	printf("Capabilities: \n");
		if(controller_info.Capabilities & CONTROLLER_CAPABILITIES_DAC_BIT)
			printf("   DAC width is switchable to 8 bits per primary color \n");
		else
			printf("   DAC is fixed width, with 6 bits per primary color \n");

		if(controller_info.Capabilities & CONTROLLER_CAPABILITIES_VGA_BIT)
			printf("   Controller is not VGA compatible \n");
		else
			printf("   Controller is VGA compatible \n");

		if(controller_info.Capabilities & CONTROLLER_CAPABILITIES_RAMDAC_BIT)
			printf("   When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h \n");
		else
			printf("   Normal RAMDAC operation \n");

		printf("Size of VRAM memory : %lu KB \n", controller_info.TotalMemory * 64);
	printf("List of mode numbers supported: \n");

	unsigned *p;

	for (p = (controller_info.VideoModePtr << 4 &  controller_info.VideoModePtr ) ; *p !=(unsigned)-1; p++)
	{
		if (*p == MODE_100)
		{
	       printf("   Mode 0x100: Resolution 640x400 and 256 Colors \n");
	    }
		if (*p == MODE_101)
		{
	       printf("   Mode 0x101: Resolution 640x480 and 256 Colors \n");
	    }
		if (*p == MODE_102)
		{
	       printf("   Mode 0x102: Resolution 800x600 and 16 Colors \n");
	    }
		if (*p == MODE_103)
		{
	       printf("   Mode 0x103: Resolution 800x600 and 256 Colors \n");
	    }
		if (*p == MODE_104)
		{
	       printf("   Mode 0x104: Resolution 1024x768 and 16 Colors \n");
	    }
		if (*p == MODE_105)
		{
	       printf("   Mode 0x105: Resolution 1024x768 and 256 Colors \n");
	    }
		if (*p == MODE_106)
		{
		   printf("   Mode 0x106: Resolution 1280x768 and 16 Colors \n");
	    }
		if (*p == MODE_107)
		{
	       printf("   Mode 0x107: Resolution 1280x768 and 256 Colors \n");
	    }

	}

	printf("Operation sucessfull \n");
	return;

}					
	
