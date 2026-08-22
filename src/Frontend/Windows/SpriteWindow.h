#ifndef LEMBALL_SCAFFOLD_FRONTEND_WINDOWS_SPRITEWINDOW_H
#define LEMBALL_SCAFFOLD_FRONTEND_WINDOWS_SPRITEWINDOW_H

#include "../../Common.h"
#include "../../Visos/Graphics/PvGWnd.h" // complete type

// SIZE 0x90
// VTABLE: LEMBALL 0x00497e30
class SpriteWindow : public PvGWnd {
public:
	SpriteWindow();
	virtual void OnCreate(); // vtable+0x3c
};

#endif
