#ifndef LEMBALL_VISOS_MESSAGING_CWRITEPACKET_H
#define LEMBALL_VISOS_MESSAGING_CWRITEPACKET_H

#include "CBasePacket.h"

class CNetworkMessage;

// SIZE 0x18
// VTABLE: LEMBALL 0x004991d0
class CWritePacket : public CBasePacket {
public:
	CWritePacket(unsigned short p_capacity);
	void Fill(const unsigned char* p_data, unsigned short p_size, CNetworkMessage* p_message);
	~CWritePacket();

private:
	unsigned char m_available;   // 0x08
	CNetworkMessage* m_message;  // 0x0c
	unsigned int m_lastSendTime; // 0x10
	unsigned char m_retryCount;  // 0x14

	friend class CWriteSocket;
	friend class CWritePacketBuff;
	friend class CWriteCBuff;
};

// SYNTHETIC: LEMBALL 0x00462a20
// CWritePacket::`scalar deleting destructor'

#endif
