#ifndef LEMBALL_VISOS_FOUNDATION_MESSAGE_H
#define LEMBALL_VISOS_FOUNDATION_MESSAGE_H

#define INPUT_KEY_UP 1
#define INPUT_KEY_DOWN 2
#define INPUT_KEY_LEFT 3
#define INPUT_KEY_RIGHT 4
#define INPUT_KEY_A 5
#define INPUT_KEY_B 0x06
#define INPUT_KEY_C 0x07
#define INPUT_KEY_D 0x08
#define INPUT_KEY_E 0x09
#define INPUT_KEY_F 0x0a
#define INPUT_KEY_G 0x0b
#define INPUT_KEY_H 0x0c
#define INPUT_KEY_I 0x0d
#define INPUT_KEY_J 0x0e
#define INPUT_KEY_K 0x0f
#define INPUT_KEY_L 0x10
#define INPUT_KEY_M 0x11
#define INPUT_KEY_N 0x12
#define INPUT_KEY_O 0x13
#define INPUT_KEY_P 0x14
#define INPUT_KEY_Q 0x15
#define INPUT_KEY_R 0x16
#define INPUT_KEY_S 0x17
#define INPUT_KEY_T 0x18
#define INPUT_KEY_U 0x19
#define INPUT_KEY_V 0x1a
#define INPUT_KEY_W 0x1b
#define INPUT_KEY_X 0x1c
#define INPUT_KEY_Y 0x1d
#define INPUT_KEY_Z 0x1e
#define INPUT_KEY_SPACE 0x1f
#define INPUT_KEY_PERIOD 0x20
#define INPUT_KEY_COMMA 0x21
#define INPUT_KEY_ESCAPE 0x23
#define INPUT_KEY_F4 0x25
#define INPUT_KEY_0 0x39
#define INPUT_KEY_1 0x3a
#define INPUT_KEY_2 0x3b
#define INPUT_KEY_3 0x3c
#define INPUT_KEY_4 0x3d
#define INPUT_KEY_5 0x3e
#define INPUT_KEY_6 0x3f
#define INPUT_KEY_7 0x40
#define INPUT_KEY_8 0x41
#define INPUT_KEY_9 0x42
#define INPUT_KEY_SHIFT 0x49
#define INPUT_KEY_LEFT_SHIFT 0x4a
#define INPUT_KEY_RETURN 0x4c
#define INPUT_KEY_DELETE 0x4d
#define INPUT_KEY_BACKSPACE 0x4e

#define INPUT_DIGIT_ASCII_OFFSET 9

#define INPUT_MOUSE_LEFT 0x43
#define INPUT_MOUSE_RIGHT 0x44
#define INPUT_MOUSE_MIDDLE 0x45
#define INPUT_MOUSE_LEFT_DOUBLE_CLICK 0x46
#define INPUT_MOUSE_RIGHT_DOUBLE_CLICK 0x47
#define INPUT_MOUSE_MIDDLE_DOUBLE_CLICK 0x48

#define MESSAGE_BUTTON_PRESSED 0x0b
#define MESSAGE_BUTTON_RELEASED 0x0c
#define MESSAGE_BUTTON_ENTERED 0x0d
#define MESSAGE_BUTTON_EXITED 0x0e
#define MESSAGE_WINDOW_COMMAND 0x0f

// SIZE 0x14
struct Message {
	unsigned short m_type;     // 0x00
	unsigned short m_reserved; // 0x02
	unsigned int m_time;       // 0x04
	int m_code;                // 0x08
	void* m_payload;           // 0x0c
	void* m_source;            // 0x10
};

#endif
