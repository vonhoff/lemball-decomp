#include "../CGWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "../../Resources/CResPALETTE.h"
#include "../../Target/Graphics/CGraphicsDriver.h"
#include "../../Target/Graphics/CGraphicsState.h"
#include "../CCursor.h"
#include "../CGDI.h"
#include "../CGDIDevice.h"
#include "../CPVGWnd.inl"
#include "../CSurface.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Graphics/CPVSurface.h"
#include "Visos/Graphics/CPVWnd.h"
#include "Visos/Graphics/CWnd.h"

#include <new.h>
#include <windows.h>

// FUNCTION: LEMBALL 0x00464490
void CGWnd::AttachPalette(unsigned long p_paletteId)
{
	CResPALETTE* palette;

	if (p_paletteId == 0) {
		return;
	}
	palette = CResPALETTE::Load(p_paletteId);
	if (palette->m_loaded != 0) {
		palette->m_age = 0;
	}
	else {
		palette->LoadData();
	}
	palette->m_directUseCount = palette->m_directUseCount + 1;
	m_gdi->m_renderTarget->AttachPalette(palette);
	palette->m_directUseCount = palette->m_directUseCount - 1;
	palette->UnLoad();
	m_paletteResourceId = p_paletteId;
}
