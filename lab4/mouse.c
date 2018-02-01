#include "mouse.h"

#define N_TRIES 100
static unsigned int hook_ID = MOUSE_IRQ; // hook_ID = 12
static unsigned char packet[PACKET_SIZE];


int mouse_subscribe_int()
{
	unsigned int hook_temporary = hook_ID;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_ID) != OK)
	{

			printf("\n sys_irqsetpolicy() failure");
			return -1;
	}
	if (sys_irqenable(&hook_ID) != OK)
	{
		printf("\n sys_irqenable() failure");
		return -1;
	}

	return BIT(hook_temporary);
}

int mouse_set_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_MODE))
	{
		printf("Error seting the mouse in stream mode \n");
		return 1;
	}
	return 0;
}
int mouse_enable_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_ENABLE))
	{
		printf("Error enabling the mouse in stream mode \n");
		return 1;
	}
	return 0;
}

int mouse_read_inb()
{
	unsigned long controlReg;
	unsigned int i = 0;

	for(i; i < N_TRIES; i++)
	{
		if(sys_inb(KBC_STAT_REG, &controlReg)) //tenta ler do status register
		{
			printf("Error checking the status register \n");
			return 1;
		}

		if(!(controlReg & INPUT_BUFFER)) //input buffer está cheio se for = 1
			return 0;

		tickdelay(micros_to_ticks(20000)); //se nao conseguir espera um segundo e tenta outra vez
	}
	return 1;
}

int mouse_read_outb()
{
	unsigned long controlReg;
	unsigned int i = 0;

	for(i; i < N_TRIES; i++)
	{
		if(sys_inb(KBC_STAT_REG, &controlReg)) //tenta ler do status register
		{
			printf("Error checking the status register \n");
			return -1;
		}

		if(!(controlReg & OUTPUT_BUFFER)) // vê se o outbuffer está cheio
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
 //------------------
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

int mouse_disable_stream_mode()
{
	if(mouse_write(MOUSE_STREAM_DISABLE))
	{
		printf("Error disabling stream mode \n");
		return 1;
	}
	return 0;
}

int mouse_unsubscribe_int() {

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

