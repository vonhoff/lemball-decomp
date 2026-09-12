#ifndef LEMBALL_VISOS_ANIMATION_TIMEDANIM_H
#define LEMBALL_VISOS_ANIMATION_TIMEDANIM_H

#include "../../Common.h"
#include "Frames.h" // complete type

// SIZE 0x1c
class TimedAnim : public Frames {
public:
	TimedAnim(unsigned int p_frames = 0, int p_direction = 0) : Frames(p_frames), m_direction(p_direction) {}
	virtual unsigned int GetFrameNo();                    // vtable+0x00
	virtual void StartAnim(unsigned long p_animTime) = 0; // vtable+0x04
	virtual void SetAnimTime(unsigned long p_animTime);   // vtable+0x08
	virtual void SetStartTime(unsigned long p_startTime); // vtable+0x0c
	virtual void SetAnimDirection(int p_direction);       // vtable+0x10

	friend class BaseFrontendDrawer;
	friend class CdLoadAnim;
	friend class GunController;
	friend class PlayThruAnim;
	friend class RepeatAnim;
	friend class PreviewDrawer;
	friend class LemmingAnimsManager;

private:
	unsigned int m_animTime;  // 0x10
	unsigned int m_fixedTime; // 0x14
	int m_direction;          // 0x18
};

#endif
