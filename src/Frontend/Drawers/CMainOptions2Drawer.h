#ifndef LEMBALL_FRONTEND_DRAWERS_CMAINOPTIONS2DRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_CMAINOPTIONS2DRAWER_H

#include "../Base/CBaseFrontendDrawer.h" // complete type

class CGdi;
class CMain2DDisplay;
class CVsRect;
// SIZE 0x3ac
// VTABLE: LEMBALL 0x00497b70 CDrawer
// VTABLE: LEMBALL 0x00497b60 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497b5c CAnimsManager
class CMainOptions2Drawer : public CBaseFrontendDrawer {
public:
	CMainOptions2Drawer(CMain2DDisplay* p_arg0, CGdi* p_arg1, const CVsRect& p_arg2);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void Load();                              // vtable+0x40
	virtual void UnLoad();                            // vtable+0x44
	virtual ~CMainOptions2Drawer();                   // vtable+0x00

private:
	unsigned int m_disableZoom;       // 0x398
	unsigned int m_transitionPending; // 0x39c
	unsigned int m_transitionState;   // 0x3a0
	unsigned int m_disableAnimations; // 0x3a4
	int* m_buttonLayout;              // 0x3a8
};

// SYNTHETIC: LEMBALL 0x00449170
// CMainOptions2Drawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004491a0
// CMainOptions2Drawer::`vector deleting destructor'

#endif
