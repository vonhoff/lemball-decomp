#ifndef LEMBALL_VISOS_NETWORK_CFILENETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_CFILENETWORKADDRESS_H

#include "CNetworkAddress.h"

// SIZE 0x20
// VTABLE: LEMBALL 0x0049a030
class CFileNetworkAddress : public CNetworkAddress {
public:
	virtual char* GetStr();                              // vtable+0x00
	virtual void operator=(CNetworkAddress& p_address);  // vtable+0x0c
	virtual void operator=(const char* p_text);          // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type);      // vtable+0x04
	virtual bool operator==(CNetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(CNetworkAddress& p_address);  // vtable+0x14

private:
	char m_text[28]; // 0x04

	friend class CFileNetwork;
};

#endif
