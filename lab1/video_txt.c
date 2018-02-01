#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr)
{
	int i = 0;
	char *vptr;         /* Pointer to video RAM */
	vptr = video_mem;   /* Point to first byte of video RAM */

	for(i = 0; i < scr_lines * scr_width; i++)
	{
	  *vptr = ch;        /* First byte, is for the character to display */
	  vptr++;             /* Point to second byte of video RAM */
	  *vptr = attr;  /* ... which is for the attributes of the first character */
	  vptr++;
	}
}

void vt_blank()
{
	vt_fill(0x20, 0x00);
}

int vt_print_char(char ch, char attr, int r, int c)
{
	// Error Verification
	if(r > 25 || c > 80)
	{
		printf("Error: Outside the boundaries");
		return;
	}

	char *vptr;         /* Pointer to video RAM */
	vptr = video_mem;   /* Point to first byte of video RAM */

	vptr+= ((r-1) * scr_width + (c-1))*2;
	*vptr = ch;
    vptr++;
    *vptr = attr;
}

int vt_print_string(char *str, char attr, int r, int c)
{
	char *vptr;         /* Pointer to video RAM */
	vptr = video_mem;   /* Point to first byte of video RAM */

	vptr+= ((r-1) * scr_width + (c-1))*2;

	int length = strlen(str);
	int i = 0;

	// Error Verification
	if(r > 25 || (c+length) > 80)
	{
		printf("Error: Outside the boundaries");
		return;
	}

	for(i = 0; i < length; i++)
	{
		*vptr = *str;
		vptr++;
		*vptr = attr;
		vptr++;
		str++;
	}
}

int vt_print_int(int num, char attr, int r, int c)
{
	char *vptr;         /* Pointer to video RAM */
	vptr = video_mem;   /* Point to first byte of video RAM */

	vptr+= ((r-1) * scr_width + (c-1))*2;

	int number_size = 0;
	int num_copy = num;

	char str[10];

	char *cptr;
	cptr = str;

	sprintf(cptr, "%d", num);

	int length = strlen(str);
	int i = 0;

	// Error Verification
	if(r > 25 || (c+length) > 80)
	{
		printf("Error: Outside the boundaries");
		return;
	}

	for(i = 0; i < length; i++)
	{
		*vptr = *cptr;
		vptr++;
		*vptr = attr;
		vptr++;
		cptr++;
	}
}


int vt_draw_frame(int width, int height, char attr, int r, int c)
{
	if((r + width) > 25 || (c + height) > 80)
	{
		// vt_print_string("error", 07, 2, 2);
		printf("Error: Outside the boundaries");
		return;
	}

	char *vptr;         /* Pointer to video RAM */
	vptr = video_mem;   /* Point to first byte of video RAM */

	vptr+= ((r-1) * scr_width + (c-1))*2;

	int i = 0;
	int j = 0;

	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			if(j == 0 && i == 0)
			{
				*vptr = 0xC9;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if(j == 0 && i == width - 1)
			{
				*vptr = 0xBB;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if(j == height - 1 && i == 0)
			{
				*vptr = 0xC8;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if(j == height - 1 && i == width - 1)
			{
				*vptr = 0xBC;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if((j == 0 || j == height - 1) && (i != 0) && (i != width - 1))
			{
				*vptr = 0xCD;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if((i == 0 || i == width - 1) && (j != 0) && (j != height - 1))
			{
				*vptr = 0xBA;
				vptr++;
				*vptr = attr;
				vptr++;
			}

			if(i != 0 && i != width -1 && j != 0 && j != height - 1)
			{
				vptr+= 2;
			}
		}
		vptr+= (scr_width - width)*2; // Change line
	}
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
