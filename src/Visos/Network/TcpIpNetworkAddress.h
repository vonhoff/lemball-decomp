#ifndef LEMBALL_VISOS_NETWORK_TCPIPNETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_TCPIPNETWORKADDRESS_H

#include "NetworkAddress.h" // complete type

// SIZE 0x18
// VTABLE: LEMBALL 0x0049a290
class TcpIpNetworkAddress : public NetworkAddress {
public:
	virtual char* GetStr();                             // vtable+0x00
	virtual void operator=(NetworkAddress& p_address);  // vtable+0x0c
	virtual void operator=(const char* p_text);         // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type);     // vtable+0x04
	virtual bool operator==(NetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(NetworkAddress& p_address);  // vtable+0x14

	friend class TcpIpNetwork;
	friend class TcpIpReadSocket;
	friend class TcpIpWriteSocket;
	friend class TcpIpBroadcast;
	friend class TcpIpConnect;

private:
	char m_text[16];            // 0x04
	unsigned int m_ipv4Address; // 0x14
};

#endif
