#include "CTimedAnim.h"

#include "Engine/Animation/AnimationConstants.h"
#include "Engine/Time/VsTime.h"

// FUNCTION: LEMBALL 0x00435890
void CTimedAnim::SetAnimTime(unsigned long p_animTime)
{
	m_animTime = p_animTime;
}

// FUNCTION: LEMBALL 0x004358a0
void CTimedAnim::SetStartTime(unsigned long p_startTime)
{
	m_frameState = p_startTime;
}

// FUNCTION: LEMBALL 0x004358b0
void CTimedAnim::SetAnimDirection(int p_direction)
{
	m_direction = p_direction;
}

// FUNCTION: LEMBALL 0x00467170
unsigned int CTimedAnim::GetFrameNo()
{
	unsigned long time = m_fixedTime;
	if (time == ANIMATION_TIME_REALTIME) {
		time = CurrentMilliTimer();
	}
	time -= m_frameState;
	unsigned long remainder = time % m_animTime;
	unsigned long frame = (remainder * m_frames) / m_animTime;
	if (m_direction != ANIMATION_DIRECTION_FORWARD) {
		frame = m_frames - frame - 1;
	}
	return frame;
}
