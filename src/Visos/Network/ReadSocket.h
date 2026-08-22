#ifndef LEMBALL_VISOS_NETWORK_READSOCKET_H
#define LEMBALL_VISOS_NETWORK_READSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h" // complete type
#include "BaseSocket.h"       // complete type

// SIZE 0x84
// VTABLE: LEMBALL 0x00498f60 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x00498f70 primary
class ReadSocket : public BaseSocket, public virtual BaseCommonSocket {
public:
	ReadSocket();
	bool IsChanged(NetworkMessage& p_arg0);
	bool ProcessPacket();
	void DeleteCBuffers();
	void DeleteNcBuffers();
	void FirstReceive();
	void GetLatest(NetworkMessage& p_arg0);
	void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1);
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void UnUseAllC();
	void UnUseAllNc();
	~ReadSocket();

private:
	unsigned int m_lastReceiveTime;         // 0x3c
	ReadNcBuff* m_nonCriticalBuffer;        // 0x48
	ReadNcmsBuff* m_nonCriticalMultiBuffer; // 0x4c
	ReadCBuff* m_criticalBuffer;            // 0x50
	ReadCmsBuff* m_criticalMultiBuffer;     // 0x54
};

#endif
