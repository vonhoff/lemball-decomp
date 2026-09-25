#ifndef LEMBALL_FRONTEND_WINDOWS_CHILITEWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_CHILITEWINDOW_H

#include "../../Visos/Graphics/CGWnd.h"

// SIZE 0x90
// VTABLE: LEMBALL 0x00498160
class CHiliteWindow : public CGWnd {
public:
	CHiliteWindow();
	virtual void OnCreate(); // vtable+0x3c
};

extern unsigned int g_dwHiliteAnimationId;
#endif
