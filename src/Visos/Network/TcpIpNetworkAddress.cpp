#include "TcpIpNetworkAddress.h"

#include "NetworkAddress.h"

#include <string.h>

struct in_addr {
	unsigned long s_addr;
};

extern "C" unsigned int __stdcall inet_addr(const char* p_text);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);

// 68K 0x1010c4f2 __as__20CTCPIPNetworkAddressF15eBroadcastTypes
// FUNCTION: LEMBALL 0x0046f9b0
void TcpIpNetworkAddress::operator=(eBroadcastTypes p_type)
{
	switch (p_type) {
	case 0:
		*this = *g_pBroadcastAddress;
		((unsigned char*) &m_ipv4Address)[3] = 0xff;
		break;
	case 1:
		m_ipv4Address = 0xffffffff;
		break;
	}
}

// 68K 0x1010c5ca __gt__20CTCPIPNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x0046f9f0
bool TcpIpNetworkAddress::operator>(NetworkAddress& p_address)
{
	return ((TcpIpNetworkAddress*) &p_address)->m_ipv4Address < m_ipv4Address;
}

// 68K 0x1010c566 __as__20CTCPIPNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x00471500
void TcpIpNetworkAddress::operator=(NetworkAddress& p_address)
{
	in_addr in;
	in.s_addr = ((TcpIpNetworkAddress*) &p_address)->m_ipv4Address;
	m_ipv4Address = in.s_addr;
	strcpy(m_text, inet_ntoa(in));
}

// 68K 0x1010c48c __as__20CTCPIPNetworkAddressFPCc
// FUNCTION: LEMBALL 0x00471550
void TcpIpNetworkAddress::operator=(const char* p_text)
{
	m_ipv4Address = inet_addr(p_text);
	strcpy(m_text, p_text);
}

// 68K 0x1010e4d0 __eq__20CTCPIPNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x00471590
bool TcpIpNetworkAddress::operator==(NetworkAddress& p_address)
{
	in_addr in;
	in.s_addr = ((TcpIpNetworkAddress*) &p_address)->m_ipv4Address;
	return m_ipv4Address == in.s_addr;
}

// 68K 0x1010d9c8 GetStr__20CTCPIPNetworkAddressFv
// FUNCTION: LEMBALL 0x004715b0
char* TcpIpNetworkAddress::GetStr()
{
	return m_text;
}
