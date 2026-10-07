#include "CRepeatAnim.h"
#include "Engine/Time/VsTime.h"

// FUNCTION: LEMBALL 0x004671b0
void CRepeatAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == ANIMATION_TIME_REALTIME) {
		m_frameState = CurrentMilliTimer();
		return;
	}
	m_frameState = m_fixedTime;
}
