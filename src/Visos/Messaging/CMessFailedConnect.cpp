#include "CMessFailedConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f610
CMessFailedConnect::CMessFailedConnect(const char* p_arg0) : CBroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f660
void CMessFailedConnect::GetData()
{
	Get(*(const char**) &m_failureReason);
}

// FUNCTION: LEMBALL 0x0045f670
void CMessFailedConnect::AddData()
{
	Add(m_failureReason);
}

// GLOBAL: LEMBALL 0x004a1e60
CMessFailedConnect* g_pMessFAILEDConnect = 0;
