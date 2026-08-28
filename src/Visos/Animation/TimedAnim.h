#ifndef LEMBALL_VISOS_ANIMATION_TIMEDANIM_H
#define LEMBALL_VISOS_ANIMATION_TIMEDANIM_H

#include "../../Common.h"
#include "Frames.h" // complete type

// SIZE 0x1c
class TimedAnim : public Frames {
public:
	virtual unsigned int GetFrameNo(); // vtable+0x00
	void SetAnimDirection(int p_direction);
	void SetAnimTime(unsigned long p_animTime);
	void SetStartTime(unsigned long p_startTime);

	friend class BaseFrontendDrawer;
	friend class CdLoadAnim;
	friend class GunController;
	friend class PlayThruAnim;

private:
	unsigned int m_animTime;  // 0x10
	unsigned int m_fixedTime; // 0x14
	int m_direction;          // 0x18
};

#endif
