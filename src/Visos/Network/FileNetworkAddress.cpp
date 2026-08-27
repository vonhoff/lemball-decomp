#include "FileNetworkAddress.h"

#include "../Foundation/VsOStream.h"

#include <string.h>

// 68K 0x101166d2 __as__19CFileNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x0046f880
void FileNetworkAddress::operator=(NetworkAddress& p_address)
{
	strcpy(m_text, ((FileNetworkAddress*) &p_address)->m_text);
}

// 68K 0x10116636 __as__19CFileNetworkAddressFPCc
// FUNCTION: LEMBALL 0x0046f8b0
void FileNetworkAddress::operator=(const char* p_text)
{
	strcpy(m_text, p_text);
}

// 68K 0x1011672a __eq__19CFileNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x0046f8e0
bool FileNetworkAddress::operator==(NetworkAddress& p_address)
{
	return strcmp(((FileNetworkAddress*) &p_address)->m_text, m_text) == 0;
}

// 68K 0x10116782 GetStr__19CFileNetworkAddressFv
// FUNCTION: LEMBALL 0x0046f920
char* FileNetworkAddress::GetStr()
{
	return m_text;
}

// 68K 0x10206b96 __as__19CFileNetworkAddressF15eBroadcastTypes
// FUNCTION: LEMBALL 0x004794e0
void FileNetworkAddress::operator=(eBroadcastTypes p_type)
{
	switch (p_type) {
	case 0:
		*this = "LAN";
		break;
	case 1:
		*this = "WAN";
		break;
	}
}

// 68K 0x10206c0c __gt__19CFileNetworkAddressFR15CNetworkAddress
// FUNCTION: LEMBALL 0x00479510
bool FileNetworkAddress::operator>(NetworkAddress& p_address)
{
	*g_pErrorOutput << "You haven't implemented major/minor destinction in 'CFileNetworkAddress'es\n";
	*g_pErrorOutput << "use count entry in broadcast file\n";
	return 0;
}
