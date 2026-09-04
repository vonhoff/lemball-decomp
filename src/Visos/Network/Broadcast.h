#ifndef LEMBALL_VISOS_NETWORK_BROADCAST_H
#define LEMBALL_VISOS_NETWORK_BROADCAST_H

#include "../../Common.h"
#include "RwSocket.h" // complete type

// SIZE 0x128
// VTABLE: LEMBALL 0x00499080 ReadSocket virtual base
// VTABLE: LEMBALL 0x004990a8 WriteSocket virtual base
// VTABLE: LEMBALL 0x004990d8 BaseCommonSocket virtual base
// VTABLE: LEMBALL 0x004990e8 primary
class Broadcast : public virtual RwSocket {
public:
	Broadcast();
	virtual short FindPort(const unsigned char* p_arg0);  // vtable+0x00
	virtual void ResetPort(short p_arg0);                 // vtable+0x04
	virtual void GetSpecificAddr(const char* p_arg0) = 0; // vtable+0x08
	virtual bool Start(const char* p_arg0) = 0;           // vtable+0x0c
	virtual void Process();                               // vtable+0x10
	virtual void StartListen() = 0;                       // vtable+0x14
	virtual void StopListen() = 0;                        // vtable+0x18
	void AddToMessage(BroadcastMessage& p_arg0);
	void Closed(unsigned char p_arg0);
	void Initialise(const char* p_arg0);
	void PostRead(NetworkEvents p_arg0, BasePacket* p_arg1);
	void Run();
	void Send(NetworkAddress* p_arg0, BroadcastMessage& p_arg1);
	void SendFailedInit(NetworkErrors p_arg0);
	void SetSpecificAddr(const char* p_arg0);
	void Stop();
	void Suspend();
	~Broadcast();

	friend class BaseNetwork;
	friend class TcpIpBroadcast;

private:
	unsigned int m_runEnabled;         // 0x08
	unsigned int m_addressMode;        // 0x0c
	unsigned char* m_connectionData;   // 0x10
	undefined4 m_unk0x14;              // 0x14
	unsigned int m_lastBroadcastTime;  // 0x18
	NetworkAddress* m_specificAddress; // 0x1c
};

#endif
