#ifndef LEMBALL_VISOS_GRAPHICS_DRAWER_H
#define LEMBALL_VISOS_GRAPHICS_DRAWER_H

#include "../../Common.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x00496d48
class Drawer {
public:
	virtual void DestroyDrawer();              // vtable+0x18
	virtual void Initialise();                 // vtable+0x24
	virtual void OnDriverChange();             // vtable+0x30
	virtual void OnMove(const VsRect& p_rect); // vtable+0x14
	virtual void OnSize(const VsRect& p_rect); // vtable+0x10
	virtual void OnZoom(const VsRect& p_rect); // vtable+0x0c
	virtual void Process();                    // vtable+0x1c
	virtual void RefreshView();                // vtable+0x34
	virtual void ShutDown();                   // vtable+0x04
	virtual ~Drawer();                         // vtable+0x00
};

#endif
