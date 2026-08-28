#include "RepeatAnim.h"

#include "../Foundation/VsTime.h"

// 68K 0x10200082 StartAnim__11CRepeatAnimFUl
// FUNCTION: LEMBALL 0x004671b0
void RepeatAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == 0xffffffff) {
		m_frameState = CurrentMilliTimer();
		return;
	}
	m_frameState = m_fixedTime;
}
