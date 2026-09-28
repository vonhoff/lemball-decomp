#ifndef LEMBALL_VISOS_FOUNDATION_MESSAGE_H
#define LEMBALL_VISOS_FOUNDATION_MESSAGE_H

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
