#ifndef LEMBALL_VISOS_ANIMATION_CREPEATANIM_H
#define LEMBALL_VISOS_ANIMATION_CREPEATANIM_H

#include "CTimedAnim.h"

// SIZE 0x1c
// VTABLE: LEMBALL 0x00496ce8
class CRepeatAnim : public CTimedAnim {
public:
	CRepeatAnim(unsigned int p_frames = 0, int p_direction = 1) : CTimedAnim(p_frames, p_direction) {}
	virtual void StartAnim(unsigned long p_animTime); // vtable+0x04
};

#endif
