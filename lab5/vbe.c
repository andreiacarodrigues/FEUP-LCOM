#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
	mmap_t m;
	if(lm_alloc(sizeof(vmi_p), &m) == NULL) //Allocates a memory block in the 1MByte memory area with the specified size
	{
		printf("vg_get_mode_info(): error allocating memory block in the 1MByte region \n");
		return 1;
	}

	struct reg86u reg86;
	reg86.u.b.intno = VBE_INTERRUPT;				// intno = 0x10 -> Interruption for the video card
	reg86.u.b.ah = VBE_FUNCTION;					// AH = 0x4F -> Vai invocar nova fun��o
	reg86.u.b.al = VBE_MODE_INFORMATION_FUNC;		// AL = 0x01 -> Referente � fun��o que retorna o VBE Mode Information
	reg86.u.w.cx = mode;							// Mode number
	reg86.u.w.es = PB2BASE(m.phys);					// ES:DI pointer para ModeInfoBlock structure que vai ser preenchida com a informa��o
	reg86.u.w.di = PB2OFF(m.phys);
  
	if( sys_int86(&reg86) == OK ) // retorna 'ok' se for bem sucedido
	{

		//AX register vai devolver a resposta da fun��o :
		//se AH = 0x00 e AL = 0x4F deu bem
		//sen�o deu erro

		if(reg86.u.b.ah == VBE_FUNCTION_SUCESSFUL && reg86.u.b.al== VBE_FUNCTION)
		{
			*vmi_p = *((vbe_mode_info_t *)m.virtual); // initializing vmi_p com a ModeInfoBlock struct que preenchemos
			lm_free(&m); // Frees a memory block in the 1MByte memory area, previously allocated using lm_alloc()
			return 0;
		}

	}

	printf("vg_get_mode_info(): sys_int86() failed \n");
	lm_free(&m); // Frees a memory block in the 1MByte memory area, previously allocated using lm_alloc()
	return 1;
}


int vbe_get_controller_info(vbe_info_block_t *vmi_p)
{
	mmap_t m;

	if(lm_alloc(sizeof(vmi_p), &m) == NULL) //Allocates a memory block in the 1MByte memory area with the specified size
	{
		printf("vg_get_mode_info(): error allocating memory block in the 1MByte region \n");
		return 1;
	}

	vmi_p->VbeSignature[0] = 'V'; vmi_p->VbeSignature[1] = 'B'; vmi_p->VbeSignature[2] = 'E'; vmi_p->VbeSignature[3] = '2';

	//colocar VBE2 no VbeSignature((VbeSignature should be set to 'VBE2' when function is called
	//to indicate VBE 2.0 information is desired and the information block is 512 bytes in size.)

	struct reg86u reg86;
	reg86.u.b.intno = VBE_INTERRUPT;					// intno = 0x10 -> Interruption for the video card
	reg86.u.b.ah = VBE_FUNCTION;						// AH = 0x4F -> Vai invocar nova fun��o
	reg86.u.b.al = VBE_CONTROLLER_INFORMATION_FUNC;		// AL = 0x00 -> Referente � fun��o que retorna o VBE Controller Information
	reg86.u.w.es = PB2BASE(m.phys);						// ES:DI pointer para ModeInfoBlock structure que vai ser preenchida com a informa��o
	reg86.u.w.di = PB2OFF(m.phys);


	if( sys_int86(&reg86) == OK ) // retorna 'ok' se for bem sucedido
	{

		//AX register vai devolver a resposta da fun��o :
		//se AH = 0x00 e AL = 0x4F deu bem
		//sen�o deu erro

		if(reg86.u.b.ah == VBE_FUNCTION_SUCESSFUL && reg86.u.b.al== VBE_FUNCTION)
		{
			*vmi_p = *((vbe_info_block_t *)m.virtual); // initializing vmi_p com a ModeInfoBlock struct que preenchemos
			lm_free(&m); // Frees a memory block in the 1MByte memory area, previously allocated using lm_alloc()
			return 0;
		}

	}

	printf("vbe_get_controller_info(): sys_int86() failed \n");
	lm_free(&m); // Frees a memory block in the 1MByte memory area, previously allocated using lm_alloc()
	return 1;

}












