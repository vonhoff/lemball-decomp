#include "CPVButton.h"

#include "../Foundation/CBaseQueue.h"
#include "CClipRect.h"
#include "CDrawingMark.h"
#include "CGDI.h"
#include "CHotAreaList.h"
#include "CSurface.h"
#include "CWnd.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CPVGWnd.h"
#include "Visos/Graphics/CPVWnd.h"
#include "Visos/Graphics/CPrimitive.h"

#include <stddef.h>

extern int g_nGunButtonsRedrawPending;
extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x0043a540
void CPVButton::Destroy()
{
	m_ownerWindow->m_hotAreaList->RemoveFromList(this);
	CWnd::Destroy();
}

#define CPV_BUTTON_INIT_HOT_AREA_LIST_STYLE 0x800
// FUNCTION: LEMBALL 0x0043a570
unsigned int CPVButton::GetStyle()
{
	return (unsigned int) (WINDOW_STYLE_DIRECT_SCROLL | CPV_BUTTON_INIT_HOT_AREA_LIST_STYLE |
						   WINDOW_STYLE_SHOW_ON_CREATE);
}
#undef CPV_BUTTON_INIT_HOT_AREA_LIST_STYLE

// FUNCTION: LEMBALL 0x0043a580
void CPVButton::Move(const CVSPoint& p_point)
{
	m_forceDrawCount = 1;
	CHotAreaHandler::m_bounds.m_x -= m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y -= m_relativeTopLeft.m_y;
	CGWnd::Move(p_point);
	CHotAreaHandler::m_bounds.m_x += m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y += m_relativeTopLeft.m_y;
}

// FUNCTION: LEMBALL 0x0043a5e0
void CPVButton::OnVisibilityChange()
{
	if (CPVWnd::m_parent != NULL) {
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
	m_forceDrawCount = 1;
	CHotAreaHandler::SetActive(CPVWnd::m_active);
}

// FUNCTION: LEMBALL 0x00467c10
CPVButton::CPVButton(const CVSRect& p_bounds, CPVGWnd* p_ownerWindow)
	: CHotAreaHandler(CVSRect(0, 0, p_bounds.m_width, p_bounds.m_height)), m_buttonX(m_buttonY = 0)
{
	m_clickY = 0;
	m_clickX = 0;
	const CVSRect* rect = &p_bounds;
	const short* position;
	if (rect != NULL) {
		position = &rect->m_x;
	}
	else {
		position = NULL;
	}
	m_buttonX = *position;
	m_buttonY = position[1];
	m_ownerWindow = p_ownerWindow;
	Initialise();
}

// FUNCTION: LEMBALL 0x00467cd0
CPVButton::CPVButton(CPVGWnd* p_ownerWindow) : m_buttonX(m_buttonY = 0)
{
	m_clickY = 0;
	m_clickX = 0;
	m_ownerWindow = p_ownerWindow;
	Initialise();
}

// FUNCTION: LEMBALL 0x00467d50
void CPVButton::Initialise()
{
	m_forceDrawCount = 1;
	m_autoDraw = 1;
	m_reserved = 1;
	m_pressed = 0;
	m_lastDrawnPressed = 0;
	m_drawCompleted = 0;
	m_primitive = new CDrawingMark();
	m_gdiFlags = 2;
	m_messageQueue = NULL;
	m_controlMessage = 0;
}

// FUNCTION: LEMBALL 0x00467dd0
CPVButton::~CPVButton()
{
	if (m_ownerWindow->m_lifecycleRefs == 1) {
		Destroy();
	}
	if (m_primitive != NULL) {
		delete m_primitive;
	}
}

// FUNCTION: LEMBALL 0x00467e40
void CPVButton::SetAutoDraw(unsigned int p_enabled)
{
	m_autoDraw = p_enabled;
}

// FUNCTION: LEMBALL 0x00467e50
void CPVButton::CheckForceDraw()
{
	m_gdi->m_renderTarget->GetCurrDB();
	if (m_forceDrawCount != 0) {
		m_forceDrawCount--;
		m_clipRect[0].m_bounds.m_width = m_gdi->m_renderTarget->m_windowRect.m_width;
		m_clipRect[0].m_bounds.m_height = m_gdi->m_renderTarget->m_windowRect.m_height;
		m_clipRect[0].m_bounds.m_x = 0;
		m_clipRect[0].m_bounds.m_y = 0;
		m_clipRect[0].m_flags = CClipRect::CLIP_IGNORE_PARENT;
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
	else {
		m_clipRect[0].m_flags = 0;
	}
	m_clipRect[0].Draw(m_gdi);
}

// FUNCTION: LEMBALL 0x00467ef0
void CPVButton::_DrawButton()
{
	if (m_pressed != m_lastDrawnPressed) {
		m_gdi->m_renderTarget->m_flag78 = 1;
		m_lastDrawnPressed = m_pressed;
	}
	CheckForceDraw();
}

// FUNCTION: LEMBALL 0x00467f30
void CPVButton::Draw(unsigned int p_force)
{
	unsigned int autoDraw;
	CVSRect paintRect;

	if (m_drawCompleted == 0 || p_force != 0) {
		autoDraw = m_autoDraw;
		m_autoDraw = 1;
		paintRect.m_width = m_rect.m_width;
		paintRect.m_height = m_rect.m_height;
		paintRect.m_x = 0;
		paintRect.m_y = 0;
		OnPaint(paintRect);
		m_autoDraw = autoDraw;
	}
	m_drawCompleted = 0;
}

// FUNCTION: LEMBALL 0x00467fa0
void CPVButton::OnEnter()
{
	if (m_buttonState[MOUSE_BUTTON_INDEX_LEFT] != 0 || m_buttonState[MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK] != 0) {
		m_pressed = 1;
	}
	_OnEnterButton();
	OnEnterButton();
}

// FUNCTION: LEMBALL 0x00467fd0
void CPVButton::OnExit()
{
	m_pressed = 0;
	_OnExitButton();
	OnExitButton();
}

// FUNCTION: LEMBALL 0x00468000
int CPVButton::ConvertDoubleClick(int p_flags)
{
	switch (p_flags) {
	case MOUSE_BUTTON_INDEX_LEFT:
	case MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK:
		return MOUSE_BUTTON_INDEX_LEFT;
	case MOUSE_BUTTON_INDEX_RIGHT:
	case MOUSE_BUTTON_INDEX_RIGHT_DOUBLE_CLICK:
		return MOUSE_BUTTON_INDEX_RIGHT;
	case MOUSE_BUTTON_INDEX_MIDDLE:
	case MOUSE_BUTTON_INDEX_MIDDLE_DOUBLE_CLICK:
		return MOUSE_BUTTON_INDEX_MIDDLE;
	default:
		return MOUSE_BUTTON_INDEX_UNSUPPORTED;
	}
}

// FUNCTION: LEMBALL 0x00468050
void CPVButton::OnButtonDown(const CVSPoint& p_point, int p_flags)
{
	int converted;
	CVSPoint clickPos;

	if (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK) {
		m_pressed = 1;
	}
	converted = ConvertDoubleClick(p_flags);
	clickPos.m_y = (short) (p_point.m_y - m_relativeTopLeft.m_y);
	clickPos.m_x = (short) (p_point.m_x - m_relativeTopLeft.m_x);
	m_clickX = clickPos.m_x;
	m_clickY = clickPos.m_y;
	_OnPressed(converted);
	OnPressed(converted);
}

// FUNCTION: LEMBALL 0x004680c0
void CPVButton::OnButtonUp(const CVSPoint& p_point, int p_flags)
{
	int converted;

	if (m_pressed != 0) {
		converted = ConvertDoubleClick(p_flags);
		CVSPoint clickPos;
		clickPos.m_y = (short) (p_point.m_y - m_relativeTopLeft.m_y);
		clickPos.m_x = (short) (p_point.m_x - m_relativeTopLeft.m_x);
		m_clickX = clickPos.m_x;
		m_clickY = clickPos.m_y;
		m_pressed = 0;
		_OnReleased(converted);
		OnReleased(converted);
	}
}

// FUNCTION: LEMBALL 0x00468130
void CPVButton::OnExternalButtonUp(const CVSPoint& p_point, int p_flags)
{
	int i;
	unsigned int* state;

	CVSPoint relativeValue(m_relativeTopLeft);
	const CVSPoint& relative = relativeValue;
	CVSPoint originValue(m_relativeTopLeft);
	const CVSPoint& origin = originValue;
	relativeValue.m_y = (short) (p_point.m_y - origin.m_y);
	relativeValue.m_x = (short) (p_point.m_x - origin.m_x);
	m_clickX = relative.m_x;
	m_clickY = relative.m_y;
	state = m_buttonState;
	i = 6;
	while (i != 0) {
		*state = 0;
		++state;
		--i;
	}
}

// FUNCTION: LEMBALL 0x00468180
void CPVButton::_OnReleased(int p_flags)
{
	Message posted;
	int converted;

	if (m_autoDraw == 0) {
		m_forceDrawCount = 1;
	}
	if (m_messageQueue != NULL) {
		converted = ConvertDoubleClick(p_flags);
		posted.m_time = timeGetTime();
		posted.m_code = (int) m_controlMessage;
		posted.m_payload = this;
		posted.m_type = MESSAGE_BUTTON_RELEASED;
		posted.m_source = (void*) converted;
		m_messageQueue->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x004681f0
void CPVButton::_OnPressed(int p_flags)
{
	Message posted;
	int converted;

	if (m_autoDraw == 0) {
		m_forceDrawCount = 1;
	}
	if (m_messageQueue != NULL) {
		converted = ConvertDoubleClick(p_flags);
		posted.m_time = timeGetTime();
		posted.m_code = (int) m_controlMessage;
		posted.m_payload = this;
		posted.m_type = MESSAGE_BUTTON_PRESSED;
		posted.m_source = (void*) converted;
		m_messageQueue->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x00468260
void CPVButton::_OnEnterButton()
{
	Message posted;

	if (m_messageQueue != NULL) {
		posted.m_time = timeGetTime();
		posted.m_code = (int) m_controlMessage;
		posted.m_type = MESSAGE_BUTTON_ENTERED;
		posted.m_payload = this;
		m_messageQueue->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x004682b0
void CPVButton::_OnExitButton()
{
	Message posted;

	if (m_messageQueue != NULL) {
		posted.m_time = timeGetTime();
		posted.m_code = (int) m_controlMessage;
		posted.m_type = MESSAGE_BUTTON_EXITED;
		posted.m_payload = this;
		m_messageQueue->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x00469870
void CPVButton::OnPaint(const CVSRect& p_rect)
{
}
