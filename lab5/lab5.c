#include <minix/drivers.h>

#include "vbe.h"
#include "test5.h"
#include "video_gr.h"
#include "pixelmap.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
  /* Initialize service */

  sef_startup();

  printf("\n Lab5: PC's video card in graphics mode (T6G15)\n");

  if ( argc == 1 )
  {
      print_usage(argv);
      return 0;
  }
  else
  {
      proc_args(argc, argv);
  }

  return 0;
}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"init <mode> <delay>\" \n"
	 "\t service run %s -args \"square <x> <y> <size> <color>\" \n"
	 "\t service run %s -args \"line <xi> <yi> <xf> <yf> <color>\" \n"
	 "\t service run %s -args \"xpm <xi> <yi> <xpm>\" \n"
	 "\t service run %s -args \"move <xi> <yi> <xpm> <hor> <delta> <time>\" \n"
	 "\t service run %s -args \"controller\" \n",
	 argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned long mode, delay, x, y, size, color, xi, yi, xf, yf, xpm, hor, delta, time;
  char *str;
  long num;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "init", strlen("init")) == 0) {
	  if( argc != 4 ) {
		  printf("video_gr: wrong no of arguments for test of test_init() \n");
		  return 1;
	  }
	  if( (mode = parse_ulong(argv[2], 16)) == ULONG_MAX )
	 	  return 1;
	  if( (delay = parse_ulong(argv[3], 10)) == ULONG_MAX )
	 	  return 1;

	  printf("video_gr:: test_init(%u, %u)\n", mode, delay);
	  test_init(mode, delay);
	  return 0;
  } else if (strncmp(argv[1], "square", strlen("square")) == 0) {
	  if( argc != 6 ) {
		  printf("video_gr: wrong no of arguments for test of test_square() \n");
		  return 1;
	  }
	  if( (x = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  if( (y = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (size = parse_ulong(argv[4], 10)) == ULONG_MAX )
		 return 1;
	  if( (color = parse_ulong(argv[5], 16)) == ULONG_MAX )
		 return 1;
	  printf("video_gr:: test_square(%u, %u, %u, %u)\n", x, y, size, color);

	  return  test_square(x, y, size, color);
  } else if (strncmp(argv[1], "line", strlen("line")) == 0) {
	  if( argc != 7 ) {
		  printf("video_gr: wrong no of arguments for test of test_line() \n");
		  return 1;
	  }
	  if( (xi = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  if( (yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (xf = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if( (yf = parse_ulong(argv[5], 10)) == ULONG_MAX )
		  return 1;
	  if( (color = parse_ulong(argv[6], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_gr:: test_line(%u, %u, %u, %u, %u)\n", xi, yi, xf, yf, color);
	  return test_line(xi, yi, xf, yf, color);
  } else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
	  if( argc != 5 ) {
		  printf("video_gr: wrong no of arguments for test of test_xpm() \n");
		  return 1;
	  }
	  if( (xi = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  if( (yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (xpm = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	 if(pixmap_get(xpm) == NULL)
	  {
		  printf("video_gr: pixmap not found \n");
		  return 1;
	  }
	 printf("video_gr:: test_xpm(%u, %u, pixmaps[%u])\n", xi, yi, xpm);
	 return test_xpm(xi, yi, pixmap_get(xpm));
  } else if (strncmp(argv[1], "move", strlen("move")) == 0) {
	  if( argc < 8) {
		  printf("video_gr: wrong no of arguments for test of test_move() \n");
		  return 1;
	  }
	  if( (xi = parse_long(argv[2], 10)) == LONG_MAX )
		  return 1;
	  if( (yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (xpm = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if(pixmap_get(xpm) == NULL)
	  {
		  printf("video_gr: pixmap not found \n");
		  return 1;
	  }
	  if( (hor = parse_long(argv[5], 10)) == LONG_MAX )
	 		  return 1;
	  if( (delta = parse_ulong(argv[6], 10)) == ULONG_MAX )
	 		 return 1;
	  if( (time = parse_ulong(argv[7], 10)) == ULONG_MAX )
	 		 return 1;
	  printf("video_gr:: test_move(%u, %u, pixmaps[%u], %u, %u, %u)\n",
		xi, yi, xpm, hor, delta, time);
	  return test_move(xi, yi, pixmap_get(xpm), hor, delta, time);
  } else
	  if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
	  	  if( argc != 2 ) {
	  		  printf("video_gr: wrong no of arguments for test of test_controller() \n");
	  		  return 1;
	  	  }
	  	  printf("video_gr:: test_controller()\n"); /* Actually, it was already invoked */
	  	  test_controller();
	  	  return 0;
  } else {
	  printf("video_gr: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
