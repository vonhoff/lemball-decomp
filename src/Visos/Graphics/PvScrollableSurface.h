#ifndef LEMBALL_VISOS_GRAPHICS_PVSCROLLABLESURFACE_H
#define LEMBALL_VISOS_GRAPHICS_PVSCROLLABLESURFACE_H

#include "PvGdiBitmap.h" // complete type
#include "PvSurface.h"   // complete type

class PvScrollableSurface : public PvGdiBitmap, public virtual PvSurface {
public:
	virtual int GetWorldWidth();             // vtable+0x48
	virtual void SetWorldWidth(int p_width); // vtable+0x44
};

#endif
