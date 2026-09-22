#ifndef LEMBALL_VISOS_NETWORK_CREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_CREADSOCKET_H

#include "CBaseCommonSocket.h" // complete type
#include "CBaseSocket.h"       // complete type
class CNetworkMessage;

class CBasePacket;
class CReadCBuff;
class CReadCmsBuff;
class CReadNcBuff;
class CReadNcmsBuff;

// SIZE 0x84
// VTABLE: LEMBALL 0x00498f60 CBaseCommonSocket
// VTABLE: LEMBALL 0x00498f70 CBaseSocket
class CReadSocket : public CBaseSocket, public virtual CBaseCommonSocket {
public:
	CReadSocket();
	bool IsChanged(CNetworkMessage& p_arg0);
	bool ProcessPacket();
	void DeleteCBuffers();
	void DeleteNcBuffers();
	virtual void FirstReceive();                           // vtable+0x18
	virtual void SendAcknowledgement() = 0;                // vtable+0x1c
	virtual CNetworkMessage* ReceiveAcknowledgement() = 0; // vtable+0x20
	void GetLatest(CNetworkMessage& p_arg0);
	virtual void PostRead(NetworkEvents p_arg0, CBasePacket* p_arg1); // vtable+0x24
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void UnUseAllC();
	void UnUseAllNc();
	virtual ~CReadSocket(); // vtable+0x14

private:
	CReadNcBuff* m_nonCriticalBuffer;        // 0x48
	CReadNcmsBuff* m_nonCriticalMultiBuffer; // 0x4c
	CReadCBuff* m_criticalBuffer;            // 0x50
	CReadCmsBuff* m_criticalMultiBuffer;     // 0x54
};

// SYNTHETIC: LEMBALL 0x00462940
// CReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462ba0
// CReadSocket::`vector deleting destructor'

#endif
