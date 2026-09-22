#include "CTCPIPNetworkAddress.h"

#include "CNetworkAddress.h"
#include "Platform/WinSock/WinSock.h"
#include "Platform/WinSock/in_addr.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0046f9b0
void CTCPIPNetworkAddress::operator=(eBroadcastTypes p_type)
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
bool CTCPIPNetworkAddress::operator>(CNetworkAddress& p_address)
{
	return ((CTCPIPNetworkAddress*) &p_address)->m_ipv4Address < m_ipv4Address;
}

// FUNCTION: LEMBALL 0x00471500
void CTCPIPNetworkAddress::operator=(CNetworkAddress& p_address)
{
	in_addr in;
	in.s_addr = ((CTCPIPNetworkAddress*) &p_address)->m_ipv4Address;
	m_ipv4Address = in.s_addr;
	strcpy(m_text, inet_ntoa(in));
}

// FUNCTION: LEMBALL 0x00471550
void CTCPIPNetworkAddress::operator=(const char* p_text)
{
	m_ipv4Address = inet_addr(p_text);
	strcpy(m_text, p_text);
}

// FUNCTION: LEMBALL 0x00471590
bool CTCPIPNetworkAddress::operator==(CNetworkAddress& p_address)
{
	in_addr in = *(in_addr*) &((CTCPIPNetworkAddress*) &p_address)->m_ipv4Address;
	return !(m_ipv4Address - in.s_addr);
}

// FUNCTION: LEMBALL 0x004715b0
char* CTCPIPNetworkAddress::GetStr()
{
	return m_text;
}
