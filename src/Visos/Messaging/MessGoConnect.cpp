#include "MessGoConnect.h"

#include <string.h>

#pragma intrinsic(strlen)

// 68K 0x1020aff4 __ct__14CMessGOConnectFPCc
// FUNCTION: LEMBALL 0x0045f580
MessGoConnect::MessGoConnect(const char* p_arg0)
{
	unsigned int nameLength;

	m_header = p_arg0;
	nameLength = (unsigned int) strlen(p_arg0) + 1;
	m_payloadCapacity += nameLength;
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// 68K 0x1020b072 GetData__14CMessGOConnectFv
// FUNCTION: LEMBALL 0x0045f5d0
void MessGoConnect::GetData()
{
	m_assignedPort = GetWord();
	m_connectionId = (unsigned int) GetDword();
}

// 68K 0x1020b0b6 AddData__14CMessGOConnectFv
// FUNCTION: LEMBALL 0x0045f5f0
void MessGoConnect::AddData()
{
	Add((unsigned short) m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// 68K 0x101170a8 __dt__14CMessGOConnectFv
// SYNTHETIC: LEMBALL 0x00462890
// MessGoConnect::`scalar deleting destructor'
MessGoConnect::~MessGoConnect()
{
}

// GLOBAL: LEMBALL 0x004a1e5c
MessGoConnect* g_pMessGOConnect = 0;
