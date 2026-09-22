#ifndef LEMBALL_PLATFORM_WINSOCK_TCPIPSERVICEENTRY_H
#define LEMBALL_PLATFORM_WINSOCK_TCPIPSERVICEENTRY_H

struct TcpIpServiceEntry {
	char* m_name;
	char** m_aliases;
	short m_port;
	char* m_protocol;
};

#endif
