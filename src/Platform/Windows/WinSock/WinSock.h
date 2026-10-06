#ifndef LEMBALL_PLATFORM_WINSOCK_WINSOCK_H
#define LEMBALL_PLATFORM_WINSOCK_WINSOCK_H

struct in_addr {
	unsigned long s_addr;
};

struct TcpIpSocketAddress {
	unsigned short m_family;
	unsigned short m_port;
	in_addr m_address;
	unsigned char m_padding[8];
};

struct TcpIpHostEntry {
	char* m_name;
	char** m_aliases;
	short m_addressType;
	short m_addressLength;
	char** m_addressList;
};

struct TcpIpServiceEntry {
	char* m_name;
	char** m_aliases;
	short m_port;
	char* m_protocol;
};

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

#define SOL_SOCKET 0xffff
#define SO_BROADCAST 0x0020
#define FD_READ 0x01
#define FD_WRITE 0x02

#define AF_INET 2
#define SOCK_DGRAM 2

#define INADDR_NONE 0xffffffff
#define MAXGETHOSTSTRUCT 1024

#define WSABASEERR 10000
#define WSAEWOULDBLOCK (WSABASEERR + 35)
#define WSAHOST_NOT_FOUND (WSABASEERR + 1001)
#define WSATRY_AGAIN (WSABASEERR + 1002)
#define WSANO_RECOVERY (WSABASEERR + 1003)
#define WSANO_DATA (WSABASEERR + 1004)

struct in_addr;
struct TcpIpSocketAddress;
struct WSAData;

extern "C" unsigned long __stdcall inet_addr(const char* p_text);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);
extern "C" int __stdcall gethostname(char* p_name, int p_nameSize);
extern "C" int __stdcall socket(int p_addressFamily, int p_type, int p_protocol);
extern "C" int __stdcall bind(int p_socket, const TcpIpSocketAddress* p_address, int p_addressLength);
extern "C" unsigned short __stdcall htons(unsigned short p_value);
extern "C" unsigned short __stdcall ntohs(unsigned short p_value);
extern "C" int __stdcall setsockopt(int p_socket, int p_level, int p_option, const char* p_value, int p_valueSize);
extern "C" int __stdcall WSACancelAsyncRequest(void* p_request);
extern "C" void* __stdcall WSAAsyncGetHostByName(void* p_window,
												 unsigned int p_message,
												 const char* p_name,
												 char* p_buffer,
												 int p_bufferSize);
extern "C" void* __stdcall WSAAsyncGetServByName(void* p_window,
												 unsigned int p_message,
												 const char* p_service,
												 const char* p_protocol,
												 char* p_buffer,
												 int p_bufferSize);
extern "C" int __stdcall WSAAsyncSelect(int p_socket, void* p_window, unsigned int p_message, long p_events);
extern "C" int __stdcall closesocket(int p_socket);
extern "C" int __stdcall WSAGetLastError();
extern "C" int __stdcall ioctlsocket(int p_socket, long p_command, unsigned long* p_value);
extern "C" int __stdcall WSAStartup(unsigned short p_version, WSAData* p_data);
extern "C" int __stdcall WSACleanup();
extern "C" int __stdcall recv(int p_socket, char* p_buffer, int p_length, int p_flags);
extern "C" int __stdcall recvfrom(int p_socket,
								  char* p_buffer,
								  int p_length,
								  int p_flags,
								  TcpIpSocketAddress* p_address,
								  int* p_addressLength);
extern "C" int __stdcall sendto(int p_socket,
								const char* p_buffer,
								int p_length,
								int p_flags,
								const TcpIpSocketAddress* p_address,
								int p_addressLength);

#endif
