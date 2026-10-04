#include "CTrackWindow.h"

#include "Visos/Controls/CHotAreaList.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

// FUNCTION: LEMBALL 0x0044e8c0
CTrackWindow::~CTrackWindow()
{
	if (m_parent->m_lifecycleRefs == 1) {
		m_parent->m_hotAreaList->RemoveFromList(this);
	}
}
