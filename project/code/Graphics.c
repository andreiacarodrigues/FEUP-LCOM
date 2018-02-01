#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "VBE.h"

/*
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES			  768
#define BITS_PER_PIXEL	  8
*/

/* ------------------------------------------------------------- Variables ---------------------------------------------------------------*/

static char *video_mem;
static char *buffer;

static unsigned h_res;
static unsigned v_res;
static unsigned video_memory_size;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;

/* -------------------------------------------------------------- Functions --------------------------------------------------------------*/

void *initVideoGraphics(unsigned short mode)
{
	 struct reg86u reg86;

	 /*
	  * Info:
	  * intno = x10
	  * AH = 0x4F
	  * AL = 0x02
	  * BX = mode | BIT(14)
	  */

	 reg86.u.b.intno = VBE_INTERRUPT;
	 reg86.u.b.ah = VBE_FUNCTION;
	 reg86.u.b.al = VBE_MODE;
	 reg86.u.w.bx = (mode | FLAT_BUFFER_MODE_BIT);

	 vbe_mode_info_t mode_info;

	 if (sys_int86(&reg86) == OK)
	 {
		if(reg86.u.b.ah == VBE_FUNCTION_SUCESSFUL && reg86.u.b.al== VBE_FUNCTION)
		{
			if(vbe_get_mode_info(mode, &mode_info))
				return NULL;

			h_res = mode_info.XResolution;
			v_res = mode_info.YResolution;
			bits_per_pixel = mode_info.BitsPerPixel;
			bytes_per_pixel = bits_per_pixel / 8;

			video_memory_size = h_res * v_res * bytes_per_pixel;

			struct mem_range mem_r;
			/* Allowing Memory mapping */
			mem_r.mr_base = (phys_bytes)mode_info.PhysBasePtr; //lowest memory address in range
			mem_r.mr_limit = mem_r.mr_base + video_memory_size; //highest memory address in range

			if( OK != (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_r)))
				return NULL;

			/* Map memory */
			 video_mem = vm_map_phys(SELF, (void *)mem_r.mr_base, video_memory_size);
			 buffer = (char*) malloc(video_memory_size);

			 if(video_mem == MAP_FAILED)
				 return NULL;
			 else
				 return video_mem;
		}
	}
}

char* getVideoMem() {	return video_mem;	}

char* getBuffer()	{	return buffer;	}

void initBuffer()	{	buffer = malloc(video_memory_size);	}

void bufferToVideoMem()	{	memcpy(video_mem, buffer, video_memory_size);	}

void clear_buffer() {	memset(buffer, 0, video_memory_size);	}

int getHorResolution()	{	return h_res;	}

int getVerResolution()	{	return v_res;	}

int getBytesPerPixel()	{	return bytes_per_pixel;	}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
	if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
		return -1;

	int red, green, blue;
	red = r * 31 / 255;
	green = g * 63 / 255;
	blue = b * 31 / 255;

	return (red << 11) | (green << 5) | blue;
}

int fillDisplay(int color) {
	char *ptr = buffer;

	// fill one line
	int i;
	for (i = 0; i < h_res; i++, ptr++) {
		*ptr = (char) (color & 0xFF);
		ptr++;
		*ptr = (char) ((color >> 8) & 0xFF);
	}

	// get pointer to that line
	int screenWidthBytes = h_res * bytes_per_pixel;

	// copy and paste first line
	for (i = 1; i < v_res; i++)
		memcpy(buffer + i * screenWidthBytes, buffer, screenWidthBytes);

	return 0;
}



int exitVideoGraphics() {
  struct reg86u reg86;

  /*
   * Info:
   * intno = 0x10
   * AH = 0x00
   * AL = 0x03
  */

  /* BIOS video services */
  reg86.u.b.intno = 0x10;

  /* Set Video Mode function */
  reg86.u.b.ah = 0x00;

  /* 80x25 text mode*/
  reg86.u.b.al = 0x03;

  if( sys_int86(&reg86) != OK )
      return 1;
  else
      return 0;
}
