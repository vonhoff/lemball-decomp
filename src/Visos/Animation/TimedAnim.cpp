#include "TimedAnim.h"

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
// STUB: LEMBALL 0x00467170
unsigned int TimedAnim::GetFrameNo()
{
	return 0;
}
