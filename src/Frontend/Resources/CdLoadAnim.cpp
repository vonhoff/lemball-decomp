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

// GLOBAL: LEMBALL 0x0049f9b0
unsigned int g_dwCdLoadAnimCompactPoints[5] = {0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004};

// GLOBAL: LEMBALL 0x0049f9c8
unsigned int g_dwCdLoadAnimFullPoints[5] = {0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008};
