#ifndef LEMBALL_VISOS_NETWORK_WRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_WRITESOCKET_H

#include "../../Common.h"
#include "../Messaging/BasePacketHeader.h" // complete type
#include "BaseCommonSocket.h"              // complete type
#include "BaseSocket.h"                    // complete type

// SIZE 0xa4
// VTABLE: LEMBALL 0x00498fa0 BaseCommonSocket
// VTABLE: LEMBALL 0x00498fb0 BaseSocket
class WriteSocket : public BaseSocket, public virtual BaseCommonSocket {
public:
	WriteSocket();
	virtual ~WriteSocket();                                               // vtable+0x14
	virtual void SendAcknowledgement();                                   // vtable+0x18
	virtual NetworkMessage* ReceiveAcknowledgement();                     // vtable+0x1c
	virtual void SetDestAddr(NetworkAddress* p_arg0) = 0;                 // vtable+0x20
	virtual bool SendPacket(const unsigned char* p_data, int p_size) = 0; // vtable+0x24
	// FUNCTION: LEMBALL 0x00462970
	virtual void SetPort(short p_arg0) // vtable+0x28
	{
		m_port = (unsigned short) p_arg0;
	}
	bool Send(NetworkMessage& p_arg0);
	bool SendCritical(NetworkMessage& p_arg0);
	bool SendNcms(NetworkMessage& p_arg0);
	bool ResendCritical(WritePacket* p_arg0);
	void Closed(unsigned char p_arg0);
	void DeleteCBuffers();
	void DeleteNcBuffers();
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);

	friend class BaseObjectManager;
	friend class BaseNetwork;
	friend class Broadcast;

protected:
	void _SetDestAddr(NetworkAddress* p_arg0);

private:
	WriteNcBuff* m_nonCriticalBuffer;     // 0x48
	WriteCBuff* m_criticalBuffer;         // 0x4c
	WriteCBuff* m_unknown50;              // 0x50
	NetworkMessage* m_segmentedMessage;   // 0x54
	unsigned int m_segmentSequence;       // 0x58
	int m_segmentIndex;                   // 0x5c
	BasePacketHeader m_savedHeader;       // 0x60
	NetworkAddress* m_destinationAddress; // 0x70
};

#endif
