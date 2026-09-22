#ifndef LEMBALL_VISOS_GRAPHICS_CPVSCROLLABLESURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVSCROLLABLESURFACE_H

#include "CPVGDIBitmap.h" // complete type
#include "CPVSurface.h"   // complete type

// VTABLE: LEMBALL 0x00499d88 CPVSurface
// VTABLE: LEMBALL 0x00499dd4 CPVGDIBitmap
class CPVScrollableSurface : public CPVGDIBitmap, public virtual CPVSurface {
public:
	virtual int GetWorldWidth();             // vtable+0x48
	virtual void SetWorldWidth(int p_width); // vtable+0x44
};

#endif
