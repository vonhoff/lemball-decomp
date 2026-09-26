#include "Views/Pause/CPauseWindow.h"

#include "Visos/Foundation/CBaseQueue.h"
#include "Visos/Graphics/CReceiveWindowState.h"

extern char* g_apPauseMenuLabels[15];

// FUNCTION: LEMBALL 0x00443af0
void CPauseWindow::Initialise()
{
	int pauseMessage;
	int index = 0;
	pauseMessage = m_pauseMessage;
	for (int menu = 0; menu <= pauseMessage; menu++) {
		m_menuItemCount = 0;
		do {
			m_menuItemCount++;
			index++;
		} while (g_apPauseMenuLabels[index] != 0);
		index++;
	}
	m_minimumSelection = 0;
	m_unavailableItems = 0;
	m_menuLabels = g_apPauseMenuLabels + index - m_menuItemCount - 1;
	switch (pauseMessage) {
	case 0:
		m_minimumSelection = 1;
		if (!m_receiverState->GetPauser()) {
			m_minimumSelection++;
			m_unavailableItems++;
		}
		m_selection = m_minimumSelection;
		break;
	case 3:
		m_minimumSelection = 1;
		m_selection = 2;
		break;
	case 1:
		m_minimumSelection = 1;
		m_selection = 1;
		break;
	case 2:
		m_minimumSelection = 1;
		m_selection = 1;
		break;
	case 4:
		m_minimumSelection = 1;
		m_selection = 1;
		break;
	default:
		m_selection = 0;
	}
	m_initialSelection = m_selection;
	g_pMasterInputQueue->Attach(this, 0);
	m_vramSurface = 0;
	m_menuItemRects = (CVsPoint*) (void*) new CVsRect[m_menuItemCount];
	RegisterRemaps();
	m_loaded = 0;
	m_borderAnims = 0;
}
