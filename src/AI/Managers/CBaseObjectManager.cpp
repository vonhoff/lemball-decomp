#include "CBaseObjectManager.h"

#include "../../Network/Game/CNetworkManager.h"
#include "../../Visos/Messaging/CWriteCBuff.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CGameObject.h"
#include "../Base/CGlobalGameObject.h"
#include "Visos/Messaging/CNetworkMessage.h"
#include "Visos/Network/CWriteSocket.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x0040ab70
void CBaseObjectManager::Restart()
{
}

void CBaseObjectManager::Process()
{
}

// FUNCTION: LEMBALL 0x0040ab80
bool CBaseObjectManager::Receive(unsigned short p_messageId, CGlobalGameObject* p_object, CNetworkMessage* p_message)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab90
int CBaseObjectManager::GetViewData(CViewData* p_viewData)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ad30
void CBaseObjectManager::Add(CNetworkMessage* p_message)
{
	while (1) {
		if (m_pendingSendCount != 0) {
			unsigned long start = timeGetTime();
			while (m_pendingSendCount != 0) {
				if (timeGetTime() - start >= 2000) {
					break;
				}
				g_pBaseNetwork->WaitProcess();
			}
			if (m_pendingSendCount != 0) {
				return;
			}
		}
		if ((int) m_openDepth <= 0) {
			OpenDataStream();
		}
		if ((m_writeCursor - m_buffer) + p_message->m_payloadCapacity + 2 > m_payloadCapacity) {
			ProcessNetwork();
		}
		else {
			p_message->CopyDataStream(m_writeCursor, 0);
			m_writeCursor += p_message->m_writeCursor - p_message->m_buffer;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0040adc0
void CBaseObjectManager::AddData()
{
}

// FUNCTION: LEMBALL 0x0040add0
void CBaseObjectManager::ProcessNetwork()
{
	if ((int) m_openDepth <= 0 || m_pendingSendCount != 0 || g_pActiveConnection == 0) {
		return;
	}
	CNetworkMessage::Add((unsigned short) 0x2f);
	CConnect* connection = g_pActiveConnection;
	if (connection->m_segmentIndex != -1 ||
		!connection->CWriteSocket::m_criticalBuffer->IsPacketAvailable(connection->CWriteSocket::m_criticalSequence)) {
		unsigned long start = timeGetTime();
		while (1) {
			connection = g_pActiveConnection;
			if (connection->m_segmentIndex == -1 && connection->CWriteSocket::m_criticalBuffer->IsPacketAvailable(
														connection->CWriteSocket::m_criticalSequence)) {
				break;
			}
			if (timeGetTime() - start >= 4000) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	connection = g_pActiveConnection;
	if (connection->m_segmentIndex == -1 &&
		connection->CWriteSocket::m_criticalBuffer->IsPacketAvailable(connection->CWriteSocket::m_criticalSequence)) {
		Send(g_pActiveConnection);
		CloseDataStream();
		return;
	}
	g_pNetworkManager->Kill();
	CloseDataStream();
}

// FUNCTION: LEMBALL 0x0040aed0
void CBaseObjectManager::GetData()
{
	unsigned short type = GetWORD();
	while (type != 0x2f) {
		unsigned short id = GetWORD();
		CGlobalGameObject* found = 0;
		for (unsigned int i = 0; (int) i < (int) (unsigned int) g_wObjectCount; i++) {
			CGameObject* obj = g_pObjects[(unsigned short) i];
			if (obj != 0 && (unsigned short) obj->GetId() == id) {
				found = (CGlobalGameObject*) obj;
				break;
			}
		}
		if (found->Receive(type, this) == 0) {
			Receive(type, found, this);
		}
		type = GetWORD();
	}
}
