#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "VBE.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


/*
 * vbe_get_mode_info():
 * returns 0 if sucessful
 */

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t m;
	if(lm_alloc(sizeof(vmi_p), &m) == NULL)
		return 1;

	/*
	 Info:
	 AH = 0x4F
	 AL = 0x01
	 CX = mode
	 ES:DI = pointer to ModeInfoBlock struct
	 */

	struct reg86u reg86;
	reg86.u.b.intno = VBE_INTERRUPT;
	reg86.u.b.ah = VBE_FUNCTION;
	reg86.u.b.al = VBE_MODE_INFORMATION_FUNC;
	reg86.u.w.cx = mode;
	reg86.u.w.es = PB2BASE(m.phys);
	reg86.u.w.di = PB2OFF(m.phys);

	if( sys_int86(&reg86) == OK )
	{
		if(reg86.u.b.ah == VBE_FUNCTION_SUCESSFUL && reg86.u.b.al== VBE_FUNCTION)
		{
			*vmi_p = *((vbe_mode_info_t *)m.virtual);
			lm_free(&m);
			return 0;
		}
	}

	lm_free(&m);
	return 1;
}
