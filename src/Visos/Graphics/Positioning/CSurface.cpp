#include "Visos/Graphics/CSurface.h"

#include "Visos/Graphics/CGDIDevice.h"
#include "Visos/Target/Graphics/CDibContext.h"
#include "Visos/Target/Graphics/CGraphicsDriver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x0046d5b0
void CSurface::Move(const CVsPoint& p_position)
{
	const CVsPoint& position = m_surfaceRect;
	CVsPoint delta(p_position.m_x - position.m_x, p_position.m_y - position.m_y);

	if (m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		CRITICAL_SECTION* lock = (CRITICAL_SECTION*) m_lock;
		EnterCriticalSection(lock);
		{
			CVsRect& surface = m_surfaceRect;
			surface.m_x = p_position.m_x;
			surface.m_y = p_position.m_y;
		}
		CVsRect oldRect(m_windowRect);
		{
			CVsRect& window = m_windowRect;
			const CVsRect& surface = m_surfaceRect;
			window.m_width = surface.m_width;
			window.m_height = surface.m_height;
			const CVsPoint& origin = surface;
			window.m_x = origin.m_x;
			window.m_y = origin.m_y;
		}

		const CVsSize& parentSize = m_parentSurface->m_windowRect;
		CVsRect& clipped = m_windowRect;
		short parentWidth = parentSize.m_width;
		short parentHeight = parentSize.m_height;

		short left = clipped.m_x;
		if (left < 0) {
			clipped.m_width += left;
			clipped.m_x = 0;
		}
		left = clipped.m_x;
		if (parentWidth < (short) (left + clipped.m_width)) {
			clipped.m_width = parentWidth - left;
		}
		short top = clipped.m_y;
		if (top < 0) {
			clipped.m_height += top;
			clipped.m_y = 0;
		}
		top = clipped.m_y;
		if (parentHeight < (short) (top + clipped.m_height)) {
			clipped.m_height = parentHeight - top;
		}
		if (clipped.m_width <= 0 || clipped.m_height <= 0) {
			clipped.m_height = 0;
			clipped.m_width = 0;
			clipped.m_y = 0;
			clipped.m_x = 0;
		}
		{
			const CVsSize& windowSize = m_windowRect;
			CVsSize& clipSize = m_clipRect;
			short height = windowSize.m_height;
			clipSize.m_width = windowSize.m_width;
			clipSize.m_height = height;
		}
		const CVsSize& newSize = m_windowRect;
		short height = newSize.m_height;
		if (newSize.m_width != oldRect.m_width || oldRect.m_height != height) {
			Resize(newSize);
		}
		if (m_platformBitmap != 0) {
			g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
			m_platformBitmap = 0;
		}
		m_bitmapPixelCount = 0;
		CreateLinePtrs();
		LeaveCriticalSection(lock);
		for (SurfaceListNode* node = m_childSurfaceHead; node != 0; node = node->m_next) {
			node->m_surface->MoveRel(delta);
		}
	}
}
