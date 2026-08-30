#include "CdLoadAnim.h"

#include "../../Control/Game/GameMain.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/Main2DDisplay.h"
#include "../../Visos/Foundation/ChangeList.h"
#include "../../Visos/Foundation/Fixed.h"
#include "../../Visos/Foundation/Vector.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsRect.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/GWnd.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/PvGWnd.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/ResBitmap.h"
#include "../../Visos/Resources/ResPalette.h"

#include <new.h>

// 68K 0x10801d2c __ct__11CCDLoadAnimFP4CGDIP14CMain2DDisplay
// FUNCTION: LEMBALL 0x0044aa80
CdLoadAnim::CdLoadAnim(Gdi* p_arg0, Main2DDisplay* p_arg1) : AnimsManager(p_arg0, 0x2b6, 1, 1, 0, 0)
{
	unsigned int* points;
	unsigned int packed;
	int offset;
	VsPoint* dest;
	ResPalette* palette;
	unsigned long animCount;

	m_progress.m_draw.m_display = p_arg1;
	m_progress.m_draw.m_gdi = p_arg0;
	m_progress.m_draw.m_points = new VsPoint[5];
	g_pCursor->SetActive(0);
	if (g_nCompactPrimaryContextLayout != 0) {
		m_progress.m_draw.m_backgroundBitmap = ResBitmap::Load(RES_FRONTEND_LOADING_LORES_PICTURE);
		points = g_dwCdLoadAnimCompactPoints;
		m_progress.m_draw.m_foregroundBitmap = ResBitmap::Load(RES_FRONTEND_LOADING_LORES_REPLACE);
		m_progress.m_draw.m_animResourceId = 0xf0;
	}
	else {
		m_progress.m_draw.m_backgroundBitmap = ResBitmap::Load(RES_FRONTEND_LOADING_HIRES_PICTURE);
		points = g_dwCdLoadAnimFullPoints;
		m_progress.m_draw.m_foregroundBitmap = ResBitmap::Load(RES_FRONTEND_LOADING_HIRES_REPLACE);
		m_progress.m_draw.m_animResourceId = 0xf3;
	}
	LoadAnims(m_progress.m_draw.m_animResourceId);
	palette = ResPalette::Load(RES_FRONTEND_LOADING_LORES_PALETTE);
	if (m_progress.m_draw.m_display->m_lifecycleRefs == 1) {
		m_progress.m_draw.m_display->Clear(-1);
	}
	p_arg1->AttachPalette(RES_FRONTEND_LOADING_LORES_PALETTE);
	palette->UnLoad();
	m_progress.m_draw.m_centerY =
		(short) ((m_progress.m_draw.m_display->m_rect.m_height - m_progress.m_draw.m_backgroundBitmap->m_y) / 2);
	m_progress.m_draw.m_centerX =
		(short) ((m_progress.m_draw.m_display->m_rect.m_width - m_progress.m_draw.m_backgroundBitmap->m_x) / 2);
	offset = 0;
	do {
		packed = *points;
		points = points + 1;
		dest = (VsPoint*) ((int) &m_progress.m_draw.m_points->m_x + offset);
		dest->m_x = (short) packed;
		dest->m_y = (short) (packed >> 0x10);
		offset = offset + 4;
	} while (offset < 0x14);
	m_progress.m_draw.m_progress = 0;
	m_progress.m_draw.m_initialDraw = 1;
	animCount = GetnAnims(m_progress.m_draw.m_animResourceId);
	m_progress.m_draw.m_repeatAnim = new RepeatAnim(animCount, 1);
	m_progress.m_draw.m_repeatAnim->m_fixedTime = 0xffffffff;
	m_progress.m_draw.m_repeatAnim->StartAnim(animCount * 0x42);
}

// 68K 0x108020ac __dt__11CCDLoadAnimFv
// FUNCTION: LEMBALL 0x0044ad60
CdLoadAnim::~CdLoadAnim()
{
	delete[] m_progress.m_draw.m_points;
	delete m_progress.m_draw.m_repeatAnim;
	UnLoadAnims(m_progress.m_draw.m_animResourceId);
	if (m_progress.m_draw.m_backgroundBitmap != 0) {
		m_progress.m_draw.m_backgroundBitmap->UnLoad();
	}
	m_progress.m_draw.m_foregroundBitmap->UnLoad();
	if (m_progress.m_draw.m_display->m_lifecycleRefs == 1) {
		m_progress.m_draw.m_display->Clear(-1);
	}
}

// 68K 0x10802252 InitialiseScreen__11CCDLoadAnimFv
// FUNCTION: LEMBALL 0x0044ae80
void CdLoadAnim::InitialiseScreen()
{
	int remaining;

	remaining = 1;
	do {
		if (m_progress.m_draw.m_display->m_lifecycleRefs == 1) {
			m_progress.m_draw.m_display->Refresh(0);
		}
		remaining = remaining - 1;
	} while (remaining != 0);
	m_progress.m_draw.m_backgroundBitmap->UnLoad();
	m_progress.m_draw.m_backgroundBitmap = 0;
}

// 68K 0x108022ce Draw__11CCDLoadAnimFv
// FUNCTION: LEMBALL 0x0044aec0
void CdLoadAnimDraw::Draw()
{
	CdLoadAnim* anims;
	VsRect rect;
	VsPoint point;
	VsPoint origin;
	VsPoint tip;
	Vector radius;
	Vector left;
	Vector right;
	Vector thick;
	Fixed sine;
	Fixed cosine;
	int angle;
	short rotX;
	short rotY;
	ChangeList* changeList;

	m_gdi->m_renderTarget->GetCurrDb();
	m_display->SetZoom(1);
	if (m_initialDraw != 0) {
		m_initialDraw = m_initialDraw - 1;
		rect.m_width = m_display->m_rect.m_width;
		rect.m_height = m_display->m_rect.m_height;
		rect.m_x = 0;
		rect.m_y = 0;
		*(VsRect*) &m_line[0].m_x1 = rect;
		m_line[0].m_color = 0;
		m_line[0].Draw(m_gdi);
		*(VsPoint*) &m_bitmapRes[0].m_x = *(VsPoint*) &m_centerX;
		m_bitmapRes[0].m_resource = m_backgroundBitmap;
		m_bitmapRes[0].m_flags = 0;
		m_bitmapRes[0].m_remap = 0;
		m_bitmapRes[0].Draw(m_gdi);
		point.m_x = 0;
		point.m_y = 0;
		*(VsPoint*) &m_clearBitmap[0].m_x = point;
		rect.m_width = m_display->m_rect.m_width;
		rect.m_height = m_display->m_rect.m_height;
		rect.m_x = 0;
		rect.m_y = 0;
		*(VsRect*) &m_clearBitmap[0].m_width = rect;
		m_clearBitmap[0].Draw(m_gdi);
	}
	point.m_x = (short) (m_points->m_x + m_centerX);
	point.m_y = (short) (m_points->m_y + m_centerY);
	*(VsPoint*) &m_fgBlit[0].m_x = point;
	m_fgBlit[0].m_resource = m_foregroundBitmap;
	m_fgBlit[0].m_flags = 0;
	m_fgBlit[0].m_remap = 0;
	m_fgBlit[0].Draw(m_gdi);
	point.m_x = (short) (m_points[1].m_x + m_centerX);
	point.m_y = (short) (m_points[1].m_y + m_centerY);
	anims = (CdLoadAnim*) ((unsigned int) this - 0x74);
	anims->DrawAnim(point, m_animResourceId, 0, (Frames*) m_repeatAnim, 0);
	origin.m_x = (short) (m_points[2].m_x + m_centerX);
	origin.m_y = (short) (m_points[2].m_y + m_centerY);
	radius.SetIntegers(-m_points[3].m_x, 0);
	thick.m_xFixed = ((int) m_points[4].m_x) << 12;
	thick.m_yFixed = ((int) m_points[4].m_y) << 12;
	left = radius + thick;
	thick.m_xFixed = ((int) m_points[4].m_x) << 12;
	thick.m_yFixed = ((int) -m_points[4].m_y) << 12;
	right = radius + thick;
	angle = m_progress;
	if (100 < angle) {
		angle = 100;
	}
	angle = (angle << 8) / 100;
	sine = g_pVSTrig->Sin(angle);
	cosine = g_pVSTrig->Cos(angle);
	radius = g_pVSTrig->Rotate(radius, sine, cosine);
	rotX = (short) (radius.m_xFixed >> 12);
	rotY = (short) (radius.m_yFixed >> 12);
	sine = g_pVSTrig->Sin(angle);
	cosine = g_pVSTrig->Sin(angle + 0x80);
	left = g_pVSTrig->Rotate(left, sine, cosine);
	sine = g_pVSTrig->Sin(angle);
	cosine = g_pVSTrig->Sin(angle + 0x80);
	right = g_pVSTrig->Rotate(right, sine, cosine);
	tip.m_x = (short) (origin.m_x + rotX);
	tip.m_y = (short) (origin.m_y + rotY);
	m_needle0[0].m_x1 = origin.m_x;
	m_needle0[0].m_y1 = origin.m_y;
	*(VsPoint*) &m_needle0[0].m_x2 = tip;
	m_needle0[0].m_color = 0x66;
	m_needle0[0].Draw(m_gdi);
	point.m_x = (short) ((left.m_xFixed >> 12) + tip.m_x);
	point.m_y = (short) ((left.m_yFixed >> 12) + tip.m_y);
	m_needle1[0].m_x1 = tip.m_x;
	m_needle1[0].m_y1 = tip.m_y;
	*(VsPoint*) &m_needle1[0].m_x2 = point;
	m_needle1[0].m_color = 0xba;
	m_needle1[0].Draw(m_gdi);
	point.m_x = (short) ((right.m_xFixed >> 12) + tip.m_x);
	point.m_y = (short) ((right.m_yFixed >> 12) + tip.m_y);
	m_needle2[0].m_x1 = tip.m_x;
	m_needle2[0].m_y1 = tip.m_y;
	*(VsPoint*) &m_needle2[0].m_x2 = point;
	m_needle2[0].m_color = 0xbf;
	m_needle2[0].Draw(m_gdi);
	anims->ResetPrimitives();
	changeList = m_gdi->m_renderTarget->GetChangeList();
	m_mark.Draw(m_gdi);
	changeList->Reset();
}

// 68K 0x108028fe Draw__11CCDLoadAnimFs
// FUNCTION: LEMBALL 0x0044b340
void CdLoadAnimProgress::Draw(short p_progress)
{
	TargetSynchronizeLoadProgress();
	m_draw.m_progress = p_progress;
	m_draw.m_display->RefreshView();
}

// GLOBAL: LEMBALL 0x0049f9b0
unsigned int g_dwCdLoadAnimCompactPoints[5] = {0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004};

// GLOBAL: LEMBALL 0x0049f9c8
unsigned int g_dwCdLoadAnimFullPoints[5] = {0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008};
