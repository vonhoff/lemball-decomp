#ifndef LEMBALL_VISOS_MESSAGING_WRITEPACKET_H
#define LEMBALL_VISOS_MESSAGING_WRITEPACKET_H

#include "../../Common.h"
#include "BasePacket.h" // complete type

// SIZE 0x18
class WritePacket : public BasePacket {
public:
	WritePacket(unsigned short p_arg0);
	void Fill(const unsigned char* p_arg0, unsigned short p_arg1, NetworkMessage* p_arg2);
	~WritePacket();

private:
	unsigned char m_available;   // 0x08
	NetworkMessage* m_message;   // 0x0c
	unsigned int m_lastSendTime; // 0x10
	unsigned char m_retryCount;  // 0x14

	friend class WriteSocket;
	friend class WritePacketBuff;
	friend class WriteCBuff;
};

#endif
