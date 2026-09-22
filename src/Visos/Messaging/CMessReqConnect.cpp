#include "CMessReqConnect.h"

#include "Visos/Messaging/CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x0045f3d0
CMessReqConnect::CMessReqConnect(const char* p_arg0) : CBroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x3c;
	m_payloadCapacity += 0x101;
	m_payloadCapacity += 0x200;
}

// FUNCTION: LEMBALL 0x0045f430
void CMessReqConnect::GetData()
{
	m_requestedPort = (unsigned short) GetDword();
	Get(*(const unsigned char**) &m_connectionData, 0x200);
	Get(*(const char**) &m_peerName);
}

// FUNCTION: LEMBALL 0x0045f460
void CMessReqConnect::AddData()
{
	Add((unsigned long) (int) (short) m_requestedPort);
	Add(m_connectionData, 0x200);
	Add(m_peerName);
}

// GLOBAL: LEMBALL 0x004a1e50
CMessReqConnect* g_pMessReqConnect = 0;
