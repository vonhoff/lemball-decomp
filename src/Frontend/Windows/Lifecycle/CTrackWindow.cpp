#include "../CTrackWindow.h"

#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaList.h"
#include "Visos/Graphics/CPVGWnd.h"

// FUNCTION: LEMBALL 0x0044e8c0
CTrackWindow::~CTrackWindow()
{
	if (m_parent->m_lifecycleRefs == 1) {
		m_parent->m_hotAreaList->RemoveFromList(this);
	}
}
