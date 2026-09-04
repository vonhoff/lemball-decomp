#ifndef LEMBALL_VISOS_NETWORK_READSOCKET_H
#define LEMBALL_VISOS_NETWORK_READSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h" // complete type
#include "BaseSocket.h"       // complete type

// SIZE 0x84
// VTABLE: LEMBALL 0x00498f60 BaseCommonSocket
// VTABLE: LEMBALL 0x00498f70 BaseSocket
class ReadSocket : public BaseSocket, public virtual BaseCommonSocket {
public:
	ReadSocket();
	bool IsChanged(NetworkMessage& p_arg0);
	bool ProcessPacket();
	void DeleteCBuffers();
	void DeleteNcBuffers();
	virtual void FirstReceive();                          // vtable+0x18
	virtual void SendAcknowledgement() = 0;               // vtable+0x1c
	virtual NetworkMessage* ReceiveAcknowledgement() = 0; // vtable+0x20
	void GetLatest(NetworkMessage& p_arg0);
	virtual void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1); // vtable+0x24
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void UnUseAllC();
	void UnUseAllNc();
	virtual ~ReadSocket(); // vtable+0x14

private:
	ReadNcBuff* m_nonCriticalBuffer;        // 0x48
	ReadNcmsBuff* m_nonCriticalMultiBuffer; // 0x4c
	ReadCBuff* m_criticalBuffer;            // 0x50
	ReadCmsBuff* m_criticalMultiBuffer;     // 0x54
};

// SYNTHETIC: LEMBALL 0x00462940
// ReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00462ba0
// ReadSocket::`vector deleting destructor'

#endif
