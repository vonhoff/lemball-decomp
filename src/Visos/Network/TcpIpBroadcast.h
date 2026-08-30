#ifndef LEMBALL_VISOS_NETWORK_TCPIPBROADCAST_H
#define LEMBALL_VISOS_NETWORK_TCPIPBROADCAST_H

#include "../../Common.h"
#include "Broadcast.h"     // complete type
#include "TcpIpRwSocket.h" // complete type

// SIZE 0x16c
// VTABLE: LEMBALL 0x0049a158 native async callback
// VTABLE: LEMBALL 0x0049a160 ReadSocket view
// VTABLE: LEMBALL 0x0049a188 WriteSocket view
// VTABLE: LEMBALL 0x0049a1b8 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049a1c8 primary Broadcast view
class TcpIpBroadcast : public Broadcast, public TcpIpRwSocket {
public:
	TcpIpBroadcast();
	bool Start(const char* p_name);
	virtual int Process();                            // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer);  // vtable+0x0c
	virtual void GetSpecificAddr(const char* p_name); // vtable+0x08
	virtual void StopListen();                        // vtable+0x18
	void GotHost();
	void GotName();
	void StartListen();
	~TcpIpBroadcast();

private:
	Broadcast m_base;                    // 0x00
	undefined m_platformComposite[0x44]; // 0x128
};

// SYNTHETIC: LEMBALL 0x00471f60
// TcpIpBroadcast::`scalar deleting destructor'

#endif
