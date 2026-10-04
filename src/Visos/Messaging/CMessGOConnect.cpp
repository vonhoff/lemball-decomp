#include "CMessGOConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"
#include "Visos/Network/NetworkConstants.h"

// FUNCTION: LEMBALL 0x0045f580
CMessGOConnect::CMessGOConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += sizeof(CMessGOConnect);
	m_payloadCapacity += NETWORK_CONNECTION_MESSAGE_EXTRA_CAPACITY_BYTES;
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
	Add(m_assignedPort);
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e5c
CMessGOConnect* g_pMessGOConnect = NULL;
