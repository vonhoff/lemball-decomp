#ifndef LEMBALL_VISOS_ANIMATION_CSTATICANIM_H
#define LEMBALL_VISOS_ANIMATION_CSTATICANIM_H

#include "CFrames.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496ce4
class CStaticAnim : public CFrames {
public:
	CStaticAnim() : CFrames(1) { m_frameState = 0; }
	virtual unsigned int GetFrameNo(); // vtable+0x00
};

#endif
