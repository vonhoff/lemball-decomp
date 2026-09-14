#include "WindowsCursorMotionState.h"

#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Animation/LemmingAnimsManager.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00432650
WindowsCursorMotionState::~WindowsCursorMotionState()
{
}

// FUNCTION: LEMBALL 0x00432680
void WindowsCursorMotionState::SendCursorPositionMessage()
{
	int x;
	int y;
	Message message;
	message.type = 1;
	memset(&message.time, 0, 16);
	int screenX = m_fixedX >> 12;
	int screenY = m_fixedY >> 12;
	m_map->ScreenToGame(screenX, screenY, x, y);
	message.code = x;
	message.payload = (void*) y;
	m_aiQueue->Post(message);
}

// FUNCTION: LEMBALL 0x004326e0
void WindowsCursorMotionState::ProcessCursorMotion()
{
	unsigned int now = CurrentMilliTimer();
	if (m_horizontalActive) {
		int elapsed = now - m_lastTickX;
		m_lastTickX = now;
		m_velocityX += m_accelerationX * elapsed / 20;
		if (m_velocityX > 0x5000) {
			m_velocityX = 0x5000;
		}
		if (m_velocityX < -0x5000) {
			m_velocityX = -0x5000;
		}
	}
	if (m_verticalActive) {
		int elapsed = now - m_lastTickY;
		m_lastTickY = now;
		m_velocityY += m_accelerationY * elapsed / 20;
		if (m_velocityY > 0x5000) {
			m_velocityY = 0x5000;
		}
		if (m_velocityY < -0x5000) {
			m_velocityY = -0x5000;
		}
	}
	if (m_horizontalActive || m_verticalActive || m_positionDirty) {
		if (m_horizontalActive || m_verticalActive) {
			m_fixedX += m_velocityX;
			m_fixedY += m_velocityY;
		}
		SendCursorPositionMessage();
		m_positionDirty = 0;
	}
}

// FUNCTION: LEMBALL 0x004327b0
void WindowsCursorMotionState::DrawCursorMotionAtCurrentPosition(undefined4 p_unused)
{
	int x = (m_fixedX >> 12) - m_aux0;
	int y = (m_fixedY >> 12) - m_aux1;
	m_anims->DrawAnim((short) x, (short) y, RES_CURSORS_HAND, 0, 0, 0);
}

// FUNCTION: LEMBALL 0x004327e0
void WindowsCursorMotionState::DrawCursorMotionAtPoint(undefined4 p_unused, const VsPoint& p_position)
{
	m_anims->DrawAnim(p_position.m_x, p_position.m_y, RES_CURSORS_HAND, 0, 0, 0);
}

// FUNCTION: LEMBALL 0x00432810
void WindowsCursorMotionState::SetCursorMotionPoint(const VsPoint& p_position)
{
	m_positionDirty = 1;
	m_horizontalActive = 0;
	m_verticalActive = 0;
	m_fixedY = p_position.m_y << 12;
	m_fixedX = p_position.m_x << 12;
}

// FUNCTION: LEMBALL 0x00432840
VsPoint WindowsCursorMotionState::GetCursorMotionPoint()
{
	return VsPoint((short) (m_fixedX >> 12), (short) (m_fixedY >> 12));
}

// FUNCTION: LEMBALL 0x00432920
void WindowsCursorMotionState::SetCursorMotionAuxValues(unsigned int p_aux0, unsigned int p_aux1)
{
	m_aux0 = p_aux0;
	m_aux1 = p_aux1;
}
