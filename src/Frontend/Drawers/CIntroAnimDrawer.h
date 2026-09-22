#ifndef LEMBALL_FRONTEND_DRAWERS_CINTROANIMDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_CINTROANIMDRAWER_H

#include "../../Visos/Graphics/CSolidRect.h" // complete type
#include "../Base/CBaseFrontendDrawer.h"     // complete type
#include "../Windows/CIntroAnimAnimWindow.h" // complete type

class CGDI;
class CMain2DDisplay;
class CVsRect;
// SIZE 0x484
// VTABLE: LEMBALL 0x00497a68 CDrawer
// VTABLE: LEMBALL 0x00497a58 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497a50 CAnimsManager
class CIntroAnimDrawer : public CBaseFrontendDrawer {
public:
	CIntroAnimDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVsRect& p_rect, unsigned int p_completionSequence);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~CIntroAnimDrawer();                      // vtable+0x00
	virtual void DestroyDrawer();                     // vtable+0x18
	void EndPhase();

private:
	unsigned int m_completionSequence; // 0x398
	int m_nextFlow;                    // 0x39c
	CSolidRect m_backgroundRect;       // 0x3a0
	unsigned int m_unk0x3b0;           // 0x3b0
	int m_startCountdown;              // 0x3b4
	unsigned int m_started;            // 0x3b8
	CIntroAnimAnimWindow m_animWindow; // 0x3bc
};

// SYNTHETIC: LEMBALL 0x004479a0
// CIntroAnimDrawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004479d0
// CIntroAnimDrawer::`vector deleting destructor'

#endif
