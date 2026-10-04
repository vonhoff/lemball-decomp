#ifndef LEMBALL_VISOS_GRAPHICS_CRECEIVEWINDOWSTATE_H
#define LEMBALL_VISOS_GRAPHICS_CRECEIVEWINDOWSTATE_H

// SIZE 0x08
// VTABLE: LEMBALL 0x00496e60
class CReceiveWindowState {
public:
	virtual void SetOptionSelection(int p_selection); // vtable+0x00
	virtual bool GetPauser();                         // vtable+0x04

	friend class C2D;

private:
	int m_optionSelection; // 0x04
};

#endif
