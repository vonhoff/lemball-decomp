#ifndef LEMBALL_VISOS_NETWORK_NETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_NETWORKADDRESS_H

#include "../../Common.h"

// SIZE 0x04
class NetworkAddress {
public:
	virtual char* GetStr() = 0;                             // vtable+0x00
	virtual void operator=(NetworkAddress& p_address) = 0; // vtable+0x0c
	virtual void operator=(const char* p_text) = 0;        // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type) = 0;     // vtable+0x04
	virtual bool operator==(NetworkAddress& p_address) = 0; // vtable+0x10
	virtual bool operator>(NetworkAddress& p_address) = 0;  // vtable+0x14
};

extern NetworkAddress* g_pBroadcastReceiveAddress;
extern NetworkAddress* g_pBroadcastAddress;
#endif
