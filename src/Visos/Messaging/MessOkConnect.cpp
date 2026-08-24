#include "MessOkConnect.h"

#include <string.h>

#pragma intrinsic(strlen)

// 68K 0x1020aeec __ct__14CMessOKConnectFPCc
// FUNCTION: LEMBALL 0x0045f4f0
MessOkConnect::MessOkConnect(const char* p_arg0)
{
	unsigned int nameLength;

	m_header = p_arg0;
	nameLength = (unsigned int) strlen(p_arg0) + 1;
	m_payloadCapacity += nameLength;
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// 68K 0x1020af6a GetData__14CMessOKConnectFv
// FUNCTION: LEMBALL 0x0045f540
void MessOkConnect::GetData()
{
	m_assignedPort = GetWord();
	m_connectionId = (unsigned int) GetDword();
}

// 68K 0x1020afae AddData__14CMessOKConnectFv
// FUNCTION: LEMBALL 0x0045f560
void MessOkConnect::AddData()
{
	Add((unsigned short) m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// 68K 0x10117108 __dt__14CMessOKConnectFv
// SYNTHETIC: LEMBALL 0x00462870
// MessOkConnect::`scalar deleting destructor'
MessOkConnect::~MessOkConnect()
{
}

// GLOBAL: LEMBALL 0x004a1e58
MessOkConnect* g_pMessOKConnect = 0;
