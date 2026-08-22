#ifndef LEMBALL_VISOS_NETWORK_BROADCAST_H
#define LEMBALL_VISOS_NETWORK_BROADCAST_H

#include "../../Common.h"
#include "ReadSocket.h"  // complete type
#include "WriteSocket.h" // complete type

// SIZE 0x128
// VTABLE: LEMBALL 0x00499080 ReadSocket virtual base
// VTABLE: LEMBALL 0x004990a8 WriteSocket virtual base
// VTABLE: LEMBALL 0x004990d8 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x004990e8 primary
class Broadcast : public virtual ReadSocket, public virtual WriteSocket {
public:
	Broadcast();
	short FindPort(const unsigned char* p_arg0);
	void AddToMessage(BroadcastMessage& p_arg0);
	void Closed(unsigned char p_arg0);
	void Initialise(const char* p_arg0);
	void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1);
	void Process();
	void ResetPort(short p_arg0);
	void Run();
	void Send(NetworkAddress* p_arg0, BroadcastMessage& p_arg1);
	void SendFailedInit(NetworkErrors p_arg0);
	void SetSpecificAddr(const char* p_arg0);
	void Stop();
	void Suspend();
	~Broadcast();

private:
	unsigned int m_runEnabled;         // 0x08
	unsigned int m_addressMode;        // 0x0c
	unsigned char* m_connectionData;   // 0x10
	undefined4 m_unk0x14;              // 0x14
	unsigned int m_lastBroadcastTime;  // 0x18
	NetworkAddress* m_specificAddress; // 0x1c
};

#endif
