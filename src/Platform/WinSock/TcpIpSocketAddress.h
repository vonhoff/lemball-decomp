#ifndef LEMBALL_PLATFORM_WINSOCK_TCPIPSOCKETADDRESS_H
#define LEMBALL_PLATFORM_WINSOCK_TCPIPSOCKETADDRESS_H

#include "in_addr.h"

struct TcpIpSocketAddress {
	unsigned short m_family;
	unsigned short m_port;
	in_addr m_address;
	unsigned char m_padding[8];
};

#endif
