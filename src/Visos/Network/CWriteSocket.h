#ifndef LEMBALL_VISOS_NETWORK_CWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_CWRITESOCKET_H

#include "../Messaging/BasePacketHeader.h" // complete type
#include "CBaseCommonSocket.h"             // complete type
#include "CBaseSocket.h"                   // complete type
class CNetworkMessage;

class CNetworkAddress;
class CWriteCBuff;
class WriteNcBuff;
class CWritePacket;

// SIZE 0xa4
// VTABLE: LEMBALL 0x00498fa0 CBaseCommonSocket
// VTABLE: LEMBALL 0x00498fb0 CBaseSocket
class CWriteSocket : public CBaseSocket, public virtual CBaseCommonSocket {
public:
	CWriteSocket();
	virtual ~CWriteSocket();                                              // vtable+0x14
	virtual void SendAcknowledgement();                                   // vtable+0x18
	virtual CNetworkMessage* ReceiveAcknowledgement();                    // vtable+0x1c
	virtual void SetDestAddr(CNetworkAddress* p_arg0) = 0;                // vtable+0x20
	virtual bool SendPacket(const unsigned char* p_data, int p_size) = 0; // vtable+0x24
	// FUNCTION: LEMBALL 0x00462970
	virtual void SetPort(short p_arg0) // vtable+0x28
	{
		m_port = p_arg0;
	}
	bool Send(CNetworkMessage& p_arg0);
	bool SendCritical(CNetworkMessage& p_arg0);
	bool SendNcms(CNetworkMessage& p_arg0);
	bool ResendCritical(CWritePacket* p_packet);
	void Closed(int p_notifyPeer);
	void DeleteCBuffers();
	void DeleteNcBuffers();
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);

	friend class CBaseObjectManager;
	friend class CBaseNetwork;
	friend class CBroadcast;
	friend class CFileWriteSocket;
	friend class CNetworkOptionsDrawer;

protected:
	void InternalSetDestAddr(CNetworkAddress* p_address);

private:
	WriteNcBuff* m_nonCriticalBuffer;      // 0x48
	CWriteCBuff* m_criticalBuffer;         // 0x4c
	CWriteCBuff* m_unknown50;              // 0x50
	CNetworkMessage* m_segmentedMessage;   // 0x54
	unsigned int m_segmentSequence;        // 0x58
	int m_segmentIndex;                    // 0x5c
	BasePacketHeader m_savedHeader;        // 0x60
	CNetworkAddress* m_destinationAddress; // 0x70
};

// SYNTHETIC: LEMBALL 0x00462990
// CWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462d80
// CWriteSocket::`scalar deleting destructor'`vtordisp{-4, 120}'

#endif
