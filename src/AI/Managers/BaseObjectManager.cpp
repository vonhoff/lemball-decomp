#include "BaseObjectManager.h"

#include "../../Network/Game/NetworkManager.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Messaging/WriteCBuff.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Connect.h"
#include "../Base/GameObject.h"
#include "../Base/GlobalGameObject.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x1011977c Restart__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040ab70
void BaseObjectManager::Restart()
{
}

void BaseObjectManager::Process()
{
}

// 68K 0x10119728 Receive__18CBaseObjectManagerFUsP17CGlobalGameObjectP15CNetworkMessage
// FUNCTION: LEMBALL 0x0040ab80
bool BaseObjectManager::Receive(unsigned short p_messageId, GlobalGameObject* p_object, NetworkMessage* p_message)
{
	return 0;
}

// 68K 0x101197a6 GetViewData__18CBaseObjectManagerFP9CViewData
// FUNCTION: LEMBALL 0x0040ab90
int BaseObjectManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1011806c __dt__18CBaseObjectManagerFv
BaseObjectManager::~BaseObjectManager()
{
}

// 68K 0x1060420e Add__18CBaseObjectManagerFP15CNetworkMessage
// FUNCTION: LEMBALL 0x0040ad30
void BaseObjectManager::Add(NetworkMessage* p_message)
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

// 68K 0x106042fe AddData__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040adc0
void BaseObjectManager::AddData()
{
}

// 68K 0x10604328 ProcessNetwork__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040add0
void BaseObjectManager::ProcessNetwork()
{
	if ((int) m_openDepth <= 0 || m_pendingSendCount != 0 || g_pActiveConnection == 0) {
		return;
	}
	NetworkMessage::Add((unsigned short) 0x2f);
	Connect* socket = g_pActiveConnection;
	if (socket->WriteSocket::m_segmentIndex != -1 ||
		!socket->WriteSocket::m_criticalBuffer->IsPacketAvailable(socket->WriteSocket::m_criticalSequence)) {
		unsigned long start = timeGetTime();
		while (1) {
			socket = g_pActiveConnection;
			if (socket->WriteSocket::m_segmentIndex == -1 &&
				socket->WriteSocket::m_criticalBuffer->IsPacketAvailable(socket->WriteSocket::m_criticalSequence)) {
				break;
			}
			if (timeGetTime() - start >= 4000) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	socket = g_pActiveConnection;
	if (socket->WriteSocket::m_segmentIndex == -1 &&
		socket->WriteSocket::m_criticalBuffer->IsPacketAvailable(socket->WriteSocket::m_criticalSequence)) {
		Send(g_pActiveConnection);
		CloseDataStream();
		return;
	}
	g_pNetworkManager->Kill();
	CloseDataStream();
}

// 68K 0x10604496 GetData__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040aed0
void BaseObjectManager::GetData()
{
	unsigned short type = GetWord();
	while (type != 0x2f) {
		unsigned short id = GetWord();
		GlobalGameObject* found = 0;
		for (unsigned int i = 0; (int) i < (int) (unsigned int) g_wObjectCount; i++) {
			GameObject* obj = g_pObjects[(unsigned short) i];
			if (obj != 0 && (unsigned short) obj->GetId() == id) {
				found = (GlobalGameObject*) obj;
				break;
			}
		}
		if (found->Receive(type, this) == 0) {
			Receive(type, found, this);
		}
		type = GetWord();
	}
}
