#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Functions
#define BIT(n) (0x01<<(n))

/*---------------------------------------- KBC ---------------------------------------*/

// Regists KBC
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60
#define KBC_STAT_REG 0x64
#define KBC_CTRL_REG 0x64
#define KBC_IRQ 1
#define SECOND_BYTE 0xE0

// KBC Keys
#define ESC_KEYCODE 0x81

#define ARROWS_MAKECODE 0x2A
#define ARROWS_BREAKCODE 0xAA

#define LEFT_ARROW_MAKECODE 0x4B
#define LEFT_ARROW_BREAKCODE 0xCB

#define RIGHT_ARROW_MAKECODE 0x4D
#define RIGHT_ARROW_BREAKCODE 0xCD

#define SPACE_MAKECODE 0x39
#define SPACE_BREAKCODE 0xB9

// KBC register
#define OUTPUT_BUFFER BIT(0)
#define PARITY BIT(7)
#define TIMEOUT BIT(6)


/*--------------------------------------- MOUSE ---------------------------------------*/

#define PACKET_SIZE 3

// Regists Mouse
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

// Mouse Bits
#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

// Acknowledgments
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

//KBC register
#define OUTPUT_BUFFER BIT(0)
#define INPUT_BUFFER BIT(1)
#define PARITY BIT(7)
#define TIMEOUT BIT(6)



#endif
