#ifndef LEMBALL_VISOS_GRAPHICS_PVSCROLLABLESURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVSCROLLABLESURFACE_H

#include "../../Common.h"

class PvScrollableSurface {
public:
	virtual int GetWorldWidth();             // vtable+0x48
	virtual void SetWorldWidth(int p_width); // vtable+0x44
};

#endif
