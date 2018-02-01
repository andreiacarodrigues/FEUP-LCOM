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

  /* Enable IO-sensitive operations for ourselves */
  sys_enable_iop(SELF);

   printf("\n Lab3: message here (T6G15)\n");

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
		 "\t service run %s -args \"scan <language: 0 - C or 1 - Assembly>\" \n"
		 "\t service run %s -args \"timed_scan <time interval>\" \n"
		 "\t service run %s -args \"leds <array size> <array> <\" \n",
		 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[])
{
	 unsigned long language, size_array, array_element, time;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0)
	{
	  if( argc != 3 )
	  {
		  printf("kbd: wrong number of arguments for test of kbd_test_scan() \n");
		  return 1;
	  }

	 if( (language = parse_ulong(argv[2], 10)) == ULONG_MAX )
			  return 1;

	 printf("kbd:: kbd_test_scan(%u)\n", (unsigned short)language);
	 kbd_test_scan(language);
	 return 0;
	}
	 else
	  {
		 if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0)
		 {
		 	 if( argc != 3 )
		 	 {
		 		 printf("kbd: wrong number of arguments for test of kbd_test_timed_scan() \n");
		 		 return 1;
		 	 }
		 	 if( (time = parse_ulong(argv[2], 10)) == ULONG_MAX )
		 		 return 1;

		 	 printf("kbd:: kbd_test_timed_scan(%u)\n",(unsigned short)time);
		 	kbd_test_timed_scan(time);

		 	 return 0;
		  }

		 else
		  {
		 	if (strncmp(argv[1], "leds", strlen("leds")) == 0)
		 	{
		 	  if( argc <2 )
		 	  {
		 		  printf("kbd: wrong number of arguments for test of kbd_test_leds() \n");
		 		  return 1;
		 	  }
		 	  if( (size_array = parse_ulong(argv[2], 10)) == ULONG_MAX )
		 		  return 1;

		 	  unsigned int index = 0;
		 	  unsigned short leds_array[size_array];

		 	 for(index;index < size_array;index++)
		 		{
		 			if( (array_element = parse_ulong(argv[3+index], 10)) == LONG_MAX )
		 				return 1;
		 			leds_array[index] = array_element;
		 		}

		 		  printf("kbd:: kbd_test_leds(%u,[", size_array);

		 			for(index=0; index < size_array;index++)
		 		  	{
		 				if (index == (size_array - 1))
		 					printf("%u",leds_array[index]);
		 			  	else
		 			  		printf("%u,",leds_array[index]);
		 		  	}

		 			printf("])\n");

		 	 kbd_test_leds(size_array, leds_array);
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
	  printf("kbd: parse_ulong: no digits were found in %s \n", str);
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
	  printf("kbd: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
