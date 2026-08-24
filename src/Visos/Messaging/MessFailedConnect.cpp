#include "MessFailedConnect.h"

#include <string.h>

#pragma intrinsic(strlen)

// 68K 0x1020b0fc __ct__18CMessFAILEDConnectFPCc
// FUNCTION: LEMBALL 0x0045f610
MessFailedConnect::MessFailedConnect(const char* p_arg0)
{
	unsigned int nameLength;

	m_header = p_arg0;
	nameLength = (unsigned int) strlen(p_arg0) + 1;
	m_payloadCapacity += nameLength;
	m_payloadCapacity += 0x101;
}

// 68K 0x1020b178 GetData__18CMessFAILEDConnectFv
// FUNCTION: LEMBALL 0x0045f660
void MessFailedConnect::GetData()
{
	Get(*(const char**) &m_failureReason);
}

// 68K 0x1020b1b2 AddData__18CMessFAILEDConnectFv
// FUNCTION: LEMBALL 0x0045f670
void MessFailedConnect::AddData()
{
	Add(m_failureReason);
}

// 68K 0x10117044 __dt__18CMessFAILEDConnectFv
// SYNTHETIC: LEMBALL 0x004628b0
// MessFailedConnect::`scalar deleting destructor'
MessFailedConnect::~MessFailedConnect()
{
}

// GLOBAL: LEMBALL 0x004a1e60
MessFailedConnect* g_pMessFAILEDConnect = 0;
