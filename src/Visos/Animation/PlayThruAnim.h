#ifndef LEMBALL_VISOS_ANIMATION_PLAYTHRUANIM_H
#define LEMBALL_VISOS_ANIMATION_PLAYTHRUANIM_H

#include "../../Common.h"
#include "TimedAnim.h" // complete type

// SIZE 0x1c
// VTABLE: LEMBALL 0x00496d20
class PlayThruAnim : public TimedAnim {
public:
	PlayThruAnim(unsigned int p_frames = 0, int p_direction = 0) : TimedAnim(p_frames, p_direction) {}
	virtual unsigned int GetFrameNo();                // vtable+0x00
	virtual void StartAnim(unsigned long p_animTime); // vtable+0x04
};

#endif
