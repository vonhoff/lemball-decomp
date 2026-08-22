#ifndef LEMBALL_FRONTEND_WINDOWS_SUCCFAILANIMWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_SUCCFAILANIMWINDOW_H

#include "../../Common.h"
#include "../../Visos/Graphics/AnimWnd.h" // complete type

// SIZE 0xd8
// VTABLE: LEMBALL 0x00498250
class SuccFailAnimWindow : public AnimWnd {
public:
	// No functions mapped yet.

private:
	SuccFailDrawer* m_owner;          // 0xbc
	Main2DDisplay* m_display;         // 0xc0
	unsigned int m_variant;           // 0xc4
	unsigned int m_primaryAnimBase;   // 0xc8
	unsigned int m_secondaryAnimBase; // 0xcc
	unsigned int m_variantIndex;      // 0xd0
	unsigned int m_state;             // 0xd4
};

#endif
