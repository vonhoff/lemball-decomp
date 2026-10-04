#ifndef LEMBALL_VISOS_ANIMATION_CSTATICANIM_H
#define LEMBALL_VISOS_ANIMATION_CSTATICANIM_H

#include "CAnimFrameBASE.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496ce4
class CStaticAnim : public CAnimFrameBASE {
public:
	CStaticAnim() : CAnimFrameBASE(1) { m_frameState = 0; }
	virtual unsigned int GetFrameNo(); // vtable+0x00
};

#endif
