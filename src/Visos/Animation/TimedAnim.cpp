#include "TimedAnim.h"

#include "../Foundation/VsTime.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x10115d96 SetAnimTime__10CTimedAnimFUl
// FUNCTION: LEMBALL 0x00435890
void TimedAnim::SetAnimTime(unsigned long p_animTime)
{
	m_animTime = p_animTime;
}

// 68K 0x10115dc8 SetStartTime__10CTimedAnimFUl
// FUNCTION: LEMBALL 0x004358a0
void TimedAnim::SetStartTime(unsigned long p_startTime)
{
	m_frameState = p_startTime;
}

// 68K 0x10115dfa SetAnimDirection__10CTimedAnimFi
// FUNCTION: LEMBALL 0x004358b0
void TimedAnim::SetAnimDirection(int p_direction)
{
	m_direction = p_direction;
}

// 68K 0x1020000c GetFrameNo__10CTimedAnimFv
// FUNCTION: LEMBALL 0x00467170
unsigned int TimedAnim::GetFrameNo()
{
	unsigned long time = m_fixedTime;
	if (time == 0xffffffff) {
		time = timeGetTime();
	}
	time -= m_frameState;
	unsigned int frame = ((time % m_animTime) * m_frames) / m_animTime;
	if (m_direction != 1) {
		frame = m_frames - frame - 1;
	}
	return frame;
}
