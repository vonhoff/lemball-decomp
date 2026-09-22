#include "CMessGOConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f580
CMessGOConnect::CMessGOConnect(const char* p_arg0) : CBroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f5d0
void CMessGOConnect::GetData()
{
	m_assignedPort = GetWORD();
	m_connectionId = (unsigned int) GetDWORD();
}

// FUNCTION: LEMBALL 0x0045f5f0
void CMessGOConnect::AddData()
{
	Add((unsigned short) m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e5c
CMessGOConnect* g_pMessGOConnect = 0;
