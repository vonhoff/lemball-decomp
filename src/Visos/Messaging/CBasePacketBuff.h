#ifndef LEMBALL_VISOS_MESSAGING_CBASEPACKETBUFF_H
#define LEMBALL_VISOS_MESSAGING_CBASEPACKETBUFF_H

class CBasePacket;

// SIZE 0x0c
class CBasePacketBuff {
public:
	CBasePacketBuff(int p_arg0, unsigned short p_arg1);
	~CBasePacketBuff();
	CBasePacketBuff();

protected:
	int m_packetCount;         // 0x00
	unsigned int m_packetSize; // 0x04
	CBasePacket** m_packets;   // 0x08

	friend class CReadSocket;
	friend class CWriteSocket;
};

#endif
