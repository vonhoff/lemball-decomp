#ifndef LEMBALL_VISOS_NETWORK_FILENETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_FILENETWORKADDRESS_H

#include "NetworkAddress.h" // complete type

// SIZE 0x20
// VTABLE: LEMBALL 0x0049a030
class FileNetworkAddress : public NetworkAddress {
public:
	virtual char* GetStr();                             // vtable+0x00
	virtual void operator=(NetworkAddress& p_address);  // vtable+0x0c
	virtual void operator=(const char* p_text);         // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type);     // vtable+0x04
	virtual bool operator==(NetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(NetworkAddress& p_address);  // vtable+0x14

private:
	char m_text[28]; // 0x04

	friend class FileNetwork;
};

#endif
