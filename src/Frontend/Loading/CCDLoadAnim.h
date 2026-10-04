#ifndef LEMBALL_FRONTEND_RESOURCES_CCDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CCDLOADANIM_H

#include "Engine/Animation/CAnimsManager.h"
#include "Engine/Animation/CRepeatAnim.h"
#include "Engine/Graphics/Primitives/CBigBitmap.h"
#include "Engine/Graphics/Primitives/CBitmap.h"
#include "Engine/Graphics/Primitives/CCopyToBackBuff.h"
#include "Engine/Graphics/Primitives/CDrawingMark.h"
#include "Engine/Graphics/Primitives/CLine.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "CCdLoadAnimDraw.h"
#include "CCdLoadAnimProgress.h"

class CGDI;
class CMain2DDisplay;
class CResBITMAP;
struct CVSPoint;
// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90 CAnimsManager
// VTABLE: LEMBALL 0x00497c8c CCdLoadAnimProgress
// VTABLE: LEMBALL 0x00497c88 CCdLoadAnimDraw
class CCDLoadAnim : public CAnimsManager, public CCdLoadAnimProgress, public CCdLoadAnimDraw {
public:
	CCDLoadAnim(CGDI* p_gdi, CMain2DDisplay* p_display);
	void InitialiseScreen();
	virtual void Draw();
	virtual void Draw(short p_progress);
	~CCDLoadAnim();

	friend class CFrontendResourceLoader;

	CVSPoint* m_points;               // 0x78
	CMain2DDisplay* m_display;        // 0x7c
	CGDI* m_gdi;                      // 0x80
	unsigned int m_animResourceId;    // 0x84
	CVSPoint m_centre;                // 0x88
	unsigned int m_initialDraw;       // 0x8c
	short m_progress;                 // 0x90
	CResBITMAP* m_backgroundBitmap;   // 0x94
	CResBITMAP* m_foregroundBitmap;   // 0x98
	CBigBitmap m_bitmapRes[1];        // 0x9c
	CBitmap m_fgBlit[1];              // 0xc0
	CRepeatAnim* m_repeatAnim;        // 0xdc
	CSolidRect m_line[1];             // 0xe0
	CLine m_needle0[1];               // 0xf0
	CLine m_needle1[1];               // 0x100
	CLine m_needle2[1];               // 0x110
	CDrawingMark m_mark;              // 0x120
	CCopyToBackBuff m_clearBitmap[1]; // 0x124
};

extern unsigned int g_dwCdLoadAnimCompactPoints[5];
extern unsigned int g_dwCdLoadAnimFullPoints[5];

#endif
