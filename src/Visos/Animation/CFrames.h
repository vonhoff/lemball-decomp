#ifndef LEMBALL_VISOS_ANIMATION_CFRAMES_H
#define LEMBALL_VISOS_ANIMATION_CFRAMES_H

#include "CAnimFrameBASE.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d04
class CFrames : public CAnimFrameBASE {
public:
	CFrames(unsigned int p_frames = 0) : CAnimFrameBASE(p_frames) { m_frameState = 0; }
	virtual unsigned int GetFrameNo(); // vtable+0x00
};

#endif
