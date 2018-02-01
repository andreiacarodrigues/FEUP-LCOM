# Makefile for the lab5 driver 

COMPILER_TYPE= gnu

CC=gcc

PROG=	project
SRCS+=	project.c
SRCS+=  Bitmap.c Graphics.c VBE.c Keyboard.c Timer.c Mouse.c RTC.c
SRCS+=  GameLoop.c LoadGame.c ScoreManager.c Character.c Enemy.c Camara.c Platform.c Coins.c Spikes.c Boxes.c Flag.c Clouds.c

CCFLAGS= -Wall -03

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys  

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>