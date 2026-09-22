#include "CMessGoConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f580
CMessGoConnect::CMessGoConnect(const char* p_arg0) : CBroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f5d0
void CMessGoConnect::GetData()
{
	m_assignedPort = GetWord();
	m_connectionId = (unsigned int) GetDword();
}

// FUNCTION: LEMBALL 0x0045f5f0
void CMessGoConnect::AddData()
{
	Add((unsigned short) m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e5c
CMessGoConnect* g_pMessGOConnect = 0;
