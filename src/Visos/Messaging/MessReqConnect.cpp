#include "MessReqConnect.h"

#include <string.h>

#pragma intrinsic(strlen)

// 68K 0x1020ace8 __ct__15CMessReqConnectFPCc
// FUNCTION: LEMBALL 0x0045f3d0
MessReqConnect::MessReqConnect(const char* p_arg0)
	: BroadcastMessage(p_arg0)
{
	m_payloadCapacity += 0x3c;
	m_payloadCapacity += 0x101;
	m_payloadCapacity += 0x200;
}

// 68K 0x1020ad6e GetData__15CMessReqConnectFv
// FUNCTION: LEMBALL 0x0045f430
void MessReqConnect::GetData()
{
	m_requestedPort = (unsigned short) GetDword();
	Get(*(const unsigned char**) &m_connectionData, 0x200);
	Get(*(const char**) &m_peerName);
}

// 68K 0x1020adc4 AddData__15CMessReqConnectFv
// FUNCTION: LEMBALL 0x0045f460
void MessReqConnect::AddData()
{
	Add((unsigned long) (int) (short) m_requestedPort);
	Add(m_connectionData, 0x200);
	Add(m_peerName);
}

// 68K 0x101060f4 __dt__15CMessReqConnectFv
MessReqConnect::~MessReqConnect()
{
}

// GLOBAL: LEMBALL 0x004a1e50
MessReqConnect* g_pMessReqConnect = 0;

