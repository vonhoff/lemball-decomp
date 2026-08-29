#ifndef LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_SUCCFAILDRAWER_H

#include "../Base/BaseFrontendDrawer.h"
#include "../../Visos/Foundation/VsPoint.h" // complete type
#include "../Windows/SuccFailAnimWindow.h"  // complete type
#include "SuccFailDrawerPrims.h"            // complete type

// SIZE 0x60c
// VTABLE: LEMBALL 0x00498358
class SuccFailDrawer : public BaseFrontendDrawer {
public:
	SuccFailDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2, unsigned char p_arg3);
	bool ConfirmedAction(int p_action);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~SuccFailDrawer();                        // vtable+0x00
	void CalculateText();
	void DestroyDrawer();
	void DrawText();
	void Go();
	void Return();

private:
	SuccFailDrawerPrims m_primitives; // 0x398
	char m_message[256];              // 0x3e0
	char* m_firstLine;                // 0x4e0
	char* m_secondLine;               // 0x4e4
	VsPoint m_firstLinePos;           // 0x4e8
	VsPoint m_secondLinePos;          // 0x4ec
	VsPoint m_passwordLabelPos;       // 0x4f0
	VsPoint m_passwordPos;            // 0x4f4
	char* m_password;                 // 0x4f8
	ResBitmap* m_primaryBitmap;       // 0x4fc
	ResBitmap* m_secondaryBitmap;     // 0x500
	unsigned int m_primaryBitmapId;   // 0x504
	unsigned int m_backgroundId;      // 0x508
	unsigned int m_secondaryBitmapId; // 0x50c
	void* m_layout;                   // 0x510
	unsigned int m_buttonBinding;     // 0x514
	unsigned int m_variant;           // 0x518
	unsigned int m_soundStarted;      // 0x51c
	SuccFailAnimWindow m_animWindow;  // 0x520
	unsigned int m_animStartDeadline; // 0x5f8
	unsigned int m_animStarted;       // 0x5fc
	unsigned int m_soundStartTime;    // 0x600
	unsigned int m_animationsEnabled; // 0x604
	unsigned int m_soundStopped;      // 0x608
};

// SYNTHETIC: LEMBALL 0x004510e0
// SuccFailDrawer::`scalar deleting destructor'


#endif

