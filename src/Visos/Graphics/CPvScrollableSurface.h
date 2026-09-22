#ifndef LEMBALL_VISOS_GRAPHICS_CPVSCROLLABLESURFACE_H
#define LEMBALL_VISOS_GRAPHICS_CPVSCROLLABLESURFACE_H

#include "CPvGdiBitmap.h" // complete type
#include "CPvSurface.h"   // complete type

// VTABLE: LEMBALL 0x00499d88 CPvSurface
// VTABLE: LEMBALL 0x00499dd4 CPvGdiBitmap
class CPvScrollableSurface : public CPvGdiBitmap, public virtual CPvSurface {
public:
	virtual int GetWorldWidth();             // vtable+0x48
	virtual void SetWorldWidth(int p_width); // vtable+0x44
};

#endif
