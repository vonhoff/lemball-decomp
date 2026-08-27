#ifndef LEMBALL_VISOS_NETWORK_TCPIPCONNECT_H
#define LEMBALL_VISOS_NETWORK_TCPIPCONNECT_H

#include "../../Common.h"
#include "Connect.h"       // complete type
#include "TcpIpRwSocket.h" // complete type

// SIZE 0x170
// VTABLE: LEMBALL 0x0049a368 native async callback
// VTABLE: LEMBALL 0x0049a370 ReadSocket view
// VTABLE: LEMBALL 0x0049a398 WriteSocket view
// VTABLE: LEMBALL 0x0049a3c8 BaseCommonSocket view
// VTABLE: LEMBALL 0x0049a3d8 primary Connect view
class TcpIpConnect : public Connect, public TcpIpRwSocket {
public:
	bool Start(const char* p_localName, const char* p_remoteName);
	int Process();
	virtual bool InitSocket();                       // vtable+0x08
	virtual void Closed(unsigned char p_notifyPeer); // vtable+0x0c
	virtual void Connect();                          // vtable+0x10
	virtual void ReceiveAcknowledgement();           // vtable+0x1c
	virtual ~TcpIpConnect();                         // vtable+0x14
	void GotHost();
	void Listen(NetworkAddress* p_address);

private:
	class Connect m_base;                // 0x00
	undefined m_platformComposite[0x14]; // 0x15c
};

// SYNTHETIC: LEMBALL 0x00471b10
// TcpIpConnect::`scalar deleting destructor'


#endif

