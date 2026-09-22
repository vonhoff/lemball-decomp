#ifndef LEMBALL_VISOS_NETWORK_CREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_CREADSOCKET_H

#include "CBaseCommonSocket.h" // complete type
#include "CBaseSocket.h"       // complete type
class CNetworkMessage;

class CBasePacket;
class CReadCBuff;
class CReadCMSBuff;
class CReadNCBuff;
class CReadNCMSBuff;

// SIZE 0x84
// VTABLE: LEMBALL 0x00498f60 CBaseCommonSocket
// VTABLE: LEMBALL 0x00498f70 CBaseSocket
class CReadSocket : public CBaseSocket, public virtual CBaseCommonSocket {
public:
	CReadSocket();
	bool IsChanged(CNetworkMessage& p_message);
	bool ProcessPacket();
	void DeleteCBuffers();
	void DeleteNCBuffers();
	virtual void FirstReceive();                           // vtable+0x18
	virtual void SendAcknowledgement() = 0;                // vtable+0x1c
	virtual CNetworkMessage* ReceiveAcknowledgement() = 0; // vtable+0x20
	void GetLatest(CNetworkMessage& p_message);
	virtual void PostRead(NetworkEvents p_event, CBasePacket* p_packet); // vtable+0x24
	void Process();
	void SetCBuffers(int p_packetCount, int p_messageCapacity);
	void SetNCBuffers(unsigned long p_lastSinglePacketMessageId, unsigned long p_lastMessageId, int p_messageCapacity);
	void UnUseAllC();
	void UnUseAllNC();
	virtual ~CReadSocket(); // vtable+0x14

private:
	CReadNCBuff* m_nonCriticalBuffer;        // 0x48
	CReadNCMSBuff* m_nonCriticalMultiBuffer; // 0x4c
	CReadCBuff* m_criticalBuffer;            // 0x50
	CReadCMSBuff* m_criticalMultiBuffer;     // 0x54
};

// SYNTHETIC: LEMBALL 0x00462940
// CReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462ba0
// CReadSocket::`vector deleting destructor'

#endif
