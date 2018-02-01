#include "timer.h"
#include <minix/syslib.h>
#include <minix/drivers.h>

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
  /* Initialize service */
  sef_startup();

   printf("\n Lab2: PC's timer/counter and speaker (T6G15)\n");

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

static void print_usage(char *argv[])
{
	 printf("\n Usage: one of the following:\n"
		 "\t service run %s -args \"config <timer - 0, 1 or 2>\" \n"
		 "\t service run %s -args \"square <frequency>\" \n"
		 "\t service run %s -args \"int <time interval> \" \n",
		 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[])
{
	 unsigned long timer, frequency, time_interval;

	if (strncmp(argv[1], "config", strlen("config")) == 0)
	{
	  if( argc != 3 )
	  {
		  printf("timer: wrong number of arguments for test of timer_test_config() \n");
		  return 1;
	  }

	 if( (timer = parse_ulong(argv[2], 10)) == ULONG_MAX )
			  return 1;

	 printf("timer:: timer_test_config(%lu)\n", (unsigned long)timer);
	 timer_test_config(timer);
	 return 0;
	}
	 else
	  {
		 if (strncmp(argv[1], "square", strlen("square")) == 0)
		 {
		 	 if( argc != 3 )
		 	 {
		 		 printf("timer: wrong number of arguments for test of timer_test_square() \n");
		 		 return 1;
		 	 }
		 	 if( (frequency = parse_ulong(argv[2], 10)) == ULONG_MAX )
		 		 return 1;

		 	 printf("timer:: timer_test_square(%lu)\n",(unsigned long)frequency);
		 	 timer_test_square(frequency);

		 	 return 0;
		  }

		 else
		 {
			if (strncmp(argv[1], "int", strlen("int")) == 0)
			{
			  if( argc != 3 )
			  {
				  printf("timer: wrong number of arguments for test of timer_test_int() \n");
				  return 1;
			  }

			  if( (time_interval = parse_ulong(argv[2], 10)) == ULONG_MAX )
				  return 1;

			  printf("timer:: timer_test_int(%lu)\n",(unsigned long)time_interval);
			  timer_test_int(time_interval);
			  return 0;
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
	  printf("timer: parse_ulong: no digits were found in %s \n", str);
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
	  printf("timer: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
