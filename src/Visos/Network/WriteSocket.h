#ifndef LEMBALL_VISOS_NETWORK_WRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_WRITESOCKET_H

#include "../../Common.h"
#include "../Messaging/BasePacketHeader.h" // complete type
#include "BaseCommonSocket.h"              // complete type
#include "BaseSocket.h"                    // complete type

// SIZE 0xa4
// VTABLE: LEMBALL 0x00498fa0 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x00498fb0 primary
class WriteSocket : public BaseSocket, public virtual BaseCommonSocket {
public:
	NetworkMessage* ReceiveAcknowledgement();
	WriteSocket();
	bool Send(NetworkMessage& p_arg0);
	bool SendCritical(NetworkMessage& p_arg0);
	bool SendNcms(NetworkMessage& p_arg0);
	char ResendCritical(WritePacket* p_arg0);
	void Closed(unsigned char p_arg0);
	void DeleteCBuffers();
	void DeleteNcBuffers();
	void Process();
	void SendAcknowledgement();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetDestAddr(NetworkAddress* p_arg0);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void SetPort(short p_arg0);
	~WriteSocket();

private:
	unsigned int m_lastSendTime;          // 0x3c
	WriteNcBuff* m_nonCriticalBuffer;     // 0x48
	WriteCBuff* m_criticalBuffer;         // 0x4c
	NetworkMessage* m_segmentedMessage;   // 0x54
	unsigned int m_segmentSequence;       // 0x58
	int m_segmentIndex;                   // 0x5c
	BasePacketHeader m_savedHeader;       // 0x60
	NetworkAddress* m_destinationAddress; // 0x70
};

#endif
