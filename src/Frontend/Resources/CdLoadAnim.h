#ifndef LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h" // complete type
#include "../../Visos/Animation/RepeatAnim.h"   // complete type
#include "../../Visos/Graphics/Bitmap.h"        // complete type
#include "../../Visos/Graphics/BitmapRes.h"     // complete type
#include "../../Visos/Graphics/ClipRect.h"      // complete type
#include "../../Visos/Graphics/DrawingMark.h"   // complete type
#include "../../Visos/Graphics/Line.h"          // complete type

// SIZE 0xc0
// VTABLE: LEMBALL 0x00497c88
class CdLoadAnimDraw {
public:
	virtual void Draw(); // vtable+0x00

	VsPoint* m_points;             // 0x04 / object+0x78
	Main2DDisplay* m_display;      // 0x08 / object+0x7c
	Gdi* m_gdi;                    // 0x0c / object+0x80
	unsigned int m_animResourceId; // 0x10 / object+0x84
	short m_centerX;               // 0x14 / object+0x88
	short m_centerY;               // 0x16 / object+0x8a
	unsigned int m_initialDraw;    // 0x18 / object+0x8c
	short m_progress;              // 0x1c / object+0x90
	ResBitmap* m_backgroundBitmap; // 0x20 / object+0x94
	ResBitmap* m_foregroundBitmap; // 0x24 / object+0x98
	BitmapRes m_bitmapRes[1];      // 0x28 / object+0x9c
	BitmapResBase m_fgBlit[1];     // 0x4c / object+0xc0
	RepeatAnim* m_repeatAnim;      // 0x68 / object+0xdc
	Line m_line[1];                // 0x6c / object+0xe0
	ClipRect m_clip0[1];           // 0x7c / object+0xf0
	ClipRect m_clip1[1];           // 0x8c / object+0x100
	ClipRect m_clip2[1];           // 0x9c / object+0x110
	DrawingMark m_mark;            // 0xac / object+0x120
	Bitmap m_clearBitmap[1];       // 0xb0 / object+0x124
};

// SIZE 0xc4
// VTABLE: LEMBALL 0x00497c8c
class CdLoadAnimProgress {
public:
	virtual void Draw(short p_progress); // vtable+0x00

	CdLoadAnimDraw m_draw; // 0x04 / object+0x74
};

// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90 AnimsManager base
class CdLoadAnim : public AnimsManager {
public:
	CdLoadAnim(Gdi* p_arg0, Main2DDisplay* p_arg1);
	void InitialiseScreen();
	~CdLoadAnim();

	friend class FrontendResourceLoader;

private:
	CdLoadAnimProgress m_progress; // 0x70
};

extern unsigned int g_dwCdLoadAnimCompactPoints[5];
extern unsigned int g_dwCdLoadAnimFullPoints[5];

#endif
