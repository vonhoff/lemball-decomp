#include "PlayThruAnim.h"

#include "../Foundation/VsTime.h"

// 68K 0x102000d0 StartAnim__13CPlayThruAnimFUl
// FUNCTION: LEMBALL 0x004671e0
void PlayThruAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == 0xffffffff) {
		m_frameState = CurrentMilliTimer();
		return;
	}
	m_frameState = m_fixedTime;
}

// 68K 0x10200120 GetFrameNo__13CPlayThruAnimFv
// FUNCTION: LEMBALL 0x00467210
unsigned int PlayThruAnim::GetFrameNo()
{
	unsigned int elapsed;
	unsigned int frame;

	if (m_fixedTime == 0xffffffff) {
		elapsed = CurrentMilliTimer() - m_frameState;
	}
	else {
		elapsed = m_fixedTime - m_frameState;
	}
	frame = (m_animTime <= elapsed) ? m_frames - 1 : (m_frames * elapsed) / m_animTime;
	if (m_direction != 1) {
		frame = (m_frames - frame) - 1;
	}
	return frame;
}
