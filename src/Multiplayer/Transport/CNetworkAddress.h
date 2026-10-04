#ifndef LEMBALL_VISOS_NETWORK_CNETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_CNETWORKADDRESS_H

enum eBroadcastTypes {
	BROADCAST_LAN = 0,
	BROADCAST_WAN = 1,
};

// SIZE 0x04
class CNetworkAddress {
public:
	virtual char* GetStr() = 0;                              // vtable+0x00
	virtual void operator=(CNetworkAddress& p_address) = 0;  // vtable+0x0c
	virtual void operator=(const char* p_text) = 0;          // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type) = 0;      // vtable+0x04
	virtual bool operator==(CNetworkAddress& p_address) = 0; // vtable+0x10
	virtual bool operator>(CNetworkAddress& p_address) = 0;  // vtable+0x14
};

extern CNetworkAddress* g_pBroadcastReceiveAddress;
extern CNetworkAddress* g_pBroadcastAddress;
#endif
