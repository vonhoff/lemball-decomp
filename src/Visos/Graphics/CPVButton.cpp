#include "CPVButton.h"

#include "../Foundation/CBaseQueue.h"
#include "CDrawingMark.h"
#include "CGDI.h"
#include "CHotAreaList.h"
#include "CSolidRect.h"
#include "CSurface.h"
#include "CWnd.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CPVGWnd.h"
#include "Visos/Graphics/CPVWnd.h"
#include "Visos/Graphics/CPrimitive.h"

extern int g_nGunButtonsRedrawPending;
extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x0043a540
void CPVButton::Destroy()
{
	m_ownerWindow->m_hotAreaList->RemoveFromList(this);
	CWnd::Destroy();
}

// FUNCTION: LEMBALL 0x0043a570
unsigned int CPVButton::GetStyle()
{
	return 2147485697;
}

// FUNCTION: LEMBALL 0x0043a580
void CPVButton::Move(const CVsPoint& p_point)
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
	if (CPVWnd::m_parent != 0) {
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
	m_forceDrawCount = 1;
	CHotAreaHandler::SetActive(CPVWnd::m_active);
}

// FUNCTION: LEMBALL 0x00467c10
CPVButton::CPVButton(const CVsRect& p_arg0, CPVGWnd* p_arg1)
	: CHotAreaHandler(CVsRect(0, 0, p_arg0.m_width, p_arg0.m_height)), m_buttonX(m_buttonY = 0)
{
	m_clickY = 0;
	m_clickX = 0;
	const CVsRect* rect = &p_arg0;
	const short* position;
	if (rect != 0) {
		position = &rect->m_x;
	}
	else {
		position = 0;
	}
	m_buttonX = *position;
	m_buttonY = position[1];
	m_ownerWindow = p_arg1;
	Initialise();
}

// FUNCTION: LEMBALL 0x00467cd0
CPVButton::CPVButton(CPVGWnd* p_arg0) : m_buttonX(m_buttonY = 0)
{
	m_clickY = 0;
	m_clickX = 0;
	m_ownerWindow = p_arg0;
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
	m_messageHandler = 0;
	m_controlMessage = 0;
}

// FUNCTION: LEMBALL 0x00467dd0
CPVButton::~CPVButton()
{
	if (m_ownerWindow->m_lifecycleRefs == 1) {
		Destroy();
	}
	if (m_primitive != 0) {
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
		m_clipRect[0].m_color = 0x10000;
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
	else {
		m_clipRect[0].m_color = 0;
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
	CVsRect paintRect;

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
	if (m_buttonState[0] != 0 || m_buttonState[3] != 0) {
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
	case 0:
	case 3:
		return 0;
	case 1:
	case 4:
		return 1;
	case 2:
	case 5:
		return 2;
	default:
		return 6;
	}
}

// FUNCTION: LEMBALL 0x00468050
void CPVButton::OnButtonDown(const CVsPoint& p_point, int p_flags)
{
	int converted;
	CVsPoint clickPos;

	if (p_flags == 0 || p_flags == 3) {
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
void CPVButton::OnButtonUp(const CVsPoint& p_point, int p_flags)
{
	int converted;

	if (m_pressed != 0) {
		converted = ConvertDoubleClick(p_flags);
		CVsPoint clickPos;
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
void CPVButton::OnExternalButtonUp(const CVsPoint& p_point, int p_flags)
{
	int i;
	unsigned int* state;

	m_clickX = (short) (p_point.m_x - m_relativeTopLeft.m_x);
	m_clickY = (short) (p_point.m_y - m_relativeTopLeft.m_y);
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
	if (m_messageHandler != 0) {
		converted = ConvertDoubleClick(p_flags);
		posted.time = timeGetTime();
		posted.code = (int) m_controlMessage;
		posted.payload = this;
		posted.type = 0xc;
		posted.source = (void*) converted;
		((CBaseQueue*) m_messageHandler)->Post(posted);
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
	if (m_messageHandler != 0) {
		converted = ConvertDoubleClick(p_flags);
		posted.time = timeGetTime();
		posted.code = (int) m_controlMessage;
		posted.payload = this;
		posted.type = 0xb;
		posted.source = (void*) converted;
		((CBaseQueue*) m_messageHandler)->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x00468260
void CPVButton::_OnEnterButton()
{
	Message posted;

	if (m_messageHandler != 0) {
		posted.time = timeGetTime();
		posted.code = (int) m_controlMessage;
		posted.type = 0xd;
		posted.payload = this;
		((CBaseQueue*) m_messageHandler)->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x004682b0
void CPVButton::_OnExitButton()
{
	Message posted;

	if (m_messageHandler != 0) {
		posted.time = timeGetTime();
		posted.code = (int) m_controlMessage;
		posted.type = 0xe;
		posted.payload = this;
		((CBaseQueue*) m_messageHandler)->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x00469870
void CPVButton::OnPaint(const CVsRect& p_rect)
{
}
