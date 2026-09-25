#ifndef LEMBALL_VISOS_NETWORK_CTCPIPNETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_CTCPIPNETWORKADDRESS_H

#include "CNetworkAddress.h"

// SIZE 0x18
// VTABLE: LEMBALL 0x0049a290
class CTCPIPNetworkAddress : public CNetworkAddress {
public:
	virtual char* GetStr();                              // vtable+0x00
	virtual void operator=(CNetworkAddress& p_address);  // vtable+0x0c
	virtual void operator=(const char* p_text);          // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type);      // vtable+0x04
	virtual bool operator==(CNetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(CNetworkAddress& p_address);  // vtable+0x14

	friend class CTCPIPNetwork;
	friend class CTCPIPReadSocket;
	friend class CTCPIPWriteSocket;
	friend class CTCPIPBroadcast;
	friend class CTCPIPConnect;

private:
	char m_text[16];            // 0x04
	unsigned int m_ipv4Address; // 0x14
};

#endif
