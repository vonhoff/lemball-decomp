#include "CMessReqNewPort.h"

#include "BasePacketHeader.h"
#include "Visos/Messaging/CMessReqConnect.h"

// FUNCTION: LEMBALL 0x0045f490
CMessReqNewPort::CMessReqNewPort(const char* p_arg0) : CMessReqConnect(p_arg0)
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
CMessReqNewPort* g_pMessReqNewPort = 0;
