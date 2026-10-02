#include "CMessFAILEDConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f610
CMessFAILEDConnect::CMessFAILEDConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f660
void CMessFAILEDConnect::GetData()
{
	Get(m_failureReason);
}

// FUNCTION: LEMBALL 0x0045f670
void CMessFAILEDConnect::AddData()
{
	Add(m_failureReason);
}

// GLOBAL: LEMBALL 0x004a1e60
CMessFAILEDConnect* g_pMessFAILEDConnect = 0;
