#include "CCDLoadAnim.h"

#include "../../Control/Game/GameMain.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Visos/Foundation/CChangeList.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVSPoint.h"
#include "../../Visos/Foundation/CVSRect.h"
#include "../../Visos/Foundation/CVector.h"
#include "../../Visos/Foundation/FixedPoint.h"
#include "../../Visos/Foundation/VSTrig.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResBITMAP.h"
#include "../../Visos/Resources/CResPALETTE.h"
#include "../../Visos/Resources/Manifest.h"
#include "Visos/Animation/CAnimsManager.h"
#include "Visos/Animation/CRepeatAnim.h"
#include "Visos/Graphics/CBigBitmap.h"
#include "Visos/Graphics/CBitmap.h"
#include "Visos/Graphics/CCopyToBackBuff.h"
#include "Visos/Graphics/CDrawingMark.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CSolidRect.h"
#include "Visos/Resources/ResourceLimits.h"

#include <stddef.h>

class CAnimFrameBASE;

enum {
	CD_LOAD_ANIMATION_FRAME_DURATION_MS = 66
};

// FUNCTION: LEMBALL 0x0044aa80
CCDLoadAnim::CCDLoadAnim(CGDI* p_gdi, CMain2DDisplay* p_display) : CAnimsManager(p_gdi, RESOURCE_ID_COUNT, 1, 1, 0, 0)
{
	unsigned int* points;
	union {
		unsigned int m_value;
		short m_coordinate[2];
	} packed;
	int index;
	CResPALETTE* palette;
	unsigned long animCount;

	m_display = p_display;
	m_gdi = p_gdi;
	m_points = new CVSPoint[5];
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
		m_display->Clear(WINDOW_CLEAR_DEFAULT_COLOUR);
	}
	p_display->AttachPalette(RES_FRONTEND_LOADING_LORES_PALETTE);
	palette->UnLoad();
	CVSRect& windowRect = m_display->m_rect;
	CVSPoint centre((short) ((short) (windowRect.m_width - m_backgroundBitmap->m_x) / 2),
					(short) ((short) (windowRect.m_height - m_backgroundBitmap->m_y) / 2));
	m_centre.m_x = centre.m_x;
	m_centre.m_y = centre.m_y;
	index = 0;
	do {
		packed.m_value = *points;
		CVSPoint& dest = m_points[index];
		dest.m_x = packed.m_coordinate[0];
		dest.m_y = packed.m_coordinate[1];
		index++;
		points = points + 1;
	} while (index < 5);
	m_progress = 0;
	m_initialDraw = 1;
	animCount = GetnAnims(m_animResourceId);
	m_repeatAnim = new CRepeatAnim(animCount, 1);
	m_repeatAnim->m_fixedTime = ANIMATION_TIME_REALTIME;
	m_repeatAnim->StartAnim(animCount * CD_LOAD_ANIMATION_FRAME_DURATION_MS);
}

// FUNCTION: LEMBALL 0x0044ad60
CCDLoadAnim::~CCDLoadAnim()
{
	delete[] m_points;
	delete m_repeatAnim;
	UnLoadAnims(m_animResourceId);
	if (m_backgroundBitmap != NULL) {
		m_backgroundBitmap->UnLoad();
	}
	m_foregroundBitmap->UnLoad();
	if (m_display->m_lifecycleRefs == 1) {
		m_display->Clear(WINDOW_CLEAR_DEFAULT_COLOUR);
	}
}

// FUNCTION: LEMBALL 0x0044ae80
void CCDLoadAnim::InitialiseScreen()
{
	int remaining;

	remaining = 1;
	do {
		if (m_display->m_lifecycleRefs == 1) {
			m_display->Refresh(NULL);
		}
		remaining = remaining - 1;
	} while (remaining != 0);
	m_backgroundBitmap->UnLoad();
	m_backgroundBitmap = NULL;
}

// FUNCTION: LEMBALL 0x0044aec0
void CCDLoadAnim::Draw()
{
	enum {
		CCD_LOAD_ANIM_PROGRESS_PERCENT_MAX = 100
	};
	short tipY;
	short tipX;
	int angle;
	CChangeList* changeList;

	m_gdi->m_renderTarget->GetCurrDB();
	m_display->SetZoom(1);
	if (m_initialDraw != 0) {
		m_initialDraw = m_initialDraw - 1;
		const CVSRect& displayRect = m_display->m_rect;
		m_line[0].m_bounds = CVSRect(0, 0, displayRect.m_width, displayRect.m_height);
		m_line[0].m_colour = 0;
		m_line[0].Draw(m_gdi);
		CResBITMAP* background = m_backgroundBitmap;
		m_bitmapRes[0].CVSPoint::operator=(m_centre);
		m_bitmapRes[0].m_resource = background;
		m_bitmapRes[0].m_flags = 0;
		m_bitmapRes[0].m_remap = NULL;
		m_bitmapRes[0].Draw(m_gdi);
		const CVSRect& clearRect = m_display->m_rect;
		CVSRect rect(clearRect);
		rect.m_x = rect.m_y = 0;
		m_clearBitmap[0].CVSPoint::operator=(CVSPoint(0, 0));
		m_clearBitmap[0].m_destination = rect;
		m_clearBitmap[0].Draw(m_gdi);
	}
	CResBITMAP* foreground = m_foregroundBitmap;
	m_fgBlit[0].CVSPoint::operator=(
		CVSPoint((short) (m_points->m_x + m_centre.m_x), (short) (m_points->m_y + m_centre.m_y)));
	m_fgBlit[0].m_resource = foreground;
	m_fgBlit[0].m_flags = 0;
	m_fgBlit[0].m_remap = NULL;
	m_fgBlit[0].Draw(m_gdi);
	DrawAnim(CVSPoint((short) (m_points[1].m_x + m_centre.m_x), (short) (m_points[1].m_y + m_centre.m_y)),
			 m_animResourceId,
			 0,
			 (CAnimFrameBASE*) m_repeatAnim,
			 NULL);
	short originStorage[2];
	originStorage[0] = (short) (m_points[2].m_x + m_centre.m_x);
	originStorage[1] = (short) (m_points[2].m_y + m_centre.m_y);
	const short& originX = originStorage[0];
	const short& originY = originStorage[1];
	CVector radius((long) (short) -m_points[3].m_x, 0L);
	const CVSPoint& thickness = m_points[4];
	int thicknessY = ((int) thickness.m_y) << FIXED_POINT_FRACTION_BITS;
	int thicknessX = ((int) thickness.m_x) << FIXED_POINT_FRACTION_BITS;
	CVector left = radius + CVector(thicknessX, thicknessY);
	thicknessY = (-(int) m_points[4].m_y) << FIXED_POINT_FRACTION_BITS;
	thicknessX = ((int) m_points[4].m_x) << FIXED_POINT_FRACTION_BITS;
	CVector right = radius + CVector(thicknessX, thicknessY);
	angle = m_progress;
	if (CCD_LOAD_ANIM_PROGRESS_PERCENT_MAX < angle) {
		angle = CCD_LOAD_ANIM_PROGRESS_PERCENT_MAX;
	}
	angle = (angle * TRIG_ANGLE_HALF_TURN) / CCD_LOAD_ANIM_PROGRESS_PERCENT_MAX;
	VSTrig* trig = g_pVSTrig;
	short radiusPoint[2];
	{
		CFixed sine = trig->Sin(angle);
		CFixed cosine = trig->Cos(angle);
		CVector rotated = trig->Rotate(radius, sine, cosine);
		radiusPoint[0] = (short) (rotated.m_xFixed >> FIXED_POINT_FRACTION_BITS);
		radiusPoint[1] = (short) (rotated.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	}
	const short& radiusX = radiusPoint[0];
	const short& radiusY = radiusPoint[1];
	trig = g_pVSTrig;
	CVector rotatedLeft = CVector(trig->Rotate(left, angle));
	trig = g_pVSTrig;
	CVector rotatedRight = CVector(trig->Rotate(right, angle));
	tipX = (short) (originX + radiusX);
	tipY = (short) (originY + radiusY);
	m_needle0[0].m_start.m_x = originX;
	m_needle0[0].m_start.m_y = originY;
	m_needle0[0].m_end.operator=(CVSPoint(tipX, tipY));
	m_needle0[0].m_colour = 0x66;
	m_needle0[0].Draw(m_gdi);
	{
		short pointStorage[2];
		CVSPoint& point = *(CVSPoint*) pointStorage;
		point.m_x = (short) ((rotatedLeft.m_xFixed >> FIXED_POINT_FRACTION_BITS) + originX);
		m_needle1[0].m_start.m_x = tipX;
		m_needle1[0].m_start.m_y = tipY;
		point.m_y = (short) ((rotatedLeft.m_yFixed >> FIXED_POINT_FRACTION_BITS) + originY);
		m_needle1[0].m_end.operator=(point);
		m_needle1[0].m_colour = 0xba;
		m_needle1[0].Draw(m_gdi);
	}
	{
		short pointStorage[2];
		CVSPoint& point = *(CVSPoint*) pointStorage;
		point.m_x = (short) ((rotatedRight.m_xFixed >> FIXED_POINT_FRACTION_BITS) + originX);
		m_needle2[0].m_start.m_x = tipX;
		m_needle2[0].m_start.m_y = tipY;
		point.m_y = (short) ((rotatedRight.m_yFixed >> FIXED_POINT_FRACTION_BITS) + originY);
		m_needle2[0].m_end.operator=(point);
		m_needle2[0].m_colour = 0xbf;
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
