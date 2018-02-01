#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test4.h"


static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
  /* Initialize service */
  sef_startup();
  /* Enable IO-sensitive operations for ourselves */
    //sys_enable_iop(SELF);

   printf("\n Lab4: The PS/2 Mouse (T6G15)\n");

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
			"\t service run %s -args \"test_packet <number of interrupts>\" \n"
			"\t service run %s -args \"test_async <time interval>\" \n"
			"\t service run %s -args \"test_config\" \n"
			"\t service run %s -args \"test_gesture <length of vertical movement> <tolerance>\" \n" ,
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[])
{
	 unsigned long cnt, idle_time, length, tolerance;

	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0)
	{
	  if( argc != 3 )
	  {
		  printf("mouse: wrong number of arguments for test of test_packet() \n");
		  return 1;
	  }

	 if( (cnt = parse_ulong(argv[2], 10)) == ULONG_MAX )
			  return 1;

	 printf("mouse:: test_packet(%u)\n", (unsigned short)cnt);
	 test_packet(cnt);
	 return 0;
	}
	 else
	  {
		 if (strncmp(argv[1], "test_async", strlen("test_async")) == 0)
		 {
		 	 if( argc != 3 )
		 	 {
		 		 printf("mouse: wrong number of arguments for test of test_async() \n");
		 		 return 1;
		 	 }
		 	 if( (idle_time = parse_ulong(argv[2], 10)) == ULONG_MAX )
		 		 return 1;

		 	 printf("mouse:: test_async(%u)\n",(unsigned short)idle_time);
		 	test_async(idle_time);

		 	 return 0;
		  }

		 else
		  {
		 	if (strncmp(argv[1], "test_config", strlen("test_config")) == 0)
			{
				if( argc != 2 )
				{
					printf("mouse: wrong number of arguments for test of test_config() \n");
					return 1;
				}

				printf("mouse:: test_config(%u)\n");
				test_config();

				return 0;
			}
			else
			{
				if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0)
				{
					if( argc != 4 )
					{
						printf("mouse: wrong number of arguments for test of test_gesture() \n");
						return 1;
					}
					if( (length = parse_ulong(argv[2], 10)) == ULONG_MAX )
					return 1;

					if( (tolerance = parse_ulong(argv[2], 10)) == ULONG_MAX )
					return 1;

					printf("mouse:: test_gesture(%d, %u)\n",(short)length, (unsigned short) tolerance);
					test_gesture(length, tolerance);

					return 0;
				}
			}
		}
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
	  printf("mouse: parse_ulong: no digits were found in %s \n", str);
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
	  printf("mouse: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}

