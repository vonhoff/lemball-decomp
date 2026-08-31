#ifndef LEMBALL_VISOS_GRAPHICS_RECEIVEWINDOWSTATE_H
#define LEMBALL_VISOS_GRAPHICS_RECEIVEWINDOWSTATE_H

#include "../../Common.h"

// SIZE 0x08
// VTABLE: LEMBALL 0x00496e60
class ReceiveWindowState {
public:
	virtual void SetOptionSelection(int p_selection); // vtable+0x00
	virtual bool GetPauser();                         // vtable+0x04

private:
	int m_optionSelection; // 0x04
};

#endif
