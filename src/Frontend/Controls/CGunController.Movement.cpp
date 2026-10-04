#include "CGunController.h"

#include "Views/Sound/CSoundView.h"
#include "Visos/Time/VsTime.h"
#include "GunControllerJunction.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Animation/CAnimsManager.h"

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

	bestY = GUN_CONTROLLER_ABOVE_TOP_BOUNDARY_Y;
	foundY = GUN_JUNCTION_COORDINATE_UNASSIGNED;
	directionField = &m_junctions[0].m_direction;
	remaining = 8;
	do {
		direction = *directionField;
		if (direction != GUN_JUNCTION_UNASSIGNED && (y = directionField[-2]) < m_targetY && bestY < y) {
			if (direction != GUN_JUNCTION_BOTH) {
				m_targetSide = direction;
			}
			foundY = directionField[-2];
			bestY = foundY;
			g_pSoundView->PlayEffect(SFX_RELOAD);
		}
		directionField += 8;
	} while (--remaining != 0);
	if (foundY != GUN_JUNCTION_COORDINATE_UNASSIGNED) {
		m_targetY = foundY;
	}
	m_moveStartTime = CurrentMilliTimer();
	m_moveEndTime = abs(m_targetY - m_gunY) * 3 + m_moveStartTime;
	m_moveStartY = m_gunY;
	m_verticalMoving = 1;
}
