#ifndef __GRAPHICS_H
#define __GRAPHICS_H
/** @defgroup Graphics Graphics
 * @{
 * Functions for initializing the graphics
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "VBE.h"
#define SKY rgb(148, 224, 255)
/**
 * @brief Initializes the graphics
 * @param mode video memory mode
 */
void *initVideoGraphics(unsigned short mode);
/**
 * @brief Get the video memory address
 * @return pointer to the video memory address
 */
char* getVideoMem();
/**
 * @brief Get the buffer address
 * @return pointer to the buffer address
 */
char* getBuffer();
/**
 * @brief Get the video memory horizontal resolution
 * @return video memory horizontal resolution
 */
int getHorResolution();
/**
 * @brief Get the video memory vertical resolution
 * @return video memory vertical resolution
 */
int getVerResolution();
/**
 * @brief Get the video memory bytes per pixel
 * @return video memory bytes per pixel
 */
int getBytesPerPixel();
/**
 * @brief Copies buffer to video memory
 */
void BufferToVideoMem();
/**
 * @brief Get an color on rgb mode, 16 bits
 * @param r red portion of color
 * @param g green portion of color
 * @param b blue portion of color
 * @return color on rgb mode, 16 bits
 */
int rgb(unsigned char r, unsigned char g, unsigned char b);
/**
 * @brief Exits graphic mode
 * @return 1 if error, 0 if sucessfull
 */
int exitVideoGraphics();
/**
 * @brief Fills the entire screen with only one color
 * @param color color to fill
 * @return 0 if sucessfull
 */
int fillDisplay(int color);

#endif
