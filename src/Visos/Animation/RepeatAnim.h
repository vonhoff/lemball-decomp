#ifndef LEMBALL_VISOS_ANIMATION_REPEATANIM_H
#define LEMBALL_VISOS_ANIMATION_REPEATANIM_H

#include "../../Common.h"
#include "TimedAnim.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00496ce8
class RepeatAnim : public TimedAnim {
public:
	RepeatAnim(unsigned int p_frames = 0, int p_direction = 1) : TimedAnim(p_frames, p_direction) {}
	virtual void StartAnim(unsigned long p_animTime); // vtable+0x04
};

#endif
