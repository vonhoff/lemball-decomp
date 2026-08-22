#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_DEPRESSEDBUTTON_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_DEPRESSEDBUTTON_H

#include "../../Common.h"
#include "PvButton.h" // complete type

// SIZE 0x10c
class DepressedButton : public PvButton {
public:
	virtual unsigned int OnPaint(const VsRect& p_rect); // vtable+0xa8
	virtual void DrawButton();                          // vtable+0xb8
	virtual void OnEnterButton();                       // vtable+0xc8
	virtual void OnExitButton();                        // vtable+0xcc
	virtual void OnPressed(int p_flags);                // vtable+0xc4
	virtual void OnReleased(int p_flags);               // vtable+0xc0

private:
	unsigned int m_depressed;          // 0x104
	unsigned int m_lastDrawnDepressed; // 0x108
};

#endif
