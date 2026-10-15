#include "CGraphicButton.h"

#include <stddef.h>

#include "CDepressedButton.h"
#include "CGWnd.h"
#include "CPVGWnd.h"
#include "Engine/Animation/CAnim.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Input/CHotAreaList.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Platform/Windows/Graphics/CSurface.h"

enum {
	GRAPHIC_BUTTON_ALIGN_HORIZONTAL_CENTER = 0x04,
	GRAPHIC_BUTTON_ALIGN_VERTICAL_CENTER = 0x08,
	GRAPHIC_BUTTON_ALIGN_RIGHT = 0x10,
	GRAPHIC_BUTTON_ALIGN_BOTTOM = 0x20,
	GRAPHIC_BUTTON_OFFSET_ON_PRESS = 0x40
};

class CRemap;

// GLOBAL: LEMBALL 0x0049f02c
char g_szButton[] = "Button";

// FUNCTION: LEMBALL 0x00468530
CGraphicButton::CGraphicButton(const CVSPoint& p_position,
							   CPVGWnd* p_parent,
							   unsigned long p_animId,
							   unsigned long p_alignmentFlags)
	: CDepressedButton(p_parent)
{
	CHotAreaHandler* area;

	m_alignmentFlags = p_alignmentFlags;
	m_animationId = p_animId;
	Initialise();
	short x = p_position.m_x;
	m_buttonPosition.m_x = x;
	short y = p_position.m_y;
	m_buttonPosition.m_y = y;
	CVSRect createRect(x, y, m_bounds.m_width, m_bounds.m_height);
	CGWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	m_bounds.m_x = (short) (m_bounds.m_x + m_relativeTopLeft.m_x);
	m_bounds.m_y = (short) (m_bounds.m_y + m_relativeTopLeft.m_y);
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

	m_remap = NULL;
	m_primitive = new CAnim[1];
	m_gdiFlags = m_gdiFlags + 1;
	m_animation = CResANIM::Load(m_animationId);
	animation = m_animation;
	animation->Lock();
	entries = m_animation->m_animationEntries;
	short& graphicHeight = m_graphicSize.m_height;
	short firstWidth = entries->m_size.m_width;
	CResZRLE* second = entries + 1;
	m_graphicSize.m_width = firstWidth;
	graphicHeight = entries->m_size.m_height;
	short width = second->m_size.m_width;
	short height = second->m_size.m_height;
	if (firstWidth < width) {
		m_graphicSize.m_width = width;
	}
	if (m_graphicSize.m_height < height) {
		graphicHeight = height;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	const CVSPoint* position = &m_bounds;
	m_graphicOffset.m_x = position->m_x;
	m_graphicOffset.m_y = position->m_y;
	boxWidth = m_bounds.m_width;
	if (boxWidth < 0) {
		m_bounds.m_width = -m_graphicSize.m_width * boxWidth;
	}
	else if (boxWidth == 0) {
		m_bounds.m_width = m_graphicSize.m_width;
	}
	boxHeight = m_bounds.m_height;
	if (boxHeight < 0) {
		m_bounds.m_height = -m_graphicSize.m_height * boxHeight;
	}
	else if (boxHeight == 0) {
		m_bounds.m_height = m_graphicSize.m_height;
	}
	if ((int) m_bounds.m_width * (int) m_bounds.m_height != 0) {
		CHotAreaHandler::SetActive(1);
	}
	short& offsetX = m_graphicOffset.m_x;
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_HORIZONTAL_CENTER) != 0) {
		offsetX = (m_bounds.m_width - m_graphicSize.m_width) / 2;
	}
	else if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_RIGHT) != 0) {
		offsetX = m_bounds.m_width - m_graphicSize.m_width;
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_VERTICAL_CENTER) != 0) {
		m_graphicOffset.m_y = (m_bounds.m_height - m_graphicSize.m_height) / 2;
		return;
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_BOTTOM) != 0) {
		m_graphicOffset.m_y = m_bounds.m_height - m_graphicSize.m_height;
	}
}

// FUNCTION: LEMBALL 0x004688e0
void CGraphicButton::SetAnimID(unsigned long p_animId)
{
	if (m_animation != NULL) {
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
	if (m_animation != NULL) {
		m_animation->UnLoad();
		m_animation = NULL;
	}
}

// FUNCTION: LEMBALL 0x004689a0
void CGraphicButton::DrawButton()
{
	short x = m_graphicOffset.m_x;
	unsigned int pressed;
	short y = m_graphicOffset.m_y;

	if (m_enabled == 0 || (pressed = 1, CHotAreaHandler::m_active == 0)) {
		pressed = 0;
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_OFFSET_ON_PRESS) != 0 && pressed != 0) {
		x++;
		y++;
	}
	m_gdi->m_renderTarget->GetCurrDB();
	CRemap* remap = m_remap;
	CResANIM* animation = m_animation;
	CAnim* primitive = m_primitive;
	primitive->m_x = x;
	primitive->m_y = y;
	primitive->m_animResource = animation;
	primitive->m_animIndex = (unsigned int) (pressed >= 1);
	primitive->m_flags = 0;
	primitive->m_remap = remap;
	m_primitive->Draw(m_gdi);
}
