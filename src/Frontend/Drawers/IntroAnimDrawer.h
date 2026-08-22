#ifndef LEMBALL_FRONTEND_DRAWERS_INTROANIMDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_INTROANIMDRAWER_H

#include "../../Common.h"
#include "../../Visos/Graphics/SolidRect.h" // complete type
#include "../Windows/IntroAnimAnimWindow.h" // complete type

// SIZE 0x484
// VTABLE: LEMBALL 0x00497a68
class IntroAnimDrawer {
public:
	IntroAnimDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2, unsigned char p_arg3);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~IntroAnimDrawer();                       // vtable+0x00
	void DestroyDrawer();
	void EndPhase();

private:
	unsigned int m_variant;           // 0x398
	int m_nextFlow;                   // 0x39c
	SolidRect m_backgroundRect;       // 0x3a0
	int m_startCountdown;             // 0x3b4
	unsigned int m_started;           // 0x3b8
	IntroAnimAnimWindow m_animWindow; // 0x3bc
};

#endif
