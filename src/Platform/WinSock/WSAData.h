#ifndef LEMBALL_PLATFORM_WINSOCK_WSADATA_H
#define LEMBALL_PLATFORM_WINSOCK_WSADATA_H

#define WSADESCRIPTION_LEN 256
#define WSASYS_STATUS_LEN 128

struct WSAData {
	unsigned short wVersion;
	unsigned short wHighVersion;
	char szDescription[WSADESCRIPTION_LEN + 1];
	char szSystemStatus[WSASYS_STATUS_LEN + 1];
	unsigned short iMaxSockets;
	unsigned short iMaxUdpDg;
	char* lpVendorInfo;
};

typedef WSAData WSADATA;

#endif
