#include "CdLoadAnimDraw.h"

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
#include "CdLoadAnim.h"

#include <new.h>

// 68K 0x108022ce Draw__11CCDLoadAnimFv
// FUNCTION: LEMBALL 0x0044aec0
void CdLoadAnimDraw::Draw()
{
	CdLoadAnim* anims;
	int angle;
	short rotX;
	short rotY;
	ChangeList* changeList;

	m_gdi->m_renderTarget->GetCurrDb();
	m_display->SetZoom(1);
	if (m_initialDraw != 0) {
		m_initialDraw = m_initialDraw - 1;
		VsRect rect;
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
		VsPoint point(0, 0);
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
	VsPoint point((short) (m_points->m_x + m_centerX), (short) (m_points->m_y + m_centerY));
	m_fgBlit[0].m_x = point.m_x;
	m_fgBlit[0].m_y = point.m_y;
	m_fgBlit[0].m_resource = m_foregroundBitmap;
	m_fgBlit[0].m_flags = 0;
	m_fgBlit[0].m_remap = 0;
	m_fgBlit[0].Draw(m_gdi);
	point.m_x = (short) (m_points[1].m_x + m_centerX);
	point.m_y = (short) (m_points[1].m_y + m_centerY);
	anims = (CdLoadAnim*) ((unsigned int) this - 0x74);
	anims->DrawAnim(point, m_animResourceId, 0, (Frames*) m_repeatAnim, 0);
	VsPoint origin((short) (m_points[2].m_x + m_centerX), (short) (m_points[2].m_y + m_centerY));
	Vector radius(0, 0);
	radius.SetIntegers((short) -m_points[3].m_x, 0);
	Vector thick(((int) m_points[4].m_x) << 12, ((int) m_points[4].m_y) << 12);
	Vector left = radius + thick;
	thick.m_xFixed = ((int) m_points[4].m_x) << 12;
	thick.m_yFixed = ((int) -m_points[4].m_y) << 12;
	Vector right = radius + thick;
	angle = m_progress;
	if (100 < angle) {
		angle = 100;
	}
	angle = (angle << 8) / 100;
	VsTrig* trig = g_pVSTrig;
	Fixed sine = trig->Sin(angle);
	Fixed cosine = trig->Cos(angle);
	Vector rotatedRadius = trig->Rotate(radius, sine, cosine);
	rotX = (short) (rotatedRadius.m_xFixed >> 12);
	rotY = (short) (rotatedRadius.m_yFixed >> 12);
	trig = g_pVSTrig;
	Fixed leftSine = trig->Sin(angle);
	Fixed leftCosine = trig->Sin(angle + 0x80);
	Vector rotatedLeft = trig->Rotate(left, leftSine, leftCosine);
	trig = g_pVSTrig;
	Fixed rightSine = trig->Sin(angle);
	Fixed rightCosine = trig->Sin(angle + 0x80);
	Vector rotatedRight = trig->Rotate(right, rightSine, rightCosine);
	VsPoint tip(0, 0);
	tip.m_x = (short) (origin.m_x + rotX);
	tip.m_y = (short) (origin.m_y + rotY);
	m_needle0[0].m_x1 = origin.m_x;
	m_needle0[0].m_y1 = origin.m_y;
	m_needle0[0].m_x2 = tip.m_x;
	m_needle0[0].m_y2 = tip.m_y;
	m_needle0[0].m_color = 0x66;
	m_needle0[0].Draw(m_gdi);
	point.m_x = (short) ((rotatedLeft.m_xFixed >> 12) + origin.m_x);
	point.m_y = (short) ((rotatedLeft.m_yFixed >> 12) + origin.m_y);
	m_needle1[0].m_x1 = tip.m_x;
	m_needle1[0].m_y1 = tip.m_y;
	m_needle1[0].m_x2 = point.m_x;
	m_needle1[0].m_y2 = point.m_y;
	m_needle1[0].m_color = 0xba;
	m_needle1[0].Draw(m_gdi);
	point.m_x = (short) ((rotatedRight.m_xFixed >> 12) + origin.m_x);
	point.m_y = (short) ((rotatedRight.m_yFixed >> 12) + origin.m_y);
	m_needle2[0].m_x1 = tip.m_x;
	m_needle2[0].m_y1 = tip.m_y;
	m_needle2[0].m_x2 = point.m_x;
	m_needle2[0].m_y2 = point.m_y;
	m_needle2[0].m_color = 0xbf;
	m_needle2[0].Draw(m_gdi);
	anims->ResetPrimitives();
	changeList = m_gdi->m_renderTarget->GetChangeList();
	m_mark.Draw(m_gdi);
	changeList->Reset();
}
