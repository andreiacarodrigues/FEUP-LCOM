	//.intel_syntax

	/* void kbd_int_handler_asm(); */

	#include "i8042.h"

	.global _kbd_int_handler_asm

	.data

	scancode: .long 0

	.text

	_kbd_int_handler_asm:
		mov $0, %eax
		in $KBC_OUT_BUF, %al
		mov %al, scancode
		ret $scancode

