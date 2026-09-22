#ifndef LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_CSUCCFAILDRAWER_H

#include "../../Visos/Foundation/CVsPoint.h" // complete type
#include "../../Visos/Graphics/CBitmapRes.h" // complete type
#include "../Base/CBaseFrontendDrawer.h"
#include "../Support/CoordPair.h"        // complete type
#include "../Windows/CSuccFailAnimWnd.h" // complete type
#include "Frontend/Support/CUserActionMessage.h"
#include "SuccFailDrawerFieldViews.h"

class CGDI;
class CMain2DDisplay;
class CResBITMAP;
class CVsRect;
// SIZE 0x68
struct SuccFailLayout {
	CoordPair m_returnButton;          // 0x00
	CoordPair m_goButton;              // 0x08
	CoordPair m_backgroundPosition;    // 0x10
	CoordPair m_primaryPosition;       // 0x18
	CoordPair m_successAnimOffset;     // 0x20
	CoordPair m_failurePosition;       // 0x28
	CoordPair m_failureAnimOffset;     // 0x30
	CoordPair m_frameStart;            // 0x38
	CoordPair m_frameEnd;              // 0x40
	CoordPair m_messagePosition;       // 0x48
	CoordPair m_secondaryPosition;     // 0x50
	CoordPair m_animWindowEnd;         // 0x58
	CoordPair m_passwordLabelPosition; // 0x60
};

// SIZE 0x60c
// VTABLE: LEMBALL 0x00498358 CDrawer
// VTABLE: LEMBALL 0x00498348 CBaseQueueHandler
// VTABLE: LEMBALL 0x00498340 CAnimsManager
class CSuccFailDrawer : public CBaseFrontendDrawer {
public:
	CSuccFailDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVsRect& p_rect, unsigned int p_success);
	bool ConfirmedAction(eUserActions p_action);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~CSuccFailDrawer();                       // vtable+0x00
	void CalculateText();
	void DestroyDrawer();
	void DrawText();
	void Go();
	void Return();

private:
	// SIZE 0x48
	struct tagPRIMS {
		tagPRIMS();
		~tagPRIMS();

		CBitmapRes m_primary;   // 0x00
		CBitmapRes m_secondary; // 0x24
	};

	tagPRIMS m_primitives[1];         // 0x398
	char m_message[256];              // 0x3e0
	char* m_firstLine;                // 0x4e0
	char* m_secondLine;               // 0x4e4
	CVsPoint m_firstLinePos;          // 0x4e8
	CVsPoint m_secondLinePos;         // 0x4ec
	CVsPoint m_passwordLabelPos;      // 0x4f0
	CVsPoint m_passwordPos;           // 0x4f4
	char* m_password;                 // 0x4f8
	CResBITMAP* m_primaryBitmap;      // 0x4fc
	CResBITMAP* m_secondaryBitmap;    // 0x500
	unsigned int m_primaryBitmapId;   // 0x504
	unsigned int m_backgroundId;      // 0x508
	unsigned int m_secondaryBitmapId; // 0x50c
	SuccFailLayout* m_layout;         // 0x510
	unsigned int m_buttonBinding;     // 0x514
	unsigned int m_success;           // 0x518
	unsigned int m_soundStarted;      // 0x51c
	CSuccFailAnimWnd m_animWindow;    // 0x520
	unsigned int m_animStartDeadline; // 0x5f8
	unsigned int m_animStarted;       // 0x5fc
	unsigned int m_soundStartTime;    // 0x600
	unsigned int m_animationsEnabled; // 0x604
	unsigned int m_soundStopped;      // 0x608
};

// SYNTHETIC: LEMBALL 0x004510e0
// CSuccFailDrawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00451200
// CSuccFailDrawer::`vector deleting destructor'

#endif
