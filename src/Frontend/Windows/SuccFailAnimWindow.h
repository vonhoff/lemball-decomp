#ifndef LEMBALL_FRONTEND_WINDOWS_SUCCFAILANIMWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_SUCCFAILANIMWINDOW_H

#include "../../Common.h"
#include "../../Visos/Graphics/AnimWnd.h" // complete type

class SuccFailDrawer;
class Main2DDisplay;

// SIZE 0xd8
// VTABLE: LEMBALL 0x00498250
class SuccFailAnimWindow : public AnimWnd {
public:
	virtual unsigned int GetStyle(); // vtable+0x64
	virtual void OnStop();           // vtable+0xc4
	unsigned int Initialise(SuccFailDrawer* p_owner, Main2DDisplay* p_display, int p_variant);
	void SetVariant(int p_variant);

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
