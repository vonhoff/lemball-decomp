#include "Gdi.h"
#include "GraphicButton.h"
#include "HotAreaList.h"
#include "PushActive.h"

#include "../Foundation/ChangeList.h"
#include "../Animation/Anim.h"
#include "../Resources/ResAnim.h"
#include "VsGdi.h"

#include <new.h>

// GLOBAL: LEMBALL 0x0049f02c
char g_szButton[] = "Button";

// 68K 0x10210510 __ct__14CGraphicButtonFRC8CVSPointP7CPVGWndUlUl
// FUNCTION: LEMBALL 0x00468530
GraphicButton::GraphicButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, unsigned long p_arg3)
	: PvButton(p_arg1)
{
	VsRect createRect;
	HotAreaHandler* area;
	PushActive* push;
	int remaining;

	m_graphicHeight = 0;
	m_graphicWidth = 0;
	m_graphicOffsetY = 0;
	m_graphicOffsetX = 0;
	m_state = 0;
	m_enabled = 0;
	remaining = 0;
	push = (PushActive*) m_statRegion;
	do {
		new (push) PushActive();
		remaining = remaining - 1;
		push = push + 1;
	} while (-1 < remaining);
	m_alignmentFlags = p_arg3;
	m_animationId = p_arg2;
	Initialise();
	m_buttonX = p_arg0.m_x;
	m_buttonY = p_arg0.m_y;
	area = this;
	createRect.m_width = area->m_width;
	createRect.m_height = area->m_height;
	createRect.m_x = p_arg0.m_x;
	createRect.m_y = p_arg0.m_y;
	Create(createRect, p_arg1, g_szButton);
	area->m_x = (short) (area->m_x + m_relativeTopLeft.m_x);
	area->m_y = (short) (area->m_y + m_relativeTopLeft.m_y);
	if (m_ownerWindow != 0 && m_ownerWindow->m_hotAreaList != 0) {
		m_ownerWindow->m_hotAreaList->AddToList(area);
	}
}

// 68K 0x1021068c Initialise__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x004686e0
void GraphicButton::Initialise()
{
	ResZrle* entries;
	void* storage;
	HotAreaHandler* area;
	short boxWidth;
	short boxHeight;

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
		if (m_animation->m_totalSize > 1) {
			if ((short) m_graphicWidth < entries[1].m_width) {
				m_graphicWidth = (unsigned short) entries[1].m_width;
			}
			if ((short) m_graphicHeight < entries[1].m_height) {
				m_graphicHeight = (unsigned short) entries[1].m_height;
			}
		}
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	area = this;
	m_graphicOffsetX = area->m_x;
	m_graphicOffsetY = area->m_y;
	boxWidth = area->m_width;
	if (boxWidth < 0) {
		area->m_width = (short) (-(short) m_graphicWidth * boxWidth);
	}
	else if (boxWidth == 0) {
		area->m_width = (short) m_graphicWidth;
	}
	boxHeight = area->m_height;
	if (boxHeight < 0) {
		area->m_height = (short) (-(short) m_graphicHeight * boxHeight);
	}
	else if (boxHeight == 0) {
		area->m_height = (short) m_graphicHeight;
	}
	if ((int) area->m_width * (int) area->m_height != 0) {
		area->SetActive(1);
	}
	if ((m_alignmentFlags & 4) != 0) {
		m_graphicOffsetX = (short) (((int) area->m_width - (int) (short) m_graphicWidth) / 2);
	}
	else if ((m_alignmentFlags & 0x10) != 0) {
		m_graphicOffsetX = (short) (area->m_width - (short) m_graphicWidth);
	}
	if ((m_alignmentFlags & 8) != 0) {
		m_graphicOffsetY = (short) (((int) area->m_height - (int) (short) m_graphicHeight) / 2);
		return;
	}
	if ((m_alignmentFlags & 0x20) != 0) {
		m_graphicOffsetY = (short) (area->m_height - (short) m_graphicHeight);
	}
}

// 68K 0x10210906 SetAnimID__14CGraphicButtonFUl
// FUNCTION: LEMBALL 0x004688e0
void GraphicButton::SetAnimId(unsigned long p_animId)
{
	if (m_animation != 0) {
		m_animation->UnLoad();
	}
	m_animationId = p_animId;
	m_animation = ResAnim::Load(p_animId);
	m_forceDrawCount = 1;
}

// 68K 0x10210a2a OnDestroy__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x00468980
void GraphicButton::OnDestroy()
{
	if (m_animation != 0) {
		m_animation->UnLoad();
		m_animation = 0;
	}
}

// FUNCTION: LEMBALL 0x00468300 FOLDED
void GraphicButton::_DrawButton()
{
	if (m_enabled == m_state) {
		if (m_gdi->m_renderTarget->HasBackBuff() == 0) {
			CheckForceDraw();
			return;
		}
	}
	m_gdi->m_renderTarget->m_flag78 = 1;
	m_state = m_enabled;
	CheckForceDraw();
}

// 68K 0x10210a70 DrawButton__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x004689a0
void GraphicButton::DrawButton()
{
	short x = m_graphicOffsetX;
	short y = m_graphicOffsetY;
	unsigned int pressed = 0;

	if (m_enabled != 0 && HotAreaHandler::m_active != 0) {
		pressed = 1;
	}
	if ((m_alignmentFlags & 0x40) != 0 && pressed != 0) {
		x++;
		y++;
	}
	m_gdi->m_renderTarget->GetCurrDb();
	Anim* primitive = (Anim*) m_primitive;
	primitive->m_x = x;
	primitive->m_y = y;
	primitive->m_animResource = m_animation;
	primitive->m_animIndex = (unsigned int) (pressed >= 1);
	primitive->m_flags = 0;
	primitive->m_remap = (Remap*) m_frame;
	primitive->Draw(m_gdi);
}

// FUNCTION: LEMBALL 0x0043a620 FOLDED
void GraphicButton::OnReleased(int p_flags)
{
	if (m_pressed != 0 && (p_flags == 0 || p_flags == 3)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0043a660 FOLDED
void GraphicButton::OnPressed(int p_flags)
{
	if (m_pressed != 0 && (p_flags == 0 || p_flags == 3)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0043a6a0 FOLDED
void GraphicButton::OnEnterButton()
{
	if (m_pressed != 0 && (m_buttonState[0] != 0 || m_buttonState[3] != 0)) {
		m_enabled = 1;
	}
	else {
		m_enabled = 0;
	}
}

// FUNCTION: LEMBALL 0x0043a6e0 FOLDED
void GraphicButton::OnExitButton()
{
	if (m_pressed != 0 && (m_buttonState[0] != 0 || m_buttonState[3] != 0)) {
		m_enabled = 1;
	}
	else {
		m_enabled = 0;
	}
}

// FUNCTION: LEMBALL 0x00468360 FOLDED
void GraphicButton::OnPaint(const VsRect& p_rect)
{
	int clipOk;
	ChangeList* changeList;

	clipOk = m_gdi->m_renderTarget->HasBackBuff();
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
