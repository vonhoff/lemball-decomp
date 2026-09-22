#ifndef LEMBALL_FRONTEND_RESOURCES_CCDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CCDLOADANIM_H

#include "../../Visos/Animation/CAnimsManager.h" // complete type
#include "../../Visos/Animation/CRepeatAnim.h"   // complete type
#include "../../Visos/Graphics/BitmapRes.h"      // complete type
#include "../../Visos/Graphics/BitmapResBase.h"  // complete type
#include "../../Visos/Graphics/CBitmap.h"        // complete type
#include "../../Visos/Graphics/CClipRect.h"
#include "../../Visos/Graphics/CDrawingMark.h" // complete type
#include "../../Visos/Graphics/CLine.h"        // complete type
#include "CCdLoadAnimDraw.h"
#include "CCdLoadAnimProgress.h"

class CGdi;
class CMain2DDisplay;
class CResBitmap;
struct CVsPoint;
// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90 CAnimsManager
// VTABLE: LEMBALL 0x00497c8c CCdLoadAnimProgress
// VTABLE: LEMBALL 0x00497c88 CCdLoadAnimDraw
class CCdLoadAnim : public CAnimsManager, public CCdLoadAnimProgress, public CCdLoadAnimDraw {
public:
	CCdLoadAnim(CGdi* p_arg0, CMain2DDisplay* p_arg1);
	void InitialiseScreen();
	virtual void Draw();
	virtual void Draw(short p_progress);
	~CCdLoadAnim();

	friend class CFrontendResourceLoader;

	CVsPoint* m_points;             // 0x78
	CMain2DDisplay* m_display;      // 0x7c
	CGdi* m_gdi;                    // 0x80
	unsigned int m_animResourceId;  // 0x84
	short m_centerX;                // 0x88
	short m_centerY;                // 0x8a
	unsigned int m_initialDraw;     // 0x8c
	short m_progress;               // 0x90
	CResBitmap* m_backgroundBitmap; // 0x94
	CResBitmap* m_foregroundBitmap; // 0x98
	BitmapRes m_bitmapRes[1];       // 0x9c
	BitmapResBase m_fgBlit[1];      // 0xc0
	CRepeatAnim* m_repeatAnim;      // 0xdc
	CLine m_line[1];                // 0xe0
	CClipRect m_needle0[1];         // 0xf0
	CClipRect m_needle1[1];         // 0x100
	CClipRect m_needle2[1];         // 0x110
	CDrawingMark m_mark;            // 0x120
	CBitmap m_clearBitmap[1];       // 0x124
};

extern unsigned int g_dwCdLoadAnimCompactPoints[5];
extern unsigned int g_dwCdLoadAnimFullPoints[5];

#endif
