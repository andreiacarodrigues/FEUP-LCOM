#include <minix/drivers.h>
#include "GameLoop.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
  /* Initialize service */

  sef_startup();
  sys_enable_iop(SELF);
  if ( argc == 1 )
  {
	  startGame(0x117);
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
}

static int proc_args(int argc, char *argv[]) {
}

static unsigned long parse_ulong(char *str, int base) {
}

static long parse_long(char *str, int base) {
}
