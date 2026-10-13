#include "CPlayThruAnim.h"

#include "Platform/Windows/VsTime.h"

// FUNCTION: LEMBALL 0x004671e0
void CPlayThruAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == ANIMATION_TIME_REALTIME) {
		m_frameState = CurrentMilliTimer();
		return;
	}
	m_frameState = m_fixedTime;
}

// FUNCTION: LEMBALL 0x00467210
unsigned int CPlayThruAnim::GetFrameNo()
{
	unsigned int elapsed;
	unsigned int frame;

	if (m_fixedTime == ANIMATION_TIME_REALTIME) {
		elapsed = CurrentMilliTimer() - m_frameState;
	}
	else {
		elapsed = m_fixedTime - m_frameState;
	}
	if (m_animTime > elapsed) {
		frame = (m_frames * elapsed) / m_animTime;
	}
	else {
		frame = m_frames - 1;
	}
	if (m_direction != ANIMATION_DIRECTION_FORWARD) {
		frame = (m_frames - frame) - 1;
	}
	return frame;
}
