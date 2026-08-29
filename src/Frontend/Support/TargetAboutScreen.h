#ifndef LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H
#define LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/VsRect.h"           // complete type
#include "../../Visos/Graphics/BitmapRes.h"          // complete type
#include "../../Visos/Graphics/Drawer.h"             // complete type
#include "../../Visos/Graphics/DrawingMark.h"        // complete type
#include "../../Visos/Graphics/PvGWnd.h"
#include "../../Visos/Graphics/Line.h"               // complete type
#include "../../Visos/Graphics/SolidRect.h"          // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00497ca0
class TargetAboutScreenQueue : public BaseQueueHandler {
public:
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
};

// SIZE 0x9c
// VTABLE: LEMBALL 0x00497cb0
class TargetAboutScreen : public Drawer {
public:
	TargetAboutScreen(Main2DDisplay* p_display, Gdi* p_gdi, const VsRect& p_rect);
	virtual ~TargetAboutScreen();            // vtable+0x00
	virtual void Draw(const VsRect& p_rect); // vtable+0x08
	virtual void ResetPrimitives();          // vtable+0x20
	virtual int GetReturnState();            // vtable+0x28
	virtual bool QuitYet();                  // vtable+0x2c
	void DrawChangedRegion();
	void DrawRegistrationText();

	friend class Main2DDisplay;

private:
	TargetAboutScreenQueue m_queueHandler; // 0x04
	unsigned int m_returnState;            // 0x14
	short m_width;                         // 0x18
	short m_height;                        // 0x1a
	Main2DDisplay* m_display;              // 0x1c
	Gdi* m_gdi;                            // 0x20
	Line m_line;                           // 0x24
	DrawingMark m_drawingMark;             // 0x34
	unsigned int m_complete;               // 0x38
	BitmapRes m_bitmap;                    // 0x3c
	ResBitmap* m_backgroundBitmap;         // 0x60
	SolidRect m_rects[2];                  // 0x64
	ResString* m_aboutString;              // 0x84
	char* m_aboutText;                     // 0x88
	class PvGWnd* m_textWindow;            // 0x8c
	TextManager* m_textManager;            // 0x90
	unsigned int m_startTime;              // 0x94
	unsigned int m_endTime;                // 0x98
};

// SYNTHETIC: LEMBALL 0x0044c0d0
// TargetAboutScreen::`scalar deleting destructor'

#endif
