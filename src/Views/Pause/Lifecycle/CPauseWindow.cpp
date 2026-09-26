#include "Views/Pause/CPauseWindow.h"

#include "Visos/Foundation/CBaseQueue.h"
#include "Visos/Graphics/CHotAreaList.h"
#include "Visos/Graphics/CPVGWnd.h"

// FUNCTION: LEMBALL 0x00444790
CPauseWindow::~CPauseWindow()
{
	m_parentWindow->m_hotAreaList->RemoveFromList(this);
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	delete[] m_menuItemRects;
	UnRegisterRemaps();
	if (m_borderAnims != 0) {
		delete[] m_borderAnims;
	}
	UnLoad();
	g_pMasterInputQueue->Detach(this, 0);
}
