#include "CFixedTimeAnim.h"

#include "AnimationConstants.h"

// FUNCTION: LEMBALL 0x0044aa20
unsigned int CFixedTimeAnim::GetFrameNo()
{
	if (m_finished != 0) {
		return m_lastFrame;
	}
	unsigned int elapsed = m_currentTime - m_startTime;
	if (m_duration <= elapsed) {
		unsigned int frame = m_frames - 1;
		m_finished = true;
		m_lastFrame = frame;
		return frame;
	}
	unsigned int frame = ((elapsed % m_duration) * m_frames) / m_duration;
	if (m_direction != ANIMATION_DIRECTION_FORWARD) {
		frame = (m_frames - frame) - 1;
	}
	return frame;
}

// FUNCTION: LEMBALL 0x0044aa70
void CFixedTimeAnim::StartAnim(unsigned int p_unused)
{
	m_finished = false;
	m_startTime = m_currentTime;
}
