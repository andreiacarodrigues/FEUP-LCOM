#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

char *double_buffer;

void vg_change_pixel(unsigned short x, unsigned short y, unsigned long color);
void vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);
void vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);
void vg_draw_pixmap(int xi,int yi, char* pixmap, int width, int height);

void start_dbl_buffer(int width, int height);
void vg_change_pixel_dbl_buffer(unsigned short x, unsigned short y, unsigned long color);
void vg_double_buffer_clear(int width, int height);
void vg_draw_pixmap_dbl(char* pixmap, int width, int height);
void buffer_to_screen(int width, int height);


/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
