#ifndef LEMBALL_SCAFFOLD_VISOS_MESSAGING_BASEPACKETHEADER_H
#define LEMBALL_SCAFFOLD_VISOS_MESSAGING_BASEPACKETHEADER_H

#include "../../Common.h"

// SIZE 0x10
struct BasePacketHeader {
public:
	// No functions mapped yet.

	unsigned int m_magic;               // 0x00
	unsigned int m_packetSize;          // 0x04
	unsigned short m_messageId;         // 0x08
	unsigned short m_packetSequence;    // 0x0a
	unsigned short m_subpacketSequence; // 0x0c
	unsigned char m_critical;           // 0x0e
	unsigned char m_reserved;           // 0x0f
};

#endif
