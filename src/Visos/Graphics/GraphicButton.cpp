#include "Gdi.h"
#include "GraphicButton.h"

#include "../Foundation/ChangeList.h"
#include "../Animation/Anim.h"
#include "../Resources/ResAnim.h"
#include "VsGdi.h"

#include <new.h>

// GLOBAL: LEMBALL 0x0049f02c
char g_szButton[] = "Button";

// 68K 0x10210510 __ct__14CGraphicButtonFRC8CVSPointP7CPVGWndUlUl
// STUB: LEMBALL 0x00468530
GraphicButton::GraphicButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, unsigned long p_arg3)
	: PvButton(p_arg1)
{
	VsRect createRect;

	m_graphicHeight = 0;
	m_graphicWidth = 0;
	m_graphicOffsetY = 0;
	m_graphicOffsetX = 0;
	m_state = 0;
	m_enabled = 0;
	m_alignmentFlags = p_arg3;
	m_animationId = p_arg2;
	Initialise();
	m_buttonX = p_arg0.m_x;
	m_buttonY = p_arg0.m_y;
	createRect.m_width = (short) m_graphicWidth;
	createRect.m_height = (short) m_graphicHeight;
	createRect.m_x = p_arg0.m_x;
	createRect.m_y = p_arg0.m_y;
	Create(createRect, p_arg1, g_szButton);
}

// 68K 0x1021068c Initialise__14CGraphicButtonFv
// STUB: LEMBALL 0x004686e0
void GraphicButton::Initialise()
{
	ResZrle* entries;
	void* storage;

	m_frame = 0;
	storage = operator new(0x20);
	if (storage == 0) {
		m_primitive = 0;
	}
	else {
		m_primitive = new (storage) Anim();
	}
	m_gdiFlags = m_gdiFlags + 1;
	m_animation = ResAnim::Load(m_animationId);
	if (m_animation == 0) {
		return;
	}
	if (m_animation->m_loaded == 0) {
		m_animation->LoadData();
	}
	else {
		m_animation->m_age = 0;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount + 1;
	entries = m_animation->m_animationEntries;
	if (entries != 0) {
		m_graphicWidth = (unsigned short) entries->m_width;
		m_graphicHeight = (unsigned short) entries->m_height;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	m_graphicOffsetX = 0;
	m_graphicOffsetY = 0;
	if ((m_alignmentFlags & 4) != 0) {
		m_graphicOffsetX = (short) (((int) m_rect.m_width - (int) (short) m_graphicWidth) / 2);
	}
	if ((m_alignmentFlags & 8) != 0) {
		m_graphicOffsetY = (short) (((int) m_rect.m_height - (int) (short) m_graphicHeight) / 2);
	}
}

// 68K 0x10210906 SetAnimID__14CGraphicButtonFUl
// STUB: LEMBALL 0x004688e0
void GraphicButton::SetAnimId(unsigned long p_animId)
{
}

// 68K 0x10210a2a OnDestroy__14CGraphicButtonFv
// STUB: LEMBALL 0x00468980
void GraphicButton::OnDestroy()
{
}

// STUB: LEMBALL 0x00468300 FOLDED
void GraphicButton::_DrawButton()
{
	if (m_enabled == m_state) {
		if (m_gdi->m_renderTarget->PvSurface::HasBackBuff() == 0) {
			CheckForceDraw();
			return;
		}
	}
	m_gdi->m_renderTarget->m_flag78 = 1;
	m_state = m_enabled;
	CheckForceDraw();
}

// 68K 0x10210a70 DrawButton__14CGraphicButtonFv
// STUB: LEMBALL 0x004689a0
void GraphicButton::DrawButton()
{
	unsigned int pressed;
	Anim* primitive;
	short x;
	short y;

	x = m_graphicOffsetX;
	y = m_graphicOffsetY;
	pressed = 0;
	if (m_enabled != 0 && HotAreaHandler::m_active != 0) {
		pressed = 1;
	}
	if ((m_alignmentFlags & 0x40) != 0 && pressed != 0) {
		x = (short) (x + 1);
		y = (short) (y + 1);
	}
	m_gdi->m_renderTarget->GetCurrDb();
	primitive = (Anim*) m_primitive;
	primitive->m_x = x;
	primitive->m_y = y;
	primitive->m_animResource = m_animation;
	primitive->m_animIndex = pressed;
	primitive->m_flags = 0;
	primitive->m_remap = (Remap*) m_frame;
	primitive->Draw(m_gdi);
}

// STUB: LEMBALL 0x00468360 FOLDED
void GraphicButton::OnPaint(const VsRect& p_rect)
{
	int clipOk;
	ChangeList* changeList;

	clipOk = m_gdi->m_renderTarget->PvSurface::HasBackBuff();
	if ((clipOk != 0 && (m_pressed != 0 || m_enabled != m_state)) ||
		(m_gdi->m_primitiveCount == 0 &&
		 (m_autoDraw != 0 || m_forceDrawCount != 0 || m_pressed != m_lastDrawnPressed))) {
		if (GetSizeStatus() != 0) {
			_DrawButton();
			DrawButton();
		}
		changeList = m_gdi->m_renderTarget->GetChangeList();
		m_gdi->AddToList(PvButton::m_primitive);
		changeList->Reset();
		m_drawCompleted = 1;
	}
}

// 68K 0x10210966 __dt__14CGraphicButtonFv
GraphicButton::~GraphicButton()
{
}
