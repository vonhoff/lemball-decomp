#ifndef LEMBALL_PLATFORM_WINSOCK_TCPIPHOSTENTRY_H
#define LEMBALL_PLATFORM_WINSOCK_TCPIPHOSTENTRY_H

struct TcpIpHostEntry {
	char* m_name;
	char** m_aliases;
	short m_addressType;
	short m_addressLength;
	char** m_addressList;
};

#endif
