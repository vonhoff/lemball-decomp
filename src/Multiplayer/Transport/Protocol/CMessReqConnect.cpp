#include "CMessReqConnect.h"

#include "CBroadcastMessage.h"
#include "Multiplayer/Transport/NetworkConstants.h"

// FUNCTION: LEMBALL 0x0045f3d0
CMessReqConnect::CMessReqConnect(const char* p_header) : CBroadcastMessage(p_header)
{
	m_payloadCapacity += sizeof(CMessReqConnect);
	m_payloadCapacity += NETWORK_CONNECTION_MESSAGE_EXTRA_CAPACITY_BYTES;
	m_payloadCapacity += NETWORK_PORT_COUNT;
}

// FUNCTION: LEMBALL 0x0045f430
void CMessReqConnect::GetData()
{
	m_requestedPort = (unsigned short) GetDWORD();
	Get(m_connectionData, NETWORK_PORT_COUNT);
	Get(m_peerName);
}

// FUNCTION: LEMBALL 0x0045f460
void CMessReqConnect::AddData()
{
	Add((unsigned long) (int) (short) m_requestedPort);
	Add(m_connectionData, NETWORK_PORT_COUNT);
	Add(m_peerName);
}

// GLOBAL: LEMBALL 0x004a1e50
CMessReqConnect* g_pMessReqConnect = NULL;
