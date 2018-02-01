#ifndef __SPRITE_H
#define __SPRITE_H

#include <minix/syslib.h>
#include <minix/drivers.h>

typedef struct {
int x, y; // current position
int width, height; // dimensions
int xspeed, yspeed; // current speed
char *map; // the pixmap
} Sprite;

Sprite *create_sprite(char *pic[], char *bas);
void destroy_sprite(Sprite *sp, char *base);
int animate_sprite(Sprite *sp, char *base);
static int check_collision(Sprite *sp, char *base) ;
static int draw_sprite(Sprite *sp, char *base);


#endif
