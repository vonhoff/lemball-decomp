#include "CMessReqConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

// FUNCTION: LEMBALL 0x0045f3d0
CMessReqConnect::CMessReqConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += 0x3c;
	m_payloadCapacity += 0x101;
	m_payloadCapacity += 0x200;
}

// FUNCTION: LEMBALL 0x0045f430
void CMessReqConnect::GetData()
{
	m_requestedPort = (unsigned short) GetDWORD();
	Get(m_connectionData, 0x200);
	Get(m_peerName);
}

// FUNCTION: LEMBALL 0x0045f460
void CMessReqConnect::AddData()
{
	Add((unsigned long) (int) (short) m_requestedPort);
	Add(m_connectionData, 0x200);
	Add(m_peerName);
}

// GLOBAL: LEMBALL 0x004a1e50
CMessReqConnect* g_pMessReqConnect = NULL;
