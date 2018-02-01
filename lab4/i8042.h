#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Regists
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60
#define KBC_STAT_REG 0x64
#define KBC_CTRL_REG 0x64
#define KBC_IRQ 1
#define ESC_KEYCODE 0x81
#define SECOND_BYTE 0xE0
#define KBD_SWITCH_LEDS 0xED
#define LEDS_RESEND 0xFE
#define LEDS_ERROR 0xFC


//Regists Mouse
#define MOUSE_ENABLE 0xA8
#define MOUSE_DISABLE 0xA7
#define KBC_WRITE_MOUSE 0xD4
#define MOUSE_IRQ 12
#define MOUSE_STREAM_ENABLE 0xF4
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_STREAM_DISABLE 0xF5
#define MOUSE_READ_DATA 0xEB
#define MOUSE_STATUS_REQ 0xE9
#define MOUSE_RESET 0xFF

//Bits
#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#define STATUS_REMOTE BIT(6)
#define STATUS_ENABLE BIT(5)
#define STATUS_SCALING BIT(4)
#define STATUS_LEFT_BUTTON BIT(2)
#define STATUS_MIDDLE_BUTTON BIT(1)
#define STATUS_RIGHT_BUTTON BIT(0)

//Functions
#define BIT(n) (0x01<<(n))

//KBC register
#define OUTPUT_BUFFER BIT(0)
#define INPUT_BUFFER BIT(1)
#define PARITY BIT(7)
#define TIMEOUT BIT(6)

//Acknowledgments
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

#define PACKET_SIZE 3
#define STATUS_SIZE 3
#endif
