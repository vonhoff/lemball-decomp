#include "CBaseFrontendDrawer.h"

#include "Visos/Text/CTextManager.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Frontend/Controls/CGunController.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"
#include "tagPRIMS.h"
#include "Visos/Graphics/Primitives/CSolidRect.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00446110
void CBaseFrontendDrawer::DrawFrame(CVSRect p_rect)
{
	int startX = p_rect.m_x;
	int startY = p_rect.m_y;
	int width = p_rect.m_width;
	int height = p_rect.m_height;
	int tileWidth;
	int tileHeight;
	{
		const CVSSize& tileSize = CAnimsManager::GetAnimSize(m_topFrameAnimId, 0);
		tileWidth = tileSize.m_width;
		tileHeight = tileSize.m_height;
	}
	width += tileWidth - 1;
	width -= width % tileWidth;
	height += tileHeight - 1;
	height -= height % tileHeight;
	{
		CVSRect frame(p_rect.m_x, p_rect.m_y, (short) width, (short) height);
		const CVSRect& frameBounds = frame;
		CSolidRect& line = m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount];
		line.m_bounds.m_width = frameBounds.m_width;
		line.m_bounds.m_height = frameBounds.m_height;
		line.m_bounds.m_x = frameBounds.m_x;
		line.m_bounds.m_y = frameBounds.m_y;
		line.m_colour = 0x10;
		m_primitiveBundle[m_primitiveBank].m_lines[m_framePrimitiveCount].Draw(m_gdi);
	}
	m_framePrimitiveCount++;
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVSPoint((short) startX, (short) startY),
							m_topFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	int x = tileWidth;
	int right = width - tileWidth;
	for (; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
								m_topFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
							m_topFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	int y = tileHeight;
	height -= tileHeight;
	for (; y < height; y += tileHeight) {
		m_staticAnim.m_frameState = 0;
		short currentY = (short) (startY + y);
		CVSPoint left((short) startX, currentY);
		CAnimsManager::DrawAnim(left, m_sideFrameAnimId, 0, (CAnimFrameBASE*) &m_staticAnim, NULL);
		m_staticAnim.m_frameState = 2;
		CAnimsManager::DrawAnim(CVSPoint((short) (width - tileWidth + startX), currentY),
								m_sideFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVSPoint((short) startX, (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
	for (x = tileWidth; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
								m_bottomFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								NULL);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							NULL);
}
