#ifndef LEMBALL_VISOS_NETWORK_FILENETWORKADDRESS_H
#define LEMBALL_VISOS_NETWORK_FILENETWORKADDRESS_H

#include "../../Common.h"

// SIZE 0x20
// VTABLE: LEMBALL 0x0049a030
class FileNetworkAddress {
public:
	virtual bool operator==(NetworkAddress& p_address); // vtable+0x10
	virtual bool operator>(NetworkAddress& p_address);  // vtable+0x14
	virtual char* GetStr();                             // vtable+0x00
	virtual char* operator=(NetworkAddress& p_address); // vtable+0x0c
	virtual char* operator=(const char* p_text);        // vtable+0x08
	virtual void operator=(eBroadcastTypes p_type);     // vtable+0x04

private:
	char m_text[28]; // 0x04
};

#endif
