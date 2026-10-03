#include "CMessOKConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

// FUNCTION: LEMBALL 0x0045f4f0
CMessOKConnect::CMessOKConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += 0x38;
	m_payloadCapacity += 0x101;
}

// FUNCTION: LEMBALL 0x0045f540
void CMessOKConnect::GetData()
{
	m_assignedPort = GetWORD();
	m_connectionId = (unsigned int) GetDWORD();
}

// FUNCTION: LEMBALL 0x0045f560
void CMessOKConnect::AddData()
{
	Add(m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e58
CMessOKConnect* g_pMessOKConnect = NULL;
