#ifndef LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H
#define LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H

#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/CVsRect.h"           // complete type
#include "../../Visos/Foundation/CVsSize.h"           // complete type
#include "../../Visos/Graphics/BitmapRes.h"           // complete type
#include "../../Visos/Graphics/CDrawer.h"             // complete type
#include "../../Visos/Graphics/CDrawingMark.h"        // complete type
#include "../../Visos/Graphics/CLine.h"               // complete type
#include "../../Visos/Graphics/CSolidRect.h"          // complete type

class CGdi;
class CMain2DDisplay;
class CResBitmap;
class CResString;
class CTextManager;
// SIZE 0x9c
// VTABLE: LEMBALL 0x00497cb0 CDrawer
// VTABLE: LEMBALL 0x00497ca0 CBaseQueueHandler
class TargetAboutScreen : public CDrawer, public CBaseQueueHandler {
public:
	TargetAboutScreen(CMain2DDisplay* p_display, CGdi* p_gdi, const CVsRect& p_rect);
	virtual ~TargetAboutScreen();               // vtable+0x00
	virtual void Draw(const CVsRect& p_rect);   // vtable+0x08
	virtual void OnSize(const CVsRect& p_rect); // vtable+0x10
	virtual void ResetPrimitives();             // vtable+0x20
	virtual int GetReturnState();               // vtable+0x28
	virtual bool QuitYet();                     // vtable+0x2c
	virtual void OnDriverChange();              // vtable+0x30
	virtual int ProcessMsg(Message* p_message); // CBaseQueueHandler vtable+0x08
	void DrawChangedRegion();
	void DrawRegistrationText();

	friend class CMain2DDisplay;

private:
	unsigned int m_returnState;     // 0x14
	CVsSize m_size;                 // 0x18
	CMain2DDisplay* m_display;      // 0x1c
	CGdi* m_gdi;                    // 0x20
	CLine m_line;                   // 0x24
	CDrawingMark m_drawingMark;     // 0x34
	unsigned int m_complete;        // 0x38
	BitmapRes m_bitmap;             // 0x3c
	CResBitmap* m_backgroundBitmap; // 0x60
	CSolidRect m_rects[2];          // 0x64
	CResString* m_aboutString;      // 0x84
	char* m_aboutText;              // 0x88
	class CPvGWnd* m_textWindow;    // 0x8c
	CTextManager* m_textManager;    // 0x90
	unsigned int m_startTime;       // 0x94
	unsigned int m_endTime;         // 0x98
};

// SYNTHETIC: LEMBALL 0x0044c0d0
// TargetAboutScreen::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0044c1d0
// TargetAboutScreen::`vector deleting destructor'

#endif
