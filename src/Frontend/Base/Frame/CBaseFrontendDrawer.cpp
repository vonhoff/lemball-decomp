#include "../CBaseFrontendDrawer.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Control/Game/GameMain.h"
#include "../../../Network/Game/CNetworkManager.h"
#include "../../../Views/Display/CMain2DDisplay.h"
#include "../../../Views/Sound/CSoundView.h"
#include "../../../Visos/Animation/CPlayThruAnim.h"
#include "../../../Visos/Foundation/CBaseQueue.h"
#include "../../../Visos/Foundation/CChangeList.h"
#include "../../../Visos/Foundation/CTextManager.h"
#include "../../../Visos/Foundation/CVSOStream.h"
#include "../../../Visos/Foundation/VsTime.h"
#include "../../../Visos/Graphics/CCopyToBackBuff.h"
#include "../../../Visos/Graphics/CCursor.h"
#include "../../../Visos/Graphics/CGDI.h"
#include "../../../Visos/Graphics/CSurface.h"
#include "../../../Visos/Network/CBaseNetwork.h"
#include "../../../Visos/Network/CConnect.h"
#include "../../../Visos/Resources/CMogRes.h"
#include "../../../Visos/Resources/CResBITMAP.h"
#include "../../../Visos/Resources/Manifest.h"
#include "../../Base/CBaseFrontendProcess.h"
#include "../../Controls/CGunButtons.h"
#include "../../Controls/CGunController.h"
#include "../../Controls/CHiliteController.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Support/CUserActionMessage.h"
#include "Frontend/Support/CoordPair.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CBigBitmap.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CPrimitive.h"
#include "Visos/Graphics/CSolidRect.h"

#include <new.h>
#include <string.h>

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
							0);
	int x = tileWidth;
	int right = width - tileWidth;
	for (; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
								m_topFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) startY),
							m_topFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							0);
	int y = tileHeight;
	height -= tileHeight;
	for (; y < height; y += tileHeight) {
		m_staticAnim.m_frameState = 0;
		short currentY = (short) (startY + y);
		CVSPoint left((short) startX, currentY);
		CAnimsManager::DrawAnim(left, m_sideFrameAnimId, 0, (CAnimFrameBASE*) &m_staticAnim, 0);
		m_staticAnim.m_frameState = 2;
		CAnimsManager::DrawAnim(CVSPoint((short) (width - tileWidth + startX), currentY),
								m_sideFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 0;
	CAnimsManager::DrawAnim(CVSPoint((short) startX, (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							0);
	for (x = tileWidth; right > x; x += tileWidth) {
		m_staticAnim.m_frameState = 1;
		CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
								m_bottomFrameAnimId,
								0,
								(CAnimFrameBASE*) &m_staticAnim,
								0);
	}
	m_staticAnim.m_frameState = 2;
	CAnimsManager::DrawAnim(CVSPoint((short) (startX + x), (short) (startY + y)),
							m_bottomFrameAnimId,
							0,
							(CAnimFrameBASE*) &m_staticAnim,
							0);
}
