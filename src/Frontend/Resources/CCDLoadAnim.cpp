#include "CCDLoadAnim.h"

#include "../../Control/Game/GameMain.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVector.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/VSTrig.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/CResPALETTE.h"
#include "../../Visos/Resources/Manifest.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CRepeatAnim.h"
#include "Visos/Graphics/CBitmap.h"
#include "Visos/Graphics/CBitmapRes.h"
#include "Visos/Graphics/CBitmapResBase.h"
#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CLine.h"

class CAnimFrameBASE;

// FUNCTION: LEMBALL 0x0044aa80
CCDLoadAnim::CCDLoadAnim(CGDI* p_gdi, CMain2DDisplay* p_display) : CAnimsManager(p_gdi, 0x2b6, 1, 1, 0, 0)
{
	unsigned int* points;
	union {
		unsigned int value;
		short coordinate[2];
	} packed;
	int offset;
	CVsPoint* dest;
	CResPALETTE* palette;
	unsigned long animCount;

	m_display = p_display;
	m_gdi = p_gdi;
	m_points = new CVsPoint[5];
	g_pCursor->SetActive(0);
	if (g_nCompactPrimaryContextLayout != 0) {
		m_backgroundBitmap = CResBITMAP::Load(RES_FRONTEND_LOADING_LORES_PICTURE);
		points = g_dwCdLoadAnimCompactPoints;
		m_foregroundBitmap = CResBITMAP::Load(RES_FRONTEND_LOADING_LORES_REPLACE);
		m_animResourceId = RES_FRONTEND_LOADING_LORES_PAINTDRIP;
	}
	else {
		m_backgroundBitmap = CResBITMAP::Load(RES_FRONTEND_LOADING_HIRES_PICTURE);
		points = g_dwCdLoadAnimFullPoints;
		m_foregroundBitmap = CResBITMAP::Load(RES_FRONTEND_LOADING_HIRES_REPLACE);
		m_animResourceId = RES_FRONTEND_LOADING_HIRES_PAINTDRIP;
	}
	LoadAnims(m_animResourceId);
	palette = CResPALETTE::Load(RES_FRONTEND_LOADING_LORES_PALETTE);
	if (m_display->m_lifecycleRefs == 1) {
		m_display->Clear(-1);
	}
	p_display->AttachPalette(RES_FRONTEND_LOADING_LORES_PALETTE);
	palette->UnLoad();
	CVsRect& windowRect = m_display->m_rect;
	CVsPoint center((short) ((short) (windowRect.m_width - m_backgroundBitmap->m_x) / 2),
					(short) ((short) (windowRect.m_height - m_backgroundBitmap->m_y) / 2));
	m_center.m_x = center.m_x;
	m_center.m_y = center.m_y;
	offset = 0;
	do {
		packed.value = *points;
		dest = (CVsPoint*) ((int) &m_points->m_x + offset);
		dest->m_x = packed.coordinate[0];
		dest->m_y = packed.coordinate[1];
		offset = offset + 4;
		points = points + 1;
	} while (offset < 0x14);
	m_progress = 0;
	m_initialDraw = 1;
	animCount = GetnAnims(m_animResourceId);
	m_repeatAnim = new CRepeatAnim(animCount, 1);
	m_repeatAnim->m_fixedTime = 0xffffffff;
	m_repeatAnim->StartAnim(animCount * 0x42);
}

// FUNCTION: LEMBALL 0x0044ad60
CCDLoadAnim::~CCDLoadAnim()
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
void CCDLoadAnim::InitialiseScreen()
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
void CCDLoadAnim::Draw()
{
	short tipY;
	short tipX;
	int angle;
	CChangeList* changeList;

	m_gdi->m_renderTarget->GetCurrDB();
	m_display->SetZoom(1);
	if (m_initialDraw != 0) {
		m_initialDraw = m_initialDraw - 1;
		const CVsRect& displayRect = m_display->m_rect;
		m_line[0].m_bounds = CVsRect(0, 0, displayRect.m_width, displayRect.m_height);
		m_line[0].m_color = 0;
		m_line[0].Draw(m_gdi);
		CResBITMAP* background = m_backgroundBitmap;
		m_bitmapRes[0].CVsPoint::operator=(m_center);
		m_bitmapRes[0].m_resource = background;
		m_bitmapRes[0].m_flags = 0;
		m_bitmapRes[0].m_remap = 0;
		m_bitmapRes[0].Draw(m_gdi);
		const CVsRect& clearRect = m_display->m_rect;
		CVsRect rect(clearRect);
		rect.m_x = rect.m_y = 0;
		m_clearBitmap[0].CVsPoint::operator=(CVsPoint(0, 0));
		m_clearBitmap[0].m_sourceRect = rect;
		m_clearBitmap[0].Draw(m_gdi);
	}
	CResBITMAP* foreground = m_foregroundBitmap;
	m_fgBlit[0].CVsPoint::operator=(
		CVsPoint((short) (m_points->m_x + m_center.m_x), (short) (m_points->m_y + m_center.m_y)));
	m_fgBlit[0].m_resource = foreground;
	m_fgBlit[0].m_flags = 0;
	m_fgBlit[0].m_remap = 0;
	m_fgBlit[0].Draw(m_gdi);
	DrawAnim(CVsPoint((short) (m_points[1].m_x + m_center.m_x), (short) (m_points[1].m_y + m_center.m_y)),
			 m_animResourceId,
			 0,
			 (CAnimFrameBASE*) m_repeatAnim,
			 0);
	short originStorage[2];
	originStorage[0] = (short) (m_points[2].m_x + m_center.m_x);
	originStorage[1] = (short) (m_points[2].m_y + m_center.m_y);
	const short& originX = originStorage[0];
	const short& originY = originStorage[1];
	CVector radius((long) (short) -m_points[3].m_x, 0L);
	const CVsPoint& thickness = m_points[4];
	int thicknessY = ((int) thickness.m_y) << 12;
	int thicknessX = ((int) thickness.m_x) << 12;
	CVector left = radius + CVector(thicknessX, thicknessY);
	thicknessY = (-(int) m_points[4].m_y) << 12;
	thicknessX = ((int) m_points[4].m_x) << 12;
	CVector right = radius + CVector(thicknessX, thicknessY);
	angle = m_progress;
	if (100 < angle) {
		angle = 100;
	}
	angle = (angle << 8) / 100;
	VSTrig* trig = g_pVSTrig;
	short radiusPoint[2];
	{
		CFixed sine = trig->Sin(angle);
		CFixed cosine = trig->Cos(angle);
		CVector rotated = trig->Rotate(radius, sine, cosine);
		radiusPoint[0] = (short) (rotated.m_xFixed >> 12);
		radiusPoint[1] = (short) (rotated.m_yFixed >> 12);
	}
	const short& radiusX = radiusPoint[0];
	const short& radiusY = radiusPoint[1];
	trig = g_pVSTrig;
	CVector rotatedLeft = CVector(trig->Rotate(left, angle));
	trig = g_pVSTrig;
	CVector rotatedRight = CVector(trig->Rotate(right, angle));
	tipX = (short) (originX + radiusX);
	tipY = (short) (originY + radiusY);
	m_needle0[0].m_bounds.m_width = originX;
	m_needle0[0].m_bounds.m_height = originY;
	m_needle0[0].m_bounds.CVsPoint::operator=(CVsPoint(tipX, tipY));
	m_needle0[0].m_reserved0c = 0x66;
	m_needle0[0].Draw(m_gdi);
	{
		short x = (short) ((rotatedLeft.m_xFixed >> 12) + originX);
		short y = (short) ((rotatedLeft.m_yFixed >> 12) + originY);
		CVsPoint point(x, y);
		m_needle1[0].m_bounds.m_width = tipX;
		m_needle1[0].m_bounds.m_height = tipY;
		m_needle1[0].m_bounds.CVsPoint::operator=(point);
		m_needle1[0].m_reserved0c = 0xba;
		m_needle1[0].Draw(m_gdi);
	}
	{
		short x = (short) ((rotatedRight.m_xFixed >> 12) + originX);
		short y = (short) ((rotatedRight.m_yFixed >> 12) + originY);
		CVsPoint point(x, y);
		m_needle2[0].m_bounds.m_width = tipX;
		m_needle2[0].m_bounds.m_height = tipY;
		m_needle2[0].m_bounds.CVsPoint::operator=(point);
		m_needle2[0].m_reserved0c = 0xbf;
		m_needle2[0].Draw(m_gdi);
	}
	ResetPrimitives();
	changeList = m_gdi->m_renderTarget->GetChangeList();
	m_mark.Draw(m_gdi);
	changeList->Reset();
}

// FUNCTION: LEMBALL 0x0044b340
void CCDLoadAnim::Draw(short p_progress)
{
	SyncLoadProgress();
	m_progress = p_progress;
	m_display->RefreshView();
}

// GLOBAL: LEMBALL 0x0049f9b0
unsigned int g_dwCdLoadAnimCompactPoints[5] = {0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004};

// GLOBAL: LEMBALL 0x0049f9c8
unsigned int g_dwCdLoadAnimFullPoints[5] = {0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008};
