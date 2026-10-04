#include "CPauseWindow.h"

#include "Visos/Queues/CBaseQueue.h"
#include "Visos/Controls/CHotAreaList.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00444790
CPauseWindow::~CPauseWindow()
{
	m_parentWindow->m_hotAreaList->RemoveFromList(this);
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	delete[] m_menuItemRects;
	UnRegisterRemaps();
	if (m_borderAnims != NULL) {
		delete[] m_borderAnims;
	}
	UnLoad();
	g_pMasterInputQueue->Detach(this, 0);
}
