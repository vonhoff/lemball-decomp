#ifndef LEMBALL_FRONTEND_WINDOWS_HILITEWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_HILITEWINDOW_H

#include "../../Common.h"
#include "../../Visos/Graphics/GWnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00498160
class HiliteWindow : public GWnd {
public:
	HiliteWindow();
	virtual void Dummy3c(); // vtable+0x3c
};

extern unsigned int g_dwHiliteAnimationId;
#endif
