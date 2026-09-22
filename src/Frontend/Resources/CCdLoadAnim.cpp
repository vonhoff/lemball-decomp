#include "CCdLoadAnim.h"

#include "../../Control/Game/GameMain.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVector.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResBitmap.h"
#include "../../Visos/Resources/CResPalette.h"
#include "../../Visos/Resources/Manifest.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CRepeatAnim.h"
#include "Visos/Graphics/CBitmap.h"
#include "Visos/Graphics/CBitmapRes.h"
#include "Visos/Graphics/CBitmapResBase.h"
#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CLine.h"

class CFrames;

// FUNCTION: LEMBALL 0x0044aa80
CCdLoadAnim::CCdLoadAnim(CGdi* p_arg0, CMain2DDisplay* p_arg1)
	: CAnimsManager(p_arg0, 0x2b6, 1, 1, 0, 0), m_centerX(m_centerY = 0)
{
	unsigned int* points;
	unsigned int packed;
	int offset;
	CVsPoint* dest;
	CResPalette* palette;
	unsigned long animCount;

	m_display = p_arg1;
	m_gdi = p_arg0;
	m_points = new CVsPoint[5];
	g_pCursor->SetActive(0);
	if (g_nCompactPrimaryContextLayout != 0) {
		m_backgroundBitmap = CResBitmap::Load(RES_FRONTEND_LOADING_LORES_PICTURE);
		points = g_dwCdLoadAnimCompactPoints;
		m_foregroundBitmap = CResBitmap::Load(RES_FRONTEND_LOADING_LORES_REPLACE);
		m_animResourceId = 0xf0;
	}
	else {
		m_backgroundBitmap = CResBitmap::Load(RES_FRONTEND_LOADING_HIRES_PICTURE);
		points = g_dwCdLoadAnimFullPoints;
		m_foregroundBitmap = CResBitmap::Load(RES_FRONTEND_LOADING_HIRES_REPLACE);
		m_animResourceId = 0xf3;
	}
	LoadAnims(m_animResourceId);
	palette = CResPalette::Load(RES_FRONTEND_LOADING_LORES_PALETTE);
	if (m_display->m_lifecycleRefs == 1) {
		m_display->Clear(-1);
	}
	p_arg1->AttachPalette(RES_FRONTEND_LOADING_LORES_PALETTE);
	palette->UnLoad();
	m_centerY = (short) ((short) (m_display->m_rect.m_height - m_backgroundBitmap->m_y) / 2);
	m_centerX = (short) ((short) (m_display->m_rect.m_width - m_backgroundBitmap->m_x) / 2);
	offset = 0;
	do {
		packed = *points;
		points = points + 1;
		dest = (CVsPoint*) ((int) &m_points->m_x + offset);
		dest->m_x = (short) packed;
		dest->m_y = (short) (packed >> 0x10);
		offset = offset + 4;
	} while (offset < 0x14);
	m_progress = 0;
	m_initialDraw = 1;
	animCount = GetnAnims(m_animResourceId);
	m_repeatAnim = new CRepeatAnim(animCount, 1);
	m_repeatAnim->m_fixedTime = 0xffffffff;
	m_repeatAnim->StartAnim(animCount * 0x42);
}

// FUNCTION: LEMBALL 0x0044ad60
CCdLoadAnim::~CCdLoadAnim()
{
	delete[] m_points;
	delete m_repeatAnim;
	UnLoadAnims(m_animResourceId);
	if (m_backgroundBitmap != 0) {
		m_backgroundBitmap->UnLoad();
	}
	m_foregroundBitmap->UnLoad();
	if (m_display->m_lifecycleRefs == 1) {
		m_display->Clear(-1);
	}
}

// FUNCTION: LEMBALL 0x0044ae80
void CCdLoadAnim::InitialiseScreen()
{
	int remaining;

	remaining = 1;
	do {
		if (m_display->m_lifecycleRefs == 1) {
			m_display->Refresh(0);
		}
		remaining = remaining - 1;
	} while (remaining != 0);
	m_backgroundBitmap->UnLoad();
	m_backgroundBitmap = 0;
}

// FUNCTION: LEMBALL 0x0044aec0
void CCdLoadAnim::Draw()
{
	int angle;
	short rotX;
	short rotY;
	CChangeList* changeList;

	m_gdi->m_renderTarget->GetCurrDb();
	m_display->SetZoom(1);
	if (m_initialDraw != 0) {
		m_initialDraw = m_initialDraw - 1;
		CVsRect rect;
		rect.m_width = m_display->m_rect.m_width;
		rect.m_height = m_display->m_rect.m_height;
		rect.m_x = 0;
		rect.m_y = 0;
		m_line[0].m_x1 = rect.m_width;
		m_line[0].m_y1 = rect.m_height;
		m_line[0].m_x2 = rect.m_x;
		m_line[0].m_y2 = rect.m_y;
		m_line[0].m_color = 0;
		m_line[0].Draw(m_gdi);
		m_bitmapRes[0].m_x = m_centerX;
		m_bitmapRes[0].m_y = m_centerY;
		m_bitmapRes[0].m_resource = m_backgroundBitmap;
		m_bitmapRes[0].m_flags = 0;
		m_bitmapRes[0].m_remap = 0;
		m_bitmapRes[0].Draw(m_gdi);
		CVsPoint point(0, 0);
		m_clearBitmap[0].m_x = point.m_x;
		m_clearBitmap[0].m_y = point.m_y;
		rect.m_width = m_display->m_rect.m_width;
		rect.m_height = m_display->m_rect.m_height;
		rect.m_x = 0;
		rect.m_y = 0;
		m_clearBitmap[0].m_width = rect.m_width;
		m_clearBitmap[0].m_height = rect.m_height;
		m_clearBitmap[0].m_sourceX = rect.m_x;
		m_clearBitmap[0].m_sourceY = rect.m_y;
		m_clearBitmap[0].Draw(m_gdi);
	}
	CVsPoint point((short) (m_points->m_x + m_centerX), (short) (m_points->m_y + m_centerY));
	m_fgBlit[0].m_x = point.m_x;
	m_fgBlit[0].m_y = point.m_y;
	m_fgBlit[0].m_resource = m_foregroundBitmap;
	m_fgBlit[0].m_flags = 0;
	m_fgBlit[0].m_remap = 0;
	m_fgBlit[0].Draw(m_gdi);
	point.m_x = (short) (m_points[1].m_x + m_centerX);
	point.m_y = (short) (m_points[1].m_y + m_centerY);
	DrawAnim(point, m_animResourceId, 0, (CFrames*) m_repeatAnim, 0);
	CVsPoint origin((short) (m_points[2].m_x + m_centerX), (short) (m_points[2].m_y + m_centerY));
	CVector radius(0, 0);
	radius.SetIntegers((short) -m_points[3].m_x, 0);
	CVector thick(((int) m_points[4].m_x) << 12, ((int) m_points[4].m_y) << 12);
	CVector left = radius + thick;
	thick.m_xFixed = ((int) m_points[4].m_x) << 12;
	thick.m_yFixed = ((int) -m_points[4].m_y) << 12;
	CVector right = radius + thick;
	angle = m_progress;
	if (100 < angle) {
		angle = 100;
	}
	angle = (angle << 8) / 100;
	VsTrig* trig = g_pVSTrig;
	CFixed sine = trig->Sin(angle);
	CFixed cosine = trig->Cos(angle);
	CVector rotatedRadius = trig->Rotate(radius, sine, cosine);
	rotX = (short) (rotatedRadius.m_xFixed >> 12);
	rotY = (short) (rotatedRadius.m_yFixed >> 12);
	trig = g_pVSTrig;
	CFixed leftSine = trig->Sin(angle);
	CFixed leftCosine = trig->Sin(angle + 0x80);
	CVector rotatedLeft = trig->Rotate(left, leftSine, leftCosine);
	trig = g_pVSTrig;
	CFixed rightSine = trig->Sin(angle);
	CFixed rightCosine = trig->Sin(angle + 0x80);
	CVector rotatedRight = trig->Rotate(right, rightSine, rightCosine);
	CVsPoint tip(0, 0);
	tip.m_x = (short) (origin.m_x + rotX);
	tip.m_y = (short) (origin.m_y + rotY);
	m_needle0[0].m_left = origin.m_x;
	m_needle0[0].m_top = origin.m_y;
	m_needle0[0].m_right = tip.m_x;
	m_needle0[0].m_bottom = tip.m_y;
	m_needle0[0].m_reserved0c = 0x66;
	m_needle0[0].Draw(m_gdi);
	point.m_x = (short) ((rotatedLeft.m_xFixed >> 12) + origin.m_x);
	point.m_y = (short) ((rotatedLeft.m_yFixed >> 12) + origin.m_y);
	m_needle1[0].m_left = tip.m_x;
	m_needle1[0].m_top = tip.m_y;
	m_needle1[0].m_right = point.m_x;
	m_needle1[0].m_bottom = point.m_y;
	m_needle1[0].m_reserved0c = 0xba;
	m_needle1[0].Draw(m_gdi);
	point.m_x = (short) ((rotatedRight.m_xFixed >> 12) + origin.m_x);
	point.m_y = (short) ((rotatedRight.m_yFixed >> 12) + origin.m_y);
	m_needle2[0].m_left = tip.m_x;
	m_needle2[0].m_top = tip.m_y;
	m_needle2[0].m_right = point.m_x;
	m_needle2[0].m_bottom = point.m_y;
	m_needle2[0].m_reserved0c = 0xbf;
	m_needle2[0].Draw(m_gdi);
	ResetPrimitives();
	changeList = m_gdi->m_renderTarget->GetChangeList();
	m_mark.Draw(m_gdi);
	changeList->Reset();
}

// FUNCTION: LEMBALL 0x0044b340
void CCdLoadAnim::Draw(short p_progress)
{
	SyncLoadProgress();
	m_progress = p_progress;
	m_display->RefreshView();
}

// GLOBAL: LEMBALL 0x0049f9b0
unsigned int g_dwCdLoadAnimCompactPoints[5] = {0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004};

// GLOBAL: LEMBALL 0x0049f9c8
unsigned int g_dwCdLoadAnimFullPoints[5] = {0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008};
