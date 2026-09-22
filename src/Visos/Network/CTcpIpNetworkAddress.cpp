#include "CTcpIpNetworkAddress.h"

#include "CNetworkAddress.h"

#include <string.h>

struct in_addr {
	unsigned long s_addr;
};

extern "C" unsigned int __stdcall inet_addr(const char* p_text);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);

// FUNCTION: LEMBALL 0x0046f9b0
void CTcpIpNetworkAddress::operator=(eBroadcastTypes p_type)
{
	switch (p_type) {
	case BROADCAST_LAN:
		*this = *g_pBroadcastAddress;
		((unsigned char*) &m_ipv4Address)[3] = 0xff;
		break;
	case BROADCAST_WAN:
		m_ipv4Address = 0xffffffff;
		break;
	}
}

// FUNCTION: LEMBALL 0x0046f9f0
bool CTcpIpNetworkAddress::operator>(CNetworkAddress& p_address)
{
	return ((CTcpIpNetworkAddress*) &p_address)->m_ipv4Address < m_ipv4Address;
}

// FUNCTION: LEMBALL 0x00471500
void CTcpIpNetworkAddress::operator=(CNetworkAddress& p_address)
{
	in_addr in;
	in.s_addr = ((CTcpIpNetworkAddress*) &p_address)->m_ipv4Address;
	m_ipv4Address = in.s_addr;
	strcpy(m_text, inet_ntoa(in));
}

// FUNCTION: LEMBALL 0x00471550
void CTcpIpNetworkAddress::operator=(const char* p_text)
{
	m_ipv4Address = inet_addr(p_text);
	strcpy(m_text, p_text);
}

// FUNCTION: LEMBALL 0x00471590
bool CTcpIpNetworkAddress::operator==(CNetworkAddress& p_address)
{
	in_addr in = *(in_addr*) &((CTcpIpNetworkAddress*) &p_address)->m_ipv4Address;
	return !(m_ipv4Address - in.s_addr);
}

// FUNCTION: LEMBALL 0x004715b0
char* CTcpIpNetworkAddress::GetStr()
{
	return m_text;
}
