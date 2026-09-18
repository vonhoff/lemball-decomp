#ifndef LEMBALL_VISOS_FOUNDATION_MESSAGE_H
#define LEMBALL_VISOS_FOUNDATION_MESSAGE_H

// SIZE 0x14
struct Message {
	unsigned short type;     // 0x00
	unsigned short reserved; // 0x02
	unsigned int time;       // 0x04
	int code;                // 0x08
	void* payload;           // 0x0c
	void* source;            // 0x10
};

#endif
