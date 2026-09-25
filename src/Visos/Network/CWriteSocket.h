#ifndef LEMBALL_VISOS_NETWORK_CWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_CWRITESOCKET_H

#include "../Messaging/BasePacketHeader.h" // complete type
#include "CBaseCommonSocket.h"             // complete type
#include "CBaseSocket.h"                   // complete type
class CNetworkMessage;

class CNetworkAddress;
class CWriteCBuff;
class CWriteNcBuff;
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
	virtual void SetDestAddr(CNetworkAddress* p_address) = 0;             // vtable+0x20
	virtual bool SendPacket(const unsigned char* p_data, int p_size) = 0; // vtable+0x24
	// FUNCTION: LEMBALL 0x00462970
	virtual void SetPort(short p_port) // vtable+0x28
	{
		m_port = p_port;
	}
	bool Send(CNetworkMessage& p_message);
	bool SendCritical(CNetworkMessage& p_message);
	bool SendNCMS(CNetworkMessage& p_message);
	bool ResendCritical(CWritePacket* p_packet);
	void Closed(int p_notifyPeer);
	void DeleteCBuffers();
	void DeleteNCBuffers();
	void Process();
	void SetCBuffers(int p_packetCount, int p_messageCapacity);
	void SetNCBuffers(unsigned long p_lastSinglePacketMessageId, unsigned long p_lastMessageId, int p_messageCapacity);

	friend class CBaseObjectManager;
	friend class CBaseNetwork;
	friend class CBroadcast;
	friend class CFileWriteSocket;
	friend class CNetworkOptionsDrawer;

protected:
	void _SetDestAddr(CNetworkAddress* p_address);

private:
	CWriteNcBuff* m_nonCriticalBuffer;     // 0x48
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

// GLOBAL: LEMBALL 0x00498f98
// CWriteSocket::`vbtable'

#endif
