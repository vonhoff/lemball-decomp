#ifndef LEMBALL_FRONTEND_DRAWERS_CMAINOPTIONS1DRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_CMAINOPTIONS1DRAWER_H

#include "../../Visos/Foundation/CBaseProcess.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../Base/CBaseFrontendDrawer.h"

class CGDI;
class CMain2DDisplay;
class CVsRect;
// SIZE 0x3bc
// VTABLE: LEMBALL 0x00497af0 CDrawer
// VTABLE: LEMBALL 0x00497ae0 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497ad8 CAnimsManager
class CMainOptions1Drawer : public CBaseFrontendDrawer {
public:
	CMainOptions1Drawer(CMain2DDisplay* p_arg0, CGDI* p_arg1, const CVsRect& p_arg2);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~CMainOptions1Drawer();                   // vtable+0x00

private:
	unsigned int m_idleDeadline;           // 0x398
	int* m_buttonLayout;                   // 0x39c
	unsigned int m_previousModeButton;     // 0x3a0
	unsigned int m_nextModeButton;         // 0x3a4
	unsigned int m_auxButtonState0;        // 0x3a8
	unsigned int m_auxButtonState1;        // 0x3ac
	unsigned int m_toggleResolutionButton; // 0x3b0
	int m_selectedDisplayMode;             // 0x3b4
	unsigned int m_navigationButton;       // 0x3b8
};

// SYNTHETIC: LEMBALL 0x00448a70
// CMainOptions1Drawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00448aa0
// CMainOptions1Drawer::`vector deleting destructor'

#endif
