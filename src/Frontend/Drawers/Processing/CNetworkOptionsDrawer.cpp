#include "Frontend/Drawers/CNetworkOptionsDrawer.h"

#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Frontend/Processes/CNetworkOptionsProc.h"
#include "Frontend/Support/CEntryHandler.h"
#include "Network/Game/CNetworkManager.h"
#include "Views/Sound/CSoundView.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Time/VsTime.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/CNetworkAddress.h"

#include <stddef.h>

extern char* g_szBroadcastPeerName;

enum {
	NETWORK_OPTIONS_REDRAW_INTERVAL_MS = 500
};

// FUNCTION: LEMBALL 0x004548c0
void CNetworkOptionsDrawer::Processing()
{
	unsigned long now;
	unsigned long duration;
	char* ident;
	char* peer;
	CConnect** current;
	CConnect** connections;
	int index;
	int activation;
	int acceptedPlayer;

	if (m_drawnMessage != (unsigned int) m_message) {
		return;
	}
	if (m_startPending != 0) {
		StartBroadcast();
		m_startPending = 0;
	}
	if (m_pendingStage != NETWORK_OPTIONS_EDIT_NONE) {
		StartEditing(m_pendingStage, 1);
		m_pendingStage = NETWORK_OPTIONS_EDIT_NONE;
	}
	if (m_pendingEvent != NETWORK_OPTIONS_PENDING_EVENT_NONE) {
		LastError();
	}
	now = CurrentMilliTimer();
	if (now - m_lastDrawTime >= NETWORK_OPTIONS_REDRAW_INTERVAL_MS) {
		m_redrawPending = m_redrawPending == 0;
		now = CurrentMilliTimer();
		m_lastDrawTime = now;
	}
	if (g_pNetworkManager != NULL) {
		ident = g_pBroadcastAddress->GetStr();
		peer = g_szBroadcastPeerName;
		if (m_localAddressText != ident) {
			m_backBufferNeeded = 1;
			m_localAddressText = ident;
		}
		if (m_localComputerName != peer) {
			m_backBufferNeeded = 1;
			m_localComputerName = peer;
		}
		if (m_networkState == NETWORK_OPTIONS_HANDLERS_CURRENT) {
			if (g_pNetworkManager->m_connectionsChanged != 0) {
				g_pNetworkManager->m_connectionsChanged = 0;
				m_networkState = NETWORK_OPTIONS_HANDLERS_CURRENT;
				m_backBufferNeeded = 1;
				InitialiseHandlers();
			}
		}
		else {
			m_networkState = NETWORK_OPTIONS_HANDLERS_CURRENT;
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
				if (acceptedPlayer != NETWORK_OPTIONS_NO_PLAYER_INDEX) {
					CConnect* connection = connections[acceptedPlayer];
					if (connection != NULL) {
						((CNetworkOptionsProc*) g_pCurrentFrontendProcess)->Reject(connection);
					}
				}
				m_acceptedPlayer = index;
				if (*current != NULL) {
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
	if (m_message != NETWORK_OPTIONS_MESSAGE_NONE) {
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
