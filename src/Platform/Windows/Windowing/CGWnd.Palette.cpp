#include "CGWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "Visos/Resources/Types/CResPALETTE.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Graphics/Surfaces/CGDIDevice.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Graphics/Surfaces/CPVSurface.h"

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
