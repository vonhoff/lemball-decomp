#include "CCursorMotion.h"

#include "../../Engine/Resources/Manifest.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Math/CFixed.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Engine/Time/VsTime.h"
#include "GameView/Animation/CLemmingAnimsManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "Map/CMap.h"

#include <string.h>

namespace
{
enum {
	CURSOR_INITIAL_X_FIXED = 160 * FIXED_POINT_ONE,
	CURSOR_INITIAL_Y_FIXED = 100 * FIXED_POINT_ONE,
	CURSOR_MAX_VELOCITY_FIXED = 5 * FIXED_POINT_ONE,
	CURSOR_VELOCITY_ACCELERATION_PER_20_MS_FIXED = 0xcc,
};
}

// FUNCTION: LEMBALL 0x00432590
CCursorMotion::CCursorMotion(CLemmingAnimsManager* p_anims, CAI* p_ai, CMap* p_map)
	: m_fixedX(DEBUG_SENTINEL), m_fixedY(DEBUG_SENTINEL), m_velocityX(DEBUG_SENTINEL), m_velocityY(DEBUG_SENTINEL),
	  m_accelerationX(DEBUG_SENTINEL), m_accelerationY(DEBUG_SENTINEL)
{
	m_ai = p_ai;
	m_map = p_map;
	m_anims = p_anims;
	m_aiQueue = p_ai->m_aiQueue;
	m_velocityX = 0;
	m_velocityY = 0;
	m_accelerationX = 0;
	m_accelerationY = 0;
	m_horizontalActive = 0;
	m_verticalActive = 0;
	m_fixedX = CURSOR_INITIAL_X_FIXED;
	m_fixedY = CURSOR_INITIAL_Y_FIXED;
	PostPosition();
}

// FUNCTION: LEMBALL 0x00432650
CCursorMotion::~CCursorMotion()
{
}

// FUNCTION: LEMBALL 0x00432680
void CCursorMotion::PostPosition()
{
	int x;
	int y;
	tagMESSAGE message;
	message.m_type = AI_MESSAGE_CURSOR_POSITION;
	memset(&message.m_time, 0, 16);
	int screenX = m_fixedX >> FIXED_POINT_FRACTION_BITS;
	int screenY = m_fixedY >> FIXED_POINT_FRACTION_BITS;
	m_map->ScreenToGame(screenX, screenY, x, y);
	message.m_code = x;
	message.m_payload = (void*) y;
	m_aiQueue->Post(message);
}

// FUNCTION: LEMBALL 0x004326e0
void CCursorMotion::Process()
{
	unsigned int now = CurrentMilliTimer();
	if (m_horizontalActive) {
		int elapsed = now - m_lastTickX;
		m_lastTickX = now;
		m_velocityX += m_accelerationX * elapsed / 20;
		if (m_velocityX > CURSOR_MAX_VELOCITY_FIXED) {
			m_velocityX = CURSOR_MAX_VELOCITY_FIXED;
		}
		if (m_velocityX < -CURSOR_MAX_VELOCITY_FIXED) {
			m_velocityX = -CURSOR_MAX_VELOCITY_FIXED;
		}
	}
	if (m_verticalActive) {
		int elapsed = now - m_lastTickY;
		m_lastTickY = now;
		m_velocityY += m_accelerationY * elapsed / 20;
		if (m_velocityY > CURSOR_MAX_VELOCITY_FIXED) {
			m_velocityY = CURSOR_MAX_VELOCITY_FIXED;
		}
		if (m_velocityY < -CURSOR_MAX_VELOCITY_FIXED) {
			m_velocityY = -CURSOR_MAX_VELOCITY_FIXED;
		}
	}
	if (m_horizontalActive || m_verticalActive || m_positionDirty) {
		if (m_horizontalActive || m_verticalActive) {
			m_fixedX += m_velocityX;
			m_fixedY += m_velocityY;
		}
		PostPosition();
		m_positionDirty = 0;
	}
}

// FUNCTION: LEMBALL 0x004327b0
void CCursorMotion::Draw(unsigned int p_unused)
{
	int x = (m_fixedX >> FIXED_POINT_FRACTION_BITS) - m_drawOffsetX;
	int y = (m_fixedY >> FIXED_POINT_FRACTION_BITS) - m_drawOffsetY;
	m_anims->DrawAnim((short) x, (short) y, RES_CURSORS_HAND, 0, 0, NULL);
}

// FUNCTION: LEMBALL 0x004327e0
void CCursorMotion::DrawAt(unsigned int p_unused, const CVSPoint& p_position)
{
	m_anims->DrawAnim(p_position.m_x, p_position.m_y, RES_CURSORS_HAND, 0, 0, NULL);
}

// FUNCTION: LEMBALL 0x00432810
void CCursorMotion::SetPosition(const CVSPoint& p_position)
{
	m_positionDirty = 1;
	m_horizontalActive = 0;
	m_verticalActive = 0;
	m_fixedY = p_position.m_y << FIXED_POINT_FRACTION_BITS;
	m_fixedX = p_position.m_x << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x00432840
CVSPoint CCursorMotion::GetPosition()
{
	return CVSPoint((short) (m_fixedX >> FIXED_POINT_FRACTION_BITS), (short) (m_fixedY >> FIXED_POINT_FRACTION_BITS));
}

// FUNCTION: LEMBALL 0x00432860
void CCursorMotion::StopVertical()
{
	m_accelerationY = 0;
	m_velocityY = 0;
	m_verticalActive = 0;
}

// FUNCTION: LEMBALL 0x00432870
void CCursorMotion::StopHorizontal()
{
	m_accelerationX = 0;
	m_velocityX = 0;
	m_horizontalActive = 0;
}

// FUNCTION: LEMBALL 0x00432880
void CCursorMotion::StartHorizontal(unsigned int p_positive)
{
	if (!m_horizontalActive) {
		unsigned int now = CurrentMilliTimer();
		m_accelerationX = (p_positive ? CFixed(CURSOR_VELOCITY_ACCELERATION_PER_20_MS_FIXED)
									  : CFixed(-CURSOR_VELOCITY_ACCELERATION_PER_20_MS_FIXED))
							  .m_value;
		m_lastTickX = now;
		m_horizontalActive = 1;
	}
}

// FUNCTION: LEMBALL 0x004328d0
void CCursorMotion::StartVertical(unsigned int p_positive)
{
	if (!m_verticalActive) {
		unsigned int now = CurrentMilliTimer();
		m_accelerationY = (p_positive ? CFixed(CURSOR_VELOCITY_ACCELERATION_PER_20_MS_FIXED)
									  : CFixed(-CURSOR_VELOCITY_ACCELERATION_PER_20_MS_FIXED))
							  .m_value;
		m_lastTickY = now;
		m_verticalActive = 1;
	}
}

// FUNCTION: LEMBALL 0x00432920
void CCursorMotion::SetDrawOffset(unsigned int p_drawOffsetX, unsigned int p_drawOffsetY)
{
	m_drawOffsetX = p_drawOffsetX;
	m_drawOffsetY = p_drawOffsetY;
}
