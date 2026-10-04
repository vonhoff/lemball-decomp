#include "CMessOKConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"
#include "Visos/Network/NetworkConstants.h"

// FUNCTION: LEMBALL 0x0045f4f0
CMessOKConnect::CMessOKConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += sizeof(CMessOKConnect);
	m_payloadCapacity += NETWORK_CONNECTION_MESSAGE_EXTRA_CAPACITY_BYTES;
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
