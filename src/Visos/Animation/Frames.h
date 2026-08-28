#ifndef LEMBALL_VISOS_ANIMATION_FRAMES_H
#define LEMBALL_VISOS_ANIMATION_FRAMES_H

#include "../../Common.h"

// SIZE 0x10
class Frames {
public:
	virtual unsigned int GetFrameNo(); // vtable+0x00

	friend class BaseFrontendDrawer;
	friend class GunController;
	friend class AnimsManager;
	friend class CdLoadAnim;

protected:
	unsigned int m_frames;     // 0x04
	undefined4 m_reserved08;   // 0x08
	unsigned int m_frameState; // 0x0c
};

#endif
