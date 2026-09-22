#include "CMessOkConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f4f0
CMessOkConnect::CMessOkConnect(const char* p_arg0) : CBroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f540
void CMessOkConnect::GetData()
{
	m_assignedPort = GetWord();
	m_connectionId = (unsigned int) GetDword();
}

// FUNCTION: LEMBALL 0x0045f560
void CMessOkConnect::AddData()
{
	Add((unsigned short) m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e58
CMessOkConnect* g_pMessOKConnect = 0;
