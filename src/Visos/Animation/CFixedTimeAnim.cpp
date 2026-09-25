class CFixedTimeAnim {
public:
	unsigned int GetFrameNo();
	void StartAnim(unsigned int p_unused);

private:
	unsigned int m_unknown00;
	unsigned int m_frames;      // 0x04
	unsigned int m_lastFrame;   // 0x08
	unsigned int m_startTime;   // 0x0c
	unsigned int m_duration;    // 0x10
	unsigned int m_currentTime; // 0x14
	int m_direction;            // 0x18
	unsigned int m_finished;    // 0x1c
};

// FUNCTION: LEMBALL 0x0044aa20
unsigned int CFixedTimeAnim::GetFrameNo()
{
	if (m_finished != 0) {
		return m_lastFrame;
	}
	unsigned int elapsed = m_currentTime - m_startTime;
	if (m_duration <= elapsed) {
		unsigned int frame = m_frames - 1;
		m_finished = 1;
		m_lastFrame = frame;
		return frame;
	}
	unsigned int frame = ((elapsed % m_duration) * m_frames) / m_duration;
	if (m_direction != 1) {
		frame = (m_frames - frame) - 1;
	}
	return frame;
}

// FUNCTION: LEMBALL 0x0044aa70
void CFixedTimeAnim::StartAnim(unsigned int p_unused)
{
	m_finished = 0;
	m_startTime = m_currentTime;
}
