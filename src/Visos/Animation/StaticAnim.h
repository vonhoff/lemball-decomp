#ifndef LEMBALL_SCAFFOLD_VISOS_ANIMATION_STATICANIM_H
#define LEMBALL_SCAFFOLD_VISOS_ANIMATION_STATICANIM_H

#include "../../Common.h"
#include "Frames.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496ce4
class StaticAnim : public Frames {
public:
	virtual unsigned int GetFrameNo(); // vtable+0x00
};

#endif
