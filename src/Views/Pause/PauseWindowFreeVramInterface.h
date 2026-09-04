#ifndef LEMBALL_VIEWS_PAUSE_PAUSEWINDOWFREEVRAMINTERFACE_H
#define LEMBALL_VIEWS_PAUSE_PAUSEWINDOWFREEVRAMINTERFACE_H

#include "../../Common.h"

// SIZE 0x04
class PauseWindowFreeVramInterface {
public:
	virtual void FreeVram() = 0; // vtable+0x00
};

#endif
