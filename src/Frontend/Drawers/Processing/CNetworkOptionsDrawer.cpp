#include "Frontend/Drawers/CNetworkOptionsDrawer.h"

#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Frontend/Processes/CNetworkOptionsProc.h"
#include "Frontend/Support/CEntryHandler.h"
#include "Network/Game/CNetworkManager.h"
#include "Views/Sound/CSoundView.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/VsTime.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/CNetworkAddress.h"

extern char* g_szBroadcastPeerName;

// FUNCTION: LEMBALL 0x004548c0
void CNetworkOptionsDrawer::Processing()
{
	unsigned long now;
	unsigned long duration;
	char* ident;
	char* peer;
	CConnect* connection;
	int index;
	CConnect** current;
	CConnect** connections;
	int activation;
	int acceptedPlayer;

	if (m_messageDirty != (unsigned int) m_message) {
		return;
	}
	if (m_startPending != 0) {
		StartBroadcast();
		m_startPending = 0;
	}
	if (m_pendingStage != 0) {
		StartEditing(m_pendingStage, 1);
		m_pendingStage = 0;
	}
	if (m_pendingEvent != 0) {
		LastError();
	}
	now = CurrentMilliTimer();
	if (now - m_lastDrawTime >= 500) {
		m_redrawPending = m_redrawPending == 0;
		now = CurrentMilliTimer();
		m_lastDrawTime = now;
	}
	if (g_pNetworkManager != 0) {
		ident = g_pBroadcastAddress->GetStr();
		peer = g_szBroadcastPeerName;
		if (m_stopPending != ident) {
			m_backBufferNeeded = 1;
			m_stopPending = ident;
		}
		if (m_connectionState != peer) {
			m_backBufferNeeded = 1;
			m_connectionState = peer;
		}
		if (m_networkState == 0) {
			if (g_pNetworkManager->m_connectionsChanged != 0) {
				g_pNetworkManager->m_connectionsChanged = 0;
				m_networkState = 0;
				m_backBufferNeeded = 1;
				InitialiseHandlers();
			}
		}
		else {
			m_networkState = 0;
			m_backBufferNeeded = 1;
			InitialiseHandlers();
		}
		connections = g_pNetworkManager->m_connections;
		current = connections;
		index = 0;
		do {
			if (m_playerEntries[index].m_pressed != 0 && m_acceptedPlayer != index) {
				g_pSoundView->PlayEffect(SFX_DRUM1);
				acceptedPlayer = m_acceptedPlayer;
				if (acceptedPlayer != -1) {
					connection = connections[acceptedPlayer];
					if (connection != 0) {
						((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Reject(connection);
					}
				}
				m_acceptedPlayer = index;
				if (*current != 0) {
					activation = m_playerEntries[index].m_activationState;
					if (activation != 0) {
						Lock();
					}
					((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Accept(*current, activation);
				}
			}
			m_playerEntries[index].m_pressed = 0;
			current = current + 1;
			index = index + 1;
		} while (index < 10);
	}
	if (m_message != 0) {
		duration = m_messageDuration;
		if (duration != 0) {
			now = CurrentMilliTimer();
			if (now - m_messageStartTime > duration) {
				m_message = 1;
				m_backBufferNeeded = 1;
				m_messageDuration = 0;
			}
		}
	}
}
