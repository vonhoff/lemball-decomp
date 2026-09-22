#include "CRepeatAnim.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x004671b0
void CRepeatAnim::StartAnim(unsigned long p_animTime)
{
	m_animTime = p_animTime;
	if (m_fixedTime == 0xffffffff) {
		m_frameState = timeGetTime();
		return;
	}
	m_frameState = m_fixedTime;
}
