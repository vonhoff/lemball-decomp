#ifndef LEMBALL_VISOS_MESSAGING_CBASEPACKET_H
#define LEMBALL_VISOS_MESSAGING_CBASEPACKET_H

// SIZE 0x08
// VTABLE: LEMBALL 0x004991cc
class CBasePacket {
public:
	virtual ~CBasePacket() {} // vtable+0x00

	friend class CNetworkOptionsProc;
	friend class CBaseFrontendProcess;
	friend class CBroadcast;
	friend class CGodManager;
	friend class CWriteSocket;
	friend class CReadNcmsBuff;
	friend class CReadCmsBuff;

protected:
	unsigned char* m_data; // 0x04
};

// SYNTHETIC: LEMBALL 0x004629e0
// CBasePacket::`scalar deleting destructor'

#endif
