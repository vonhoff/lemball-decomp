#ifndef LEMBALL_VISOS_MESSAGING_CREADPACKET_H
#define LEMBALL_VISOS_MESSAGING_CREADPACKET_H

#include "../Foundation/CCritical.h"
#include "CBasePacket.h" // complete type

// SIZE 0x2c
// VTABLE: LEMBALL 0x004991c0 CCritical
// VTABLE: LEMBALL 0x004991c8 CBasePacket
class CReadPacket : public CBasePacket, public CCritical {
public:
	CReadPacket(unsigned short p_capacity);
	virtual ~CReadPacket();
	void Fill(const unsigned char* p_data, unsigned short p_size);

	friend class CNetworkOptionsProc;
	friend class CNetworkManager;
	friend class CBaseFrontendProcess;
	friend class CBroadcast;
	friend class CGodManager;
	friend class CReadPacketBuff;
	friend class CReadNcBuff;
	friend class CReadCBuff;
	friend class CReadSocket;

private:
	unsigned int m_used;  // 0x24
	unsigned int m_ready; // 0x28
};

// SYNTHETIC: LEMBALL 0x00462a00
// CReadPacket::`scalar deleting destructor'

#endif
