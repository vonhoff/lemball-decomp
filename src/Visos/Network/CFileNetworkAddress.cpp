#include "CFileNetworkAddress.h"

#include "../Foundation/CVsOStream.h"
#include "Visos/Network/CNetworkAddress.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0046f880
void CFileNetworkAddress::operator=(CNetworkAddress& p_address)
{
	strcpy(m_text, ((CFileNetworkAddress*) &p_address)->m_text);
}

// FUNCTION: LEMBALL 0x0046f8b0
void CFileNetworkAddress::operator=(const char* p_text)
{
	strcpy(m_text, p_text);
}

// FUNCTION: LEMBALL 0x0046f8e0
bool CFileNetworkAddress::operator==(CNetworkAddress& p_address)
{
	return strcmp(((CFileNetworkAddress*) &p_address)->m_text, m_text) == 0;
}

// FUNCTION: LEMBALL 0x0046f920
char* CFileNetworkAddress::GetStr()
{
	return m_text;
}

// FUNCTION: LEMBALL 0x004794e0
void CFileNetworkAddress::operator=(eBroadcastTypes p_type)
{
	switch (p_type) {
	case BROADCAST_LAN:
		*this = "LAN";
		break;
	case BROADCAST_WAN:
		*this = "WAN";
		break;
	}
}

// FUNCTION: LEMBALL 0x00479510
bool CFileNetworkAddress::operator>(CNetworkAddress& p_address)
{
	*g_pErrorOutput << "You haven't implemented major/minor destinction in 'CFileNetworkAddress'es\n";
	*g_pErrorOutput << "use count entry in broadcast file\n";
	return 0;
}
