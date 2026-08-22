#ifndef LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H
#define LEMBALL_FRONTEND_SUPPORT_TARGETABOUTSCREEN_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Graphics/Bitmap.h"             // complete type
#include "../../Visos/Graphics/DrawingMark.h"        // complete type
#include "../../Visos/Graphics/Line.h"               // complete type
#include "../../Visos/Graphics/SolidRect.h"          // complete type

// SIZE 0x9c
// VTABLE: LEMBALL 0x00497cb0 primary process interface
// VTABLE: LEMBALL 0x00497ca0 secondary BaseQueueHandler at +0x04
class TargetAboutScreen {
public:
	// No functions mapped yet.

private:
	BaseQueueHandler m_queueHandler; // 0x04
	unsigned int m_returnState;      // 0x14
	short m_width;                   // 0x18
	short m_height;                  // 0x1a
	Main2DDisplay* m_display;        // 0x1c
	Gdi* m_gdi;                      // 0x20
	Line m_line;                     // 0x24
	DrawingMark m_drawingMark;       // 0x34
	unsigned int m_complete;         // 0x38
	Bitmap m_bitmap;                 // 0x3c
	unsigned char m_bitmapState[20]; // 0x4c
	ResBitmap* m_backgroundBitmap;   // 0x60
	SolidRect m_rects[2];            // 0x64
	ResString* m_aboutString;        // 0x84
	char* m_aboutText;               // 0x88
	void* m_textWindow;              // 0x8c
	TextManager* m_textManager;      // 0x90
	unsigned int m_startTime;        // 0x94
	unsigned int m_endTime;          // 0x98
};

#endif
