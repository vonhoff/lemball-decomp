#ifndef LEMBALL_PLATFORM_WINSOCK_WINSOCK_H
#define LEMBALL_PLATFORM_WINSOCK_WINSOCK_H


struct in_addr;
struct TcpIpSocketAddress;
struct TcpIpDestinationAddress;

extern "C" unsigned long __stdcall inet_addr(const char* p_text);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);
extern "C" int __stdcall gethostname(char* p_name, int p_nameSize);
extern "C" int __stdcall socket(int p_addressFamily, int p_type, int p_protocol);
extern "C" int __stdcall bind(int p_socket, const TcpIpSocketAddress* p_address, int p_addressLength);
extern "C" unsigned short __stdcall htons(unsigned short p_value);
extern "C" unsigned short __stdcall ntohs(unsigned short p_value);
extern "C" int __stdcall setsockopt(int p_socket, int p_level, int p_option, const char* p_value, int p_valueSize);
extern "C" int __stdcall WSACancelAsyncRequest(unsigned int p_request);
extern "C" unsigned int __stdcall WSAAsyncGetHostByName(void* p_window,
														unsigned int p_message,
														const char* p_name,
														char* p_buffer,
														int p_bufferSize);
extern "C" unsigned int __stdcall WSAAsyncGetServByName(void* p_window,
														unsigned int p_message,
														const char* p_service,
														const char* p_protocol,
														char* p_buffer,
														int p_bufferSize);
extern "C" int __stdcall WSAAsyncSelect(int p_socket, void* p_window, unsigned int p_message, long p_events);
extern "C" int __stdcall closesocket(int p_socket);
extern "C" int __stdcall WSAGetLastError();
extern "C" int __stdcall ioctlsocket(int p_socket, long p_command, unsigned long* p_value);
extern "C" int __stdcall WSAStartup(unsigned short p_version, void* p_data);
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
								const TcpIpDestinationAddress* p_address,
								int p_addressLength);

#endif
