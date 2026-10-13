#ifndef LEMBALL_ENGINE_ANIMATION_CFIXEDTIMEANIM_H
#define LEMBALL_ENGINE_ANIMATION_CFIXEDTIMEANIM_H

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
	bool m_finished;            // 0x1c
};

#endif
