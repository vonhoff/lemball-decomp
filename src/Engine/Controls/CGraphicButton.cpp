#include "CGraphicButton.h"

#include "Engine/Animation/CAnim.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "CHotAreaList.h"
#include "Engine/Graphics/Surfaces/CSurface.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "CDepressedButton.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "CHotAreaHandler.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"
#include "Engine/Graphics/Primitives/CPrimitive.h"
#include "Engine/Resources/Types/CResZRLE.h"

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
	CVSRect createRect(x, y, CHotAreaHandler::m_bounds.m_width, CHotAreaHandler::m_bounds.m_height);
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
	unsigned short& graphicHeight = m_graphicHeight;
	short firstWidth = entries->m_width;
	CResZRLE* second = entries + 1;
	m_graphicWidth = (unsigned short) firstWidth;
	graphicHeight = (unsigned short) entries->m_height;
	short width = second->m_width;
	short height = second->m_height;
	if (firstWidth < width) {
		m_graphicWidth = (unsigned short) width;
	}
	if ((short) m_graphicHeight < height) {
		graphicHeight = (unsigned short) height;
	}
	m_animation->m_directUseCount = m_animation->m_directUseCount - 1;
	const CVSPoint* position = &this->CHotAreaHandler::m_bounds;
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
	short& offsetX = m_graphicOffsetX;
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_HORIZONTAL_CENTER) != 0) {
		offsetX = (short) (((int) CHotAreaHandler::m_bounds.m_width - (int) (short) m_graphicWidth) / 2);
	}
	else if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_RIGHT) != 0) {
		offsetX = (short) (CHotAreaHandler::m_bounds.m_width - (short) m_graphicWidth);
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_VERTICAL_CENTER) != 0) {
		m_graphicOffsetY = (short) (((int) CHotAreaHandler::m_bounds.m_height - (int) (short) m_graphicHeight) / 2);
		return;
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_ALIGN_BOTTOM) != 0) {
		m_graphicOffsetY = (short) (CHotAreaHandler::m_bounds.m_height - (short) m_graphicHeight);
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
	short x = m_graphicOffsetX;
	unsigned int pressed;
	short y = m_graphicOffsetY;

	if (m_enabled == 0 || (pressed = 1, CHotAreaHandler::m_active == 0)) {
		pressed = 0;
	}
	if ((m_alignmentFlags & GRAPHIC_BUTTON_OFFSET_ON_PRESS) != 0 && pressed != 0) {
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
