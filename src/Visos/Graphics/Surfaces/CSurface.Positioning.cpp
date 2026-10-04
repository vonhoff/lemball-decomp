#include "CSurface.h"

#include "CGDIDevice.h"
#include "Platform/Windows/Graphics/CDibContext.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x0046d5b0
void CSurface::Move(const CVSPoint& p_position)
{
	const CVSPoint& position = m_surfaceRect;
	CVSPoint delta(p_position.m_x - position.m_x, p_position.m_y - position.m_y);

	if (m_parentSurface != (CSurface*) g_pGdiHelperTarget) {
		CRITICAL_SECTION* lock = (CRITICAL_SECTION*) m_lock;
		EnterCriticalSection(lock);
		{
			CVSRect& surface = m_surfaceRect;
			surface.m_x = p_position.m_x;
			surface.m_y = p_position.m_y;
		}
		CVSRect oldRect(m_windowRect);
		{
			CVSRect& window = m_windowRect;
			const CVSRect& surface = m_surfaceRect;
			window.m_width = surface.m_width;
			window.m_height = surface.m_height;
			const CVSPoint& origin = surface;
			window.m_x = origin.m_x;
			window.m_y = origin.m_y;
		}

		const CVSSize& parentSize = m_parentSurface->m_windowRect;
		CVSRect& clipped = m_windowRect;
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
			const CVSSize& windowSize = m_windowRect;
			CVSSize& clipSize = m_clipRect;
			short height = windowSize.m_height;
			clipSize.m_width = windowSize.m_width;
			clipSize.m_height = height;
		}
		const CVSSize& newSize = m_windowRect;
		short height = newSize.m_height;
		if (newSize.m_width != oldRect.m_width || oldRect.m_height != height) {
			Resize(newSize);
		}
		if (m_platformBitmap != NULL) {
			g_pTargetGraphicsDriver->DestroyDibContext((CDibContext*) m_platformBitmap);
			m_platformBitmap = NULL;
		}
		m_bitmapPixelCount = 0;
		CreateLinePtrs();
		LeaveCriticalSection(lock);
		for (SurfaceListNode* node = m_childSurfaceHead; node != NULL; node = node->m_next) {
			node->m_surface->MoveRel(delta);
		}
	}
}
