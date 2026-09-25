#ifndef LEMBALL_FRONTEND_SUPPORT_CABOUTSCREEN_H
#define LEMBALL_FRONTEND_SUPPORT_CABOUTSCREEN_H

#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/CVsSize.h"
#include "../../Visos/Graphics/CBitmapRes.h"
#include "../../Visos/Graphics/CDrawer.h"
#include "../../Visos/Graphics/CDrawingMark.h"
#include "../../Visos/Graphics/CLine.h"
#include "../../Visos/Graphics/CSolidRect.h"

class CGDI;
class CMain2DDisplay;
class CResBITMAP;
class CResSTRING;
class CTextManager;
// SIZE 0x9c
// VTABLE: LEMBALL 0x00497cb0 CDrawer
// VTABLE: LEMBALL 0x00497ca0 CBaseQueueHandler
class CAboutScreen : public CDrawer, public CBaseQueueHandler {
public:
	CAboutScreen(CMain2DDisplay* p_display, CGDI* p_gdi, const CVsRect& p_rect);
	virtual ~CAboutScreen();                    // vtable+0x00
	virtual void Draw(const CVsRect& p_rect);   // vtable+0x08
	virtual void OnSize(const CVsRect& p_rect); // vtable+0x10
	virtual void ResetPrimitives();             // vtable+0x20
	virtual int GetReturnState();               // vtable+0x28
	virtual bool QuitYet();                     // vtable+0x2c
	virtual void OnDriverChange();              // vtable+0x30
	virtual int ProcessMsg(Message* p_message);
	void DrawChangedRegion();
	void DrawRegistrationText();

	friend class CMain2DDisplay;

private:
	unsigned int m_returnState;     // 0x14
	CVsSize m_size;                 // 0x18
	CMain2DDisplay* m_display;      // 0x1c
	CGDI* m_gdi;                    // 0x20
	CLine m_line;                   // 0x24
	CDrawingMark m_drawingMark;     // 0x34
	unsigned int m_complete;        // 0x38
	CBitmapRes m_bitmap;            // 0x3c
	CResBITMAP* m_backgroundBitmap; // 0x60
	CSolidRect m_rects[2];          // 0x64
	CResSTRING* m_aboutString;      // 0x84
	char* m_aboutText;              // 0x88
	class CPVGWnd* m_textWindow;    // 0x8c
	CTextManager* m_textManager;    // 0x90
	unsigned int m_startTime;       // 0x94
	unsigned int m_endTime;         // 0x98
};

// SYNTHETIC: LEMBALL 0x0044c0d0
// CAboutScreen::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0044c1d0
// CAboutScreen::`vector deleting destructor'

#endif
