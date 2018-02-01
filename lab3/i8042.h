#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Regists KBC
#define KBC_OUT_BUF 0x60
#define KBC_STAT_REG 0x64
#define KBC_CTRL_REG 0x64
#define KBC_IRQ 1
#define ESC_KEYCODE 0x81
#define SECOND_BYTE 0xE0
#define KBD_SWITCH_LEDS 0xED
#define LEDS_RESEND 0xFE
#define LEDS_ERROR 0xFC

//Functions
#define BIT(n) (0x01<<(n))

//KBC register
#define OUTPUT_BUFFER BIT(0)
#define PARITY BIT(7)
#define TIMEOUT BIT(6)

#endif
