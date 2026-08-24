#include "TcpIpNetworkAddress.h"
#include "NetworkAddress.h"

#include <string.h>

extern "C" __declspec(dllimport) unsigned int __stdcall inet_addr(const char* p_text);
extern "C" __declspec(dllimport) char* __stdcall inet_ntoa(unsigned int p_address);

// 68K 0x1010c4f2 __as__20CTCPIPNetworkAddressF15eBroadcastTypes
// FUNCTION: LEMBALL 0x0046f9b0
void TcpIpNetworkAddress::operator=(eBroadcastTypes p_type)
{
	unsigned char* ipBytes;

	if (p_type == 0) {
		*this = *g_pBroadcastAddress;
		ipBytes = (unsigned char*) &m_ipv4Address;
		ipBytes[3] = 0xff;
		return;
	}
	if (p_type != 1) {
		return;
	}
	m_ipv4Address = 0xffffffff;
}

// 68K 0x1010c5ca __gt__20CTCPIPNetworkAddressFR15CNetworkAddress
// STUB: LEMBALL 0x0046f9f0
bool TcpIpNetworkAddress::operator>(NetworkAddress& p_address)
{
	return 0;
}

// 68K 0x1010c566 __as__20CTCPIPNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x00471500
void TcpIpNetworkAddress::operator=(NetworkAddress& p_address)
{
	m_ipv4Address = ((TcpIpNetworkAddress*) &p_address)->m_ipv4Address;
	strcpy(m_text, inet_ntoa(m_ipv4Address));
}

// 68K 0x1010c48c __as__20CTCPIPNetworkAddressFPCc
// FUNCTION: LEMBALL 0x00471550
char* TcpIpNetworkAddress::operator=(const char* p_text)
{
	m_ipv4Address = inet_addr(p_text);
	strcpy(m_text, p_text);
	return m_text;
}

// 68K 0x1010e4d0 __eq__20CTCPIPNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x00471590
bool TcpIpNetworkAddress::operator==(NetworkAddress& p_address)
{
	return m_ipv4Address == ((TcpIpNetworkAddress*) &p_address)->m_ipv4Address;
}

// 68K 0x1010d9c8 GetStr__20CTCPIPNetworkAddressFv
// FUNCTION: LEMBALL 0x004715b0
char* TcpIpNetworkAddress::GetStr()
{
	return m_text;
}
