#include "WindowsCursorMotionState.h"

#include "../../AI/Navigation/CAi.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Animation/CLemmingAnimsManager.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/VsDebug.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00432590
WindowsCursorMotionState::WindowsCursorMotionState(CLemmingAnimsManager* p_anims, CAi* p_ai, CMap* p_map)
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
	m_fixedX = 0xa0000;
	m_fixedY = 0x64000;
	SendCursorPositionMessage();
}

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
void WindowsCursorMotionState::DrawCursorMotionAtCurrentPosition(unsigned int p_unused)
{
	int x = (m_fixedX >> 12) - m_aux0;
	int y = (m_fixedY >> 12) - m_aux1;
	m_anims->DrawAnim((short) x, (short) y, RES_CURSORS_HAND, 0, 0, 0);
}

// FUNCTION: LEMBALL 0x004327e0
void WindowsCursorMotionState::DrawCursorMotionAtPoint(unsigned int p_unused, const CVsPoint& p_position)
{
	m_anims->DrawAnim(p_position.m_x, p_position.m_y, RES_CURSORS_HAND, 0, 0, 0);
}

// FUNCTION: LEMBALL 0x00432810
void WindowsCursorMotionState::SetCursorMotionPoint(const CVsPoint& p_position)
{
	m_positionDirty = 1;
	m_horizontalActive = 0;
	m_verticalActive = 0;
	m_fixedY = p_position.m_y << 12;
	m_fixedX = p_position.m_x << 12;
}

// FUNCTION: LEMBALL 0x00432840
CVsPoint WindowsCursorMotionState::GetCursorMotionPoint()
{
	return CVsPoint((short) (m_fixedX >> 12), (short) (m_fixedY >> 12));
}

// FUNCTION: LEMBALL 0x00432860
void WindowsCursorMotionState::StopVerticalMotion()
{
	m_accelerationY = 0;
	m_velocityY = 0;
	m_verticalActive = 0;
}

// FUNCTION: LEMBALL 0x00432870
void WindowsCursorMotionState::StopHorizontalMotion()
{
	m_accelerationX = 0;
	m_velocityX = 0;
	m_horizontalActive = 0;
}

// FUNCTION: LEMBALL 0x00432880
void WindowsCursorMotionState::StartHorizontalMotion(unsigned int p_positive)
{
	if (!m_horizontalActive) {
		unsigned int now = CurrentMilliTimer();
		m_accelerationX = (p_positive ? CFixed(0xcc) : CFixed(-0xcc)).m_value;
		m_lastTickX = now;
		m_horizontalActive = 1;
	}
}

// FUNCTION: LEMBALL 0x004328d0
void WindowsCursorMotionState::StartVerticalMotion(unsigned int p_positive)
{
	if (!m_verticalActive) {
		unsigned int now = CurrentMilliTimer();
		m_accelerationY = (p_positive ? CFixed(0xcc) : CFixed(-0xcc)).m_value;
		m_lastTickY = now;
		m_verticalActive = 1;
	}
}

// FUNCTION: LEMBALL 0x00432920
void WindowsCursorMotionState::SetCursorMotionAuxValues(unsigned int p_aux0, unsigned int p_aux1)
{
	m_aux0 = p_aux0;
	m_aux1 = p_aux1;
}
