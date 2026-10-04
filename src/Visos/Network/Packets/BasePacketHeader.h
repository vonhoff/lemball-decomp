#ifndef LEMBALL_VISOS_MESSAGING_BASEPACKETHEADER_H
#define LEMBALL_VISOS_MESSAGING_BASEPACKETHEADER_H

#define BASE_PACKET_MAGIC 0x56533039
#define BASE_PACKET_UNSEGMENTED 0x100

// SIZE 0x10
struct BasePacketHeader {
	unsigned int m_magic;               // 0x00
	unsigned int m_packetSize;          // 0x04
	unsigned short m_messageId;         // 0x08
	unsigned short m_packetSequence;    // 0x0a
	unsigned short m_subpacketSequence; // 0x0c
	unsigned char m_critical;           // 0x0e
	unsigned char m_reserved;           // 0x0f
};

#endif
