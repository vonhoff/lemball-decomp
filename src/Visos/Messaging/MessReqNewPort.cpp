#include "MessReqNewPort.h"
#include "BasePacketHeader.h"

// 68K 0x1020ae1c __ct__15CMessReqNewPortFPCc
// FUNCTION: LEMBALL 0x0045f490
MessReqNewPort::MessReqNewPort(const char* p_arg0) : MessReqConnect(p_arg0)
{
	m_payloadCapacity += sizeof(BasePacketHeader);
}

// 68K 0x1020ae66 GetData__15CMessReqNewPortFv
// FUNCTION: LEMBALL 0x0045f4b0
void MessReqNewPort::GetData()
{
	MessReqConnect::GetData();
	m_connectionId = (unsigned int) GetDword();
}

// 68K 0x1020aea8 AddData__15CMessReqNewPortFv
// FUNCTION: LEMBALL 0x0045f4d0
void MessReqNewPort::AddData()
{
	MessReqConnect::AddData();
	Add((unsigned long) m_connectionId);
}

// 68K 0x10117168 __dt__15CMessReqNewPortFv
MessReqNewPort::~MessReqNewPort()
{
}

// GLOBAL: LEMBALL 0x004a1e54
MessReqNewPort* g_pMessReqNewPort = 0;

