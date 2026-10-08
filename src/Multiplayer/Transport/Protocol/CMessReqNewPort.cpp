#include "CMessReqNewPort.h"

#include "CMessReqConnect.h"
#include "Multiplayer/Transport/Packets/BasePacketHeader.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045f490
CMessReqNewPort::CMessReqNewPort(const char* p_header) : CMessReqConnect(p_header)
{
	m_payloadCapacity += sizeof(BasePacketHeader);
}

// FUNCTION: LEMBALL 0x0045f4b0
void CMessReqNewPort::GetData()
{
	CMessReqConnect::GetData();
	m_connectionId = (unsigned int) GetDWORD();
}

// FUNCTION: LEMBALL 0x0045f4d0
void CMessReqNewPort::AddData()
{
	CMessReqConnect::AddData();
	Add((unsigned long) m_connectionId);
}

// GLOBAL: LEMBALL 0x004a1e54
CMessReqNewPort* g_pMessReqNewPort = NULL;
