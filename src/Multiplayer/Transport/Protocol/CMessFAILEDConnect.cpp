#include "CMessFAILEDConnect.h"

#include "CBroadcastMessage.h"
#include "Multiplayer/Transport/NetworkConstants.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045f610
CMessFAILEDConnect::CMessFAILEDConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += NETWORK_CONNECTION_MESSAGE_EXTRA_CAPACITY_BYTES;
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
CMessFAILEDConnect* g_pMessFAILEDConnect = NULL;
