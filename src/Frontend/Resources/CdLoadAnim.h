#ifndef LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H

#include "../../Visos/Animation/AnimsManager.h" // complete type
#include "../../Visos/Animation/RepeatAnim.h"   // complete type
#include "../../Visos/Graphics/Bitmap.h"        // complete type
#include "../../Visos/Graphics/BitmapRes.h"     // complete type
#include "../../Visos/Graphics/BitmapResBase.h" // complete type
#include "../../Visos/Graphics/ClipRect.h"
#include "../../Visos/Graphics/DrawingMark.h" // complete type
#include "../../Visos/Graphics/Line.h"        // complete type
#include "CdLoadAnimDraw.h"
#include "CdLoadAnimProgress.h"

class Gdi;
class Main2DDisplay;
class ResBitmap;
struct VsPoint;
// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90 AnimsManager
// VTABLE: LEMBALL 0x00497c8c CdLoadAnimProgress
// VTABLE: LEMBALL 0x00497c88 CdLoadAnimDraw
class CdLoadAnim : public AnimsManager, public CdLoadAnimProgress, public CdLoadAnimDraw {
public:
	CdLoadAnim(Gdi* p_arg0, Main2DDisplay* p_arg1);
	void InitialiseScreen();
	virtual void Draw();
	virtual void Draw(short p_progress);
	~CdLoadAnim();

	friend class FrontendResourceLoader;

	VsPoint* m_points;             // 0x78
	Main2DDisplay* m_display;      // 0x7c
	Gdi* m_gdi;                    // 0x80
	unsigned int m_animResourceId; // 0x84
	short m_centerX;               // 0x88
	short m_centerY;               // 0x8a
	unsigned int m_initialDraw;    // 0x8c
	short m_progress;              // 0x90
	ResBitmap* m_backgroundBitmap; // 0x94
	ResBitmap* m_foregroundBitmap; // 0x98
	BitmapRes m_bitmapRes[1];      // 0x9c
	BitmapResBase m_fgBlit[1];     // 0xc0
	RepeatAnim* m_repeatAnim;      // 0xdc
	Line m_line[1];                // 0xe0
	ClipRect m_needle0[1];         // 0xf0
	ClipRect m_needle1[1];         // 0x100
	ClipRect m_needle2[1];         // 0x110
	DrawingMark m_mark;            // 0x120
	Bitmap m_clearBitmap[1];       // 0x124
};

extern unsigned int g_dwCdLoadAnimCompactPoints[5];
extern unsigned int g_dwCdLoadAnimFullPoints[5];

#endif
