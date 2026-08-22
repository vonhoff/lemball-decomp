#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_TCPIPNETWORKADDRESS_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_TCPIPNETWORKADDRESS_H

#include "../../Common.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x0049a290
class TcpIpNetworkAddress {
public:
	virtual bool operator==(NetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(NetworkAddress& p_address);  // vtable+0x14
	virtual char* GetStr();                             // vtable+0x00
	virtual char* operator=(const char* p_text);        // vtable+0x08
	virtual void operator=(NetworkAddress& p_address);  // vtable+0x0c
	virtual void operator=(eBroadcastTypes p_type);     // vtable+0x04

private:
	char m_text[16];            // 0x04
	unsigned int m_ipv4Address; // 0x14
};

#endif
