#include "GraphicButton.h"

#include "../Animation/Anim.h"
#include "../Foundation/ChangeList.h"
#include "../Resources/ResAnim.h"
#include "Gdi.h"
#include "HotAreaList.h"
#include "PushActive.h"
#include "VsGdi.h"

// GLOBAL: LEMBALL 0x0049f02c
char g_szButton[] = "Button";

// 68K 0x10210510 __ct__14CGraphicButtonFRC8CVSPointP7CPVGWndUlUl
// FUNCTION: LEMBALL 0x00468530
GraphicButton::GraphicButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, unsigned long p_arg3)
	: DepressedButton(p_arg1), m_graphicWidth(m_graphicHeight = 0), m_graphicOffsetX(m_graphicOffsetY = 0)
{
	HotAreaHandler* area;

	m_alignmentFlags = p_arg3;
	m_animationId = p_arg2;
	Initialise();
	short x = p_arg0.m_x;
	m_buttonX = x;
	short y = p_arg0.m_y;
	m_buttonY = y;
	VsRect createRect(x, y, HotAreaHandler::m_bounds.m_width, HotAreaHandler::m_bounds.m_height);
	GWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	HotAreaHandler::m_bounds.m_x = (short) (HotAreaHandler::m_bounds.m_x + m_relativeTopLeft.m_x);
	HotAreaHandler::m_bounds.m_y = (short) (HotAreaHandler::m_bounds.m_y + m_relativeTopLeft.m_y);
	area = this;
	m_ownerWindow->m_hotAreaList->AddToList(area);
}

// 68K 0x1021068c Initialise__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x004686e0
void GraphicButton::Initialise()
{
	ResZrle* entries;
	ResAnim* animation;
	short boxWidth;
	short boxHeight;

	m_frame = 0;
	m_primitive = new Anim[1];
	m_gdiFlags = m_gdiFlags + 1;
	m_animation = ResAnim::Load(m_animationId);
	animation = m_animation;
	if (animation->m_loaded != 0) {
		animation->m_age = 0;
	}
	else {
		animation->LoadData();
	}
	animation->m_directUseCount++;
	entries = m_animation->m_animationEntries;
	m_graphicWidth = (unsigned short) entries->m_width;
	m_graphicHeight = (unsigned short) entries->m_height;
	ResZrle* second = entries + 1;
	short width = second->m_width;
	short height = second->m_height;
	if ((short) m_graphicWidth < width) {
		m_graphicWidth = (unsigned short) width;
	}
	if ((short) m_graphicHeight < height) {
		m_graphicHeight = (unsigned short) height;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	const VsPoint* position = &this->HotAreaHandler::m_bounds;
	m_graphicOffsetX = position->m_x;
	m_graphicOffsetY = position->m_y;
	boxWidth = HotAreaHandler::m_bounds.m_width;
	if (boxWidth < 0) {
		HotAreaHandler::m_bounds.m_width = (short) (-(short) m_graphicWidth * boxWidth);
	}
	else if (boxWidth == 0) {
		HotAreaHandler::m_bounds.m_width = (short) m_graphicWidth;
	}
	boxHeight = HotAreaHandler::m_bounds.m_height;
	if (boxHeight < 0) {
		HotAreaHandler::m_bounds.m_height = (short) (-(short) m_graphicHeight * boxHeight);
	}
	else if (boxHeight == 0) {
		HotAreaHandler::m_bounds.m_height = (short) m_graphicHeight;
	}
	if ((int) HotAreaHandler::m_bounds.m_width * (int) HotAreaHandler::m_bounds.m_height != 0) {
		HotAreaHandler::SetActive(1);
	}
	if ((m_alignmentFlags & 4) != 0) {
		m_graphicOffsetX = (short) (((int) HotAreaHandler::m_bounds.m_width - (int) (short) m_graphicWidth) / 2);
	}
	else if ((m_alignmentFlags & 0x10) != 0) {
		m_graphicOffsetX = (short) (HotAreaHandler::m_bounds.m_width - (short) m_graphicWidth);
	}
	if ((m_alignmentFlags & 8) != 0) {
		m_graphicOffsetY = (short) (((int) HotAreaHandler::m_bounds.m_height - (int) (short) m_graphicHeight) / 2);
		return;
	}
	if ((m_alignmentFlags & 0x20) != 0) {
		m_graphicOffsetY = (short) (HotAreaHandler::m_bounds.m_height - (short) m_graphicHeight);
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

// 68K 0x10210966 __dt__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x00468920
GraphicButton::~GraphicButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	delete[] m_primitive;
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

// 68K 0x10210a70 DrawButton__14CGraphicButtonFv
// FUNCTION: LEMBALL 0x004689a0
void GraphicButton::DrawButton()
{
	short x = m_graphicOffsetX;
	unsigned int pressed;
	short y = m_graphicOffsetY;

	if (m_enabled == 0 || (pressed = 1, HotAreaHandler::m_active == 0)) {
		pressed = 0;
	}
	if ((m_alignmentFlags & 0x40) != 0 && pressed != 0) {
		x++;
		y++;
	}
	m_gdi->m_renderTarget->GetCurrDb();
	Remap* remap = (Remap*) m_frame;
	ResAnim* animation = m_animation;
	Anim* primitive = (Anim*) m_primitive;
	primitive->m_x = x;
	primitive->m_y = y;
	primitive->m_animResource = animation;
	primitive->m_animIndex = (unsigned int) (pressed >= 1);
	primitive->m_flags = 0;
	primitive->m_remap = remap;
	((Anim*) m_primitive)->Draw(m_gdi);
}
