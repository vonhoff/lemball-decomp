#ifndef LEMBALL_FRONTEND_WINDOWS_CINTROANIMANIMWINDOW_H
#define LEMBALL_FRONTEND_WINDOWS_CINTROANIMANIMWINDOW_H

#include "../../Visos/Graphics/CAnimWnd.h"

class CIntroAnimDrawer;
class CMain2DDisplay;
// SIZE 0xc8
// VTABLE: LEMBALL 0x00497960
class CIntroAnimAnimWindow : public CAnimWnd {
public:
	virtual unsigned int GetStyle(); // vtable+0x64
	virtual void OnStop();           // vtable+0xc4
	void Initialise(CIntroAnimDrawer* p_owner, CMain2DDisplay* p_display, unsigned int p_completionSequence);
	void SetAnim();

private:
	CIntroAnimDrawer* m_owner; // 0xbc
	CMain2DDisplay* m_display; // 0xc0
	unsigned int m_completionSequence; // 0xc4
};

#endif
