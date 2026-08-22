#ifndef LEMBALL_SCAFFOLD_VISOS_ANIMATION_FRAMES_H
#define LEMBALL_SCAFFOLD_VISOS_ANIMATION_FRAMES_H

#include "../../Common.h"

// SIZE 0x10
class Frames {
public:
	virtual unsigned int GetFrameNo(); // vtable+0x00

private:
	unsigned int m_frames;     // 0x04
	undefined4 m_reserved08;   // 0x08
	unsigned int m_frameState; // 0x0c
};

#endif
