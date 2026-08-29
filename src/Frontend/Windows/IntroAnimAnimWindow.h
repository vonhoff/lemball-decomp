#ifndef LEMBALL_FRONTEND_WINDOWS_INTROANIMANIMWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_INTROANIMANIMWINDOW_H

#include "../../Common.h"
#include "../../Visos/Graphics/AnimWnd.h" // complete type

// SIZE 0xc8
// VTABLE: LEMBALL 0x00497960
class IntroAnimAnimWindow : public AnimWnd {
public:
	virtual unsigned int GetStyle(); // vtable+0x64
	virtual void OnStop();           // vtable+0xc4
	void Initialise(IntroAnimDrawer* p_owner, Main2DDisplay* p_display, unsigned int p_variant);
	void SetAnim();

private:
	IntroAnimDrawer* m_owner; // 0xbc
	Main2DDisplay* m_display; // 0xc0
	unsigned int m_variant;   // 0xc4
};

#endif
