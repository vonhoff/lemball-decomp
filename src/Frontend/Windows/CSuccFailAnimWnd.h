#ifndef LEMBALL_FRONTEND_WINDOWS_CSUCCFAILANIMWND_H
#define LEMBALL_FRONTEND_WINDOWS_CSUCCFAILANIMWND_H

#include "../../Visos/Graphics/CAnimWnd.h" // complete type

class CSuccFailDrawer;
class CMain2DDisplay;

// SIZE 0xd8
// VTABLE: LEMBALL 0x00498250
class CSuccFailAnimWnd : public CAnimWnd {
public:
	virtual unsigned int GetStyle(); // vtable+0x64
	virtual void OnStop();           // vtable+0xc4
	void Initialise(CSuccFailDrawer* p_owner, CMain2DDisplay* p_display, int p_variant);
	void SetVariant(int p_variant);

private:
	CSuccFailDrawer* m_owner;         // 0xbc
	CMain2DDisplay* m_display;        // 0xc0
	unsigned int m_variant;           // 0xc4
	unsigned int m_primaryAnimBase;   // 0xc8
	unsigned int m_secondaryAnimBase; // 0xcc
	unsigned int m_variantIndex;      // 0xd0
	unsigned int m_state;             // 0xd4
};

#endif
