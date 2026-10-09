#ifndef LEMBALL_VISOS_GRAPHICS_CRECEIVEWINDOWSTATE_H
#define LEMBALL_VISOS_GRAPHICS_CRECEIVEWINDOWSTATE_H

enum eOptionSelections {
	PAUSE_OPTION_NONE = 0,
	PAUSE_OPTION_RESUME = 2,
	PAUSE_OPTION_RESTART = 3,
	PAUSE_OPTION_QUIT = 4
};

// SIZE 0x08
// VTABLE: LEMBALL 0x00496e60
class CReceiveWindowState {
public:
	virtual void SetOptionSelection(eOptionSelections p_selection); // vtable+0x00
	virtual bool GetPauser();                                       // vtable+0x04

	friend class C2D;

private:
	eOptionSelections m_optionSelection; // 0x04
};

#endif
