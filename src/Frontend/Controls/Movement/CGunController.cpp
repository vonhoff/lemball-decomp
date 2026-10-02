#include "../CGunController.h"

#include "../../../Views/Sound/CSoundView.h"
#include "../../../Visos/Animation/CPlayThruAnim.h"
#include "../../../Visos/Foundation/CBaseQueue.h"
#include "../../../Visos/Foundation/CVSPoint.h"
#include "../../../Visos/Foundation/VsTime.h"
#include "../../../Visos/Graphics/CGDI.h"
#include "../../../Visos/Graphics/CGraphicButton.h"
#include "../../../Visos/Graphics/CSurface.h"
#include "../../../Visos/Resources/Manifest.h"
#include "../../Windows/CSpriteWindow.h"
#include "../../Windows/CTrackWindow.h"
#include "../CGunButtons.h"
#include "../CTrackerButton.h"
#include "Frontend/Controls/GunControllerJunction.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CStaticAnim.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/CVSSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CGWnd.h"

#include <stdlib.h>

// FUNCTION: LEMBALL 0x0044d830
void CGunController::MoveUp()
{
	int* directionField;
	int remaining;
	int direction;
	int y;
	int bestY;
	int foundY;

	bestY = -1;
	foundY = -1;
	directionField = &m_junctions[0].m_direction;
	remaining = 8;
	do {
		direction = *directionField;
		if (direction != 3 && (y = directionField[-2]) < m_targetY && bestY < y) {
			if (direction != 2) {
				m_targetSide = direction;
			}
			foundY = directionField[-2];
			bestY = foundY;
			g_pSoundView->PlayEffect(SFX_RELOAD);
		}
		directionField += 8;
	} while (--remaining != 0);
	if (foundY != -1) {
		m_targetY = foundY;
	}
	m_moveStartTime = CurrentMilliTimer();
	m_moveEndTime = abs(m_targetY - m_gunY) * 3 + m_moveStartTime;
	m_moveStartY = m_gunY;
	m_verticalMoving = 1;
}
