#include "CGraphicButton.h"

#include "../Animation/CAnim.h"
#include "../Resources/CResANIM.h"
#include "CGDI.h"
#include "CHotAreaList.h"
#include "CSurface.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Graphics/CDepressedButton.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CPVGWnd.h"
#include "Visos/Graphics/CPrimitive.h"
#include "Visos/Resources/CResZRLE.h"

class CRemap;

// GLOBAL: LEMBALL 0x0049f02c
char g_szButton[] = "Button";

// FUNCTION: LEMBALL 0x00468530
CGraphicButton::CGraphicButton(const CVsPoint& p_position,
							   CPVGWnd* p_parent,
							   unsigned long p_animId,
							   unsigned long p_alignmentFlags)
	: CDepressedButton(p_parent), m_graphicWidth(m_graphicHeight = 0), m_graphicOffsetX(m_graphicOffsetY = 0)
{
	CHotAreaHandler* area;

	m_alignmentFlags = p_alignmentFlags;
	m_animationId = p_animId;
	Initialise();
	short x = p_position.m_x;
	m_buttonX = x;
	short y = p_position.m_y;
	m_buttonY = y;
	CVsRect createRect(x, y, CHotAreaHandler::m_bounds.m_width, CHotAreaHandler::m_bounds.m_height);
	CGWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	CHotAreaHandler::m_bounds.m_x = (short) (CHotAreaHandler::m_bounds.m_x + m_relativeTopLeft.m_x);
	CHotAreaHandler::m_bounds.m_y = (short) (CHotAreaHandler::m_bounds.m_y + m_relativeTopLeft.m_y);
	area = this;
	m_ownerWindow->m_hotAreaList->AddToList(area);
}

// FUNCTION: LEMBALL 0x004686e0
void CGraphicButton::Initialise()
{
	CResZRLE* entries;
	CResANIM* animation;
	short boxWidth;
	short boxHeight;

	m_frame = 0;
	m_primitive = new CAnim[1];
	m_gdiFlags = m_gdiFlags + 1;
	m_animation = CResANIM::Load(m_animationId);
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
	CResZRLE* second = entries + 1;
	short width = second->m_width;
	short height = second->m_height;
	if ((short) m_graphicWidth < width) {
		m_graphicWidth = (unsigned short) width;
	}
	if ((short) m_graphicHeight < height) {
		m_graphicHeight = (unsigned short) height;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	const CVsPoint* position = &this->CHotAreaHandler::m_bounds;
	m_graphicOffsetX = position->m_x;
	m_graphicOffsetY = position->m_y;
	boxWidth = CHotAreaHandler::m_bounds.m_width;
	if (boxWidth < 0) {
		CHotAreaHandler::m_bounds.m_width = (short) (-(short) m_graphicWidth * boxWidth);
	}
	else if (boxWidth == 0) {
		CHotAreaHandler::m_bounds.m_width = (short) m_graphicWidth;
	}
	boxHeight = CHotAreaHandler::m_bounds.m_height;
	if (boxHeight < 0) {
		CHotAreaHandler::m_bounds.m_height = (short) (-(short) m_graphicHeight * boxHeight);
	}
	else if (boxHeight == 0) {
		CHotAreaHandler::m_bounds.m_height = (short) m_graphicHeight;
	}
	if ((int) CHotAreaHandler::m_bounds.m_width * (int) CHotAreaHandler::m_bounds.m_height != 0) {
		CHotAreaHandler::SetActive(1);
	}
	if ((m_alignmentFlags & 4) != 0) {
		m_graphicOffsetX = (short) (((int) CHotAreaHandler::m_bounds.m_width - (int) (short) m_graphicWidth) / 2);
	}
	else if ((m_alignmentFlags & 0x10) != 0) {
		m_graphicOffsetX = (short) (CHotAreaHandler::m_bounds.m_width - (short) m_graphicWidth);
	}
	if ((m_alignmentFlags & 8) != 0) {
		m_graphicOffsetY = (short) (((int) CHotAreaHandler::m_bounds.m_height - (int) (short) m_graphicHeight) / 2);
		return;
	}
	if ((m_alignmentFlags & 0x20) != 0) {
		m_graphicOffsetY = (short) (CHotAreaHandler::m_bounds.m_height - (short) m_graphicHeight);
	}
}

// FUNCTION: LEMBALL 0x004688e0
void CGraphicButton::SetAnimID(unsigned long p_animId)
{
	if (m_animation != 0) {
		m_animation->UnLoad();
	}
	m_animationId = p_animId;
	m_animation = CResANIM::Load(p_animId);
	m_forceDrawCount = 1;
}

// FUNCTION: LEMBALL 0x00468920
CGraphicButton::~CGraphicButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	delete[] m_primitive;
}

// FUNCTION: LEMBALL 0x00468980
void CGraphicButton::OnDestroy()
{
	if (m_animation != 0) {
		m_animation->UnLoad();
		m_animation = 0;
	}
}

// FUNCTION: LEMBALL 0x004689a0
void CGraphicButton::DrawButton()
{
	short x = m_graphicOffsetX;
	unsigned int pressed;
	short y = m_graphicOffsetY;

	if (m_enabled == 0 || (pressed = 1, CHotAreaHandler::m_active == 0)) {
		pressed = 0;
	}
	if ((m_alignmentFlags & 0x40) != 0 && pressed != 0) {
		x++;
		y++;
	}
	m_gdi->m_renderTarget->GetCurrDB();
	CRemap* remap = (CRemap*) m_frame;
	CResANIM* animation = m_animation;
	CAnim* primitive = (CAnim*) m_primitive;
	primitive->m_x = x;
	primitive->m_y = y;
	primitive->m_animResource = animation;
	primitive->m_animIndex = (unsigned int) (pressed >= 1);
	primitive->m_flags = 0;
	primitive->m_remap = remap;
	((CAnim*) m_primitive)->Draw(m_gdi);
}
