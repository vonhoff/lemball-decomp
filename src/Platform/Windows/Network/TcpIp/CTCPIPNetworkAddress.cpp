#include "Platform/Windows/Network/TcpIp/CTCPIPNetworkAddress.h"

#include <string.h>

#include "Platform/Windows/WindowsTypes.h"
#include <winsock.h>

#include "Multiplayer/Transport/CNetworkAddress.h"

// FUNCTION: LEMBALL 0x0046f9b0
void CTCPIPNetworkAddress::operator=(eBroadcastTypes p_type)
{
	switch (p_type) {
	case BROADCAST_LAN:
		*this = *g_pBroadcastAddress;
		m_ipv4Address.S_un.S_un_b.s_b4 = 0xff;
		break;
	case BROADCAST_WAN:
		m_ipv4Address.s_addr = INADDR_NONE;
		break;
	}
}

// FUNCTION: LEMBALL 0x0046f9f0
bool CTCPIPNetworkAddress::operator>(CNetworkAddress& p_address)
{
	return ((CTCPIPNetworkAddress*) &p_address)->m_ipv4Address.s_addr < m_ipv4Address.s_addr;
}

// FUNCTION: LEMBALL 0x00471500
void CTCPIPNetworkAddress::operator=(CNetworkAddress& p_address)
{
	in_addr in;
	in = ((CTCPIPNetworkAddress*) &p_address)->m_ipv4Address;
	m_ipv4Address = in;
	strcpy(m_text, inet_ntoa(in));
}

// FUNCTION: LEMBALL 0x00471550
void CTCPIPNetworkAddress::operator=(const char* p_text)
{
	m_ipv4Address.s_addr = inet_addr(p_text);
	strcpy(m_text, p_text);
}

// FUNCTION: LEMBALL 0x00471590
bool CTCPIPNetworkAddress::operator==(CNetworkAddress& p_address)
{
	return ((CTCPIPNetworkAddress*) &p_address)->Get().s_addr == m_ipv4Address.s_addr;
}

// FUNCTION: LEMBALL 0x004715b0
char* CTCPIPNetworkAddress::GetStr()
{
	return m_text;
}
