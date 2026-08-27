#ifndef LEMBALL_FRONTEND_DRAWERS_MAINOPTIONS2DRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_MAINOPTIONS2DRAWER_H

#include "../../Common.h"

// SIZE 0x3ac
// VTABLE: LEMBALL 0x00497b70
class MainOptions2Drawer {
public:
	MainOptions2Drawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void Load();                              // vtable+0x40
	virtual void UnLoad();                            // vtable+0x44
	virtual ~MainOptions2Drawer();                    // vtable+0x00

private:
	GunController* m_gunController;   // 0x334
	int m_nextFlow;                   // 0x374
	unsigned int m_flowPending;       // 0x378
	unsigned int m_disableZoom;       // 0x398
	unsigned int m_navigationControl; // 0x39c
	unsigned int m_transitionState;   // 0x3a0
	unsigned int m_disableAnimations; // 0x3a4
	void* m_buttonLayout;             // 0x3a8
};

// SYNTHETIC: LEMBALL 0x00449170
// MainOptions2Drawer::`scalar deleting destructor'


#endif

