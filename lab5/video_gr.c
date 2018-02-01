#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "video_gr.h"
#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES			  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode)
{
	 struct reg86u reg86;

	 reg86.u.b.intno = VBE_INTERRUPT; 				/* BIOS video services ; intno = 0x10  -> Interruption for the video card */

	 reg86.u.b.ah = VBE_FUNCTION; 				  	// AH = 0x4F -> Vai invocar nova fun��o
	 reg86.u.b.al = VBE_MODE;  						// AL = 0x02 -> Referente � fun��o que inicial o VBE Mode
	 reg86.u.w.bx = (mode | FLAT_BUFFER_MODE_BIT);	// BX = mode number -> BIT(14) tem de ser = 1 para ser em flat buffer mode

	 vbe_mode_info_t mode_info;

	 if (sys_int86(&reg86) == OK)
	 {
		 //AX register vai devolver a resposta da fun��o :
	   	//se AH = 0x00 e AL = 0x4F deu bem
		//sen�o deu erro

		if(reg86.u.b.ah == VBE_FUNCTION_SUCESSFUL && reg86.u.b.al== VBE_FUNCTION)
		{
			if(vbe_get_mode_info(mode, &mode_info))
				return NULL;

			h_res = mode_info.XResolution;
			v_res = mode_info.YResolution;
			bits_per_pixel = mode_info.BitsPerPixel;

			struct mem_range mem_r;
			int mem_r_size = 0;
			int r;

			//Allowing Memory mapping

			mem_r.mr_base = (phys_bytes)mode_info.PhysBasePtr; //lowest memory address in range
			mem_r_size = ((h_res * v_res * bits_per_pixel)/8); //memory size needed
			mem_r.mr_limit = mem_r.mr_base + mem_r_size; //highest memory address in range

			if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_r)))
			{
				printf("vg_init(): sys_privctl (ADD_MEM) failed: %d\n", r);
				return NULL;
			}

			//Map memory
			 video_mem = vm_map_phys(SELF, (void *)mem_r.mr_base, mem_r_size);

			 if(video_mem == MAP_FAILED)
			 {
				 printf("vg_init() couldn't map video memory \n");
				 return NULL;
			 }
			 else
				 return video_mem;
		}
	}
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

static void swap (unsigned short * a, unsigned short * b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void vg_change_pixel(unsigned short x, unsigned short y, unsigned long color)
{
	unsigned int pos = (x + y * H_RES) * (bits_per_pixel/8);
	video_mem[pos] = color;
	return;
}

void vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color)
{
	if(x + size > H_RES || y + size > V_RES)
	{
		printf("vg_draw_square(): outside the boundaries");
		return;
	}

	unsigned int i;
	unsigned int j;

	for(i = x; i < x+size; i++)
	{
		for(j = y; j < y+size; j++)
		{
			vg_change_pixel(i, j, color);
		}
	}

	return;
}

void vg_draw_line(unsigned short xi, unsigned short yi,
        unsigned short xf, unsigned short yf, unsigned long color)
{
	unsigned short dx = abs(xf - xi);
	unsigned short dy = abs(yf - yi);

	unsigned int slope = 0;
	unsigned int inc_y = 0;
	unsigned int p = 2 * dy - dx;

	if(dy > dx)
	{
		swap(&xi, &yi);
		swap(&xf, &yf);
		swap(&dx, &dy);
		slope = 1;
	}
	if(xi > xf)
	{
		swap(&xi, &xf);
		swap (&yi, &yf);
	}
	if(yi > yf)
	{
		inc_y = -1;
	}
	else
		inc_y = 1;


	while(xi < xf)
	{
		if(p <= 0)
		{
			p += 2 * dy;
		}
		else
		{
			p += 2 * (dy - dx);
			yi += inc_y;
		}
		xi++;

		if(slope)
			vg_change_pixel(yi, xi, color);
		else
			vg_change_pixel(xi, yi, color);
	}

	return;
}

void vg_draw_pixmap(int xi,int yi, char* pixmap, int width, int height)
{
	int i;
	int j;

	for(i = 0; i < width; ++i)
		for (j = 0; j < height; ++j)
			vg_change_pixel(xi + i, yi + j, *(pixmap + (i + j * width) * bits_per_pixel / 8));
}

// Double Buffering

void start_dbl_buffer(int width, int height)
{
	printf("entrei no start_dbl_buffer \n");

	double_buffer = (char*) malloc (width * height);
}

void vg_change_pixel_dbl_buffer(unsigned short x, unsigned short y, unsigned long color)
{
	printf("entrei no vg_change_pixel_dbl_buffer \n");

	unsigned int pos = (x + y * H_RES);
	printf("AAAentrei no vg_change_pixel_dbl_buffer \n");
	printf("%d", pos);

	double_buffer[pos] = color;
	printf("BBBentrei no vg_change_pixel_dbl_buffer \n");
	return;
}

void vg_double_buffer_clear(int width, int height)
{
	printf("entrei no vg_double_buffer_clear \n");

	memset(double_buffer, 0, width * height);
	return;
}

void vg_draw_pixmap_dbl(char* pixmap, int width, int height)
{
	printf("entrei no vg_draw_pixmap_dbl \n");

	int i;
	int j;

	for(i = 0; i < width; ++i)
		for (j = 0; j < height; ++j)
			vg_change_pixel_dbl_buffer(i, j, *(pixmap + (i + j * width) * bits_per_pixel / 8));

	return;
}

void buffer_to_screen(int width, int height)
{
	printf("entrei no buffer_to_screen \n");

	memcpy(double_buffer,video_mem, width * height  * bits_per_pixel / 8);
}
