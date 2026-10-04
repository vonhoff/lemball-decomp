#include "CPlayThruAnim.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x004671e0
void CPlayThruAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == ANIMATION_TIME_REALTIME) {
		m_frameState = timeGetTime();
		return;
	}
	m_frameState = m_fixedTime;
}

// FUNCTION: LEMBALL 0x00467210
unsigned int CPlayThruAnim::GetFrameNo()
{
	unsigned long duration;
	unsigned int elapsed;
	unsigned int frame;

	if (m_fixedTime == ANIMATION_TIME_REALTIME) {
		elapsed = timeGetTime() - m_frameState;
	}
	else {
		elapsed = m_fixedTime - m_frameState;
	}
	duration = m_animTime;
	if (duration > elapsed) {
		frame = (m_frames * elapsed) / duration;
	}
	else {
		frame = m_frames - 1;
	}
	if (m_direction != ANIMATION_DIRECTION_FORWARD) {
		frame = (m_frames - frame) - 1;
	}
	return frame;
}
