#include "PvButton.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/VsTime.h"
#include "ClipRect.h"
#include "DrawingMark.h"
#include "Gdi.h"
#include "HotAreaList.h"
#include "SolidRect.h"
#include "VsGdi.h"
#include "Wnd.h"

#include <new.h>

extern int g_nGunButtonsRedrawPending;

// 68K 0x101173ca Destroy__9CPVButtonFv
// FUNCTION: LEMBALL 0x0043a540
void PvButton::Destroy()
{
	HotAreaHandler* handler;

	handler = this;
	if (this == 0) {
		handler = 0;
	}
	if (m_ownerWindow != 0 && m_ownerWindow->m_hotAreaList != 0) {
		m_ownerWindow->m_hotAreaList->RemoveFromList(handler);
	}
	Wnd::Destroy();
}

// 68K 0x10117742 GetStyle__9CPVButtonFv
// FUNCTION: LEMBALL 0x0043a570
unsigned int PvButton::GetStyle()
{
	return 2147485697;
}

// 68K 0x1011776a Move__9CPVButtonFRC8CVSPoint
// STUB: LEMBALL 0x0043a580
void PvButton::Move(const VsPoint& p_point)
{
}

// 68K 0x101177f6 OnVisibilityChange__9CPVButtonFv
// STUB: LEMBALL 0x0043a5e0
void PvButton::OnVisibilityChange()
{
}

// 68K 0x1020f994 __ct__9CPVButtonFRC7CVSRectP7CPVGWnd
// STUB: LEMBALL 0x00467c10
PvButton::PvButton(const VsRect& p_arg0, PvGWnd* p_arg1)
{
	m_x = 0;
	m_y = 0;
	m_width = p_arg0.m_width;
	m_height = p_arg0.m_height;
	m_buttonY = 0;
	m_buttonX = 0;
	m_clickY = 0;
	m_clickX = 0;
	m_buttonX = p_arg0.m_x;
	m_buttonY = p_arg0.m_y;
	m_ownerWindow = p_arg1;
	Initialise();
}

// 68K 0x1020fa78 __ct__9CPVButtonFP7CPVGWnd
// STUB: LEMBALL 0x00467cd0
PvButton::PvButton(PvGWnd* p_arg0)
{
	m_buttonY = 0;
	m_buttonX = 0;
	m_clickY = 0;
	m_clickX = 0;
	m_ownerWindow = p_arg0;
	Initialise();
}

// 68K 0x1020fb1a Initialise__9CPVButtonFv
// STUB: LEMBALL 0x00467d50
void PvButton::Initialise()
{
	void* storage;

	m_forceDrawCount = 1;
	m_autoDraw = 1;
	m_reserved = 1;
	m_pressed = 0;
	m_lastDrawnPressed = 0;
	m_drawCompleted = 0;
	storage = operator new(4);
	if (storage == 0) {
		m_primitive = 0;
	}
	else {
		m_primitive = new (storage) DrawingMark();
	}
	m_gdiFlags = 2;
	m_messageHandler = 0;
	m_controlMessage = 0;
}

// 68K 0x1020fc6e SetAutoDraw__9CPVButtonFUc
// FUNCTION: LEMBALL 0x00467e40
void PvButton::SetAutoDraw(unsigned int p_enabled)
{
	m_autoDraw = p_enabled;
}

// 68K 0x1020fc9e CheckForceDraw__9CPVButtonFv
// FUNCTION: LEMBALL 0x00467e50
void PvButton::CheckForceDraw()
{
	m_gdi->m_renderTarget->GetCurrDb();
	if (m_forceDrawCount != 0) {
		m_forceDrawCount--;
		m_clipRect.m_left = m_gdi->m_renderTarget->m_windowRect.m_width;
		m_clipRect.m_top = m_gdi->m_renderTarget->m_windowRect.m_height;
		m_clipRect.m_right = 0;
		m_clipRect.m_bottom = 0;
		m_clipRect.m_reserved0c = 0x10000;
		m_gdi->m_renderTarget->m_flag78 = 1;
	}
	else {
		m_clipRect.m_reserved0c = 0;
	}
	m_clipRect.Draw(m_gdi);
}

// 68K 0x1020fdc2 _DrawButton__9CPVButtonFv
// STUB: LEMBALL 0x00467ef0
void PvButton::_DrawButton()
{
	if (m_pressed != m_lastDrawnPressed) {
		m_gdi->m_renderTarget->m_flag78 = 1;
		m_lastDrawnPressed = m_pressed;
	}
	CheckForceDraw();
}

// 68K 0x1020fe1c Draw__9CPVButtonFUc
// FUNCTION: LEMBALL 0x00467f30
void PvButton::Draw(unsigned char p_force)
{
	unsigned int autoDraw;
	VsRect paintRect;

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

// 68K 0x1020fe92 OnEnter__9CPVButtonFv
// FUNCTION: LEMBALL 0x00467fa0
void PvButton::OnEnter()
{
	if (m_buttonState[0] != 0 || m_buttonState[3] != 0) {
		m_pressed = 1;
	}
	PvButton::OnEnterButton();
	OnEnterButton();
}

// 68K 0x1020fee4 OnExit__9CPVButtonFv
// FUNCTION: LEMBALL 0x00467fd0
void PvButton::OnExit()
{
	m_pressed = 0;
	PvButton::OnExitButton();
	OnExitButton();
	if (m_autoDraw == 0) {
		g_nGunButtonsRedrawPending = 1;
	}
}

// 68K 0x1020ff28 ConvertDoubleClick__9CPVButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00468000
int PvButton::ConvertDoubleClick(int p_flags)
{
	if ((unsigned int) p_flags > 5) {
		return 6;
	}
	return p_flags % 3;
}

// 68K 0x1020ff92 OnButtonDown__9CPVButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00468050
void PvButton::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	int converted;
	VsPoint clickPos;

	if (p_flags == 0 || p_flags == 3) {
		m_pressed = 1;
	}
	converted = ConvertDoubleClick(p_flags);
	clickPos.m_y = (short) (p_point.m_y - m_relativeTopLeft.m_y);
	clickPos.m_x = (short) (p_point.m_x - m_relativeTopLeft.m_x);
	m_clickX = clickPos.m_x;
	m_clickY = clickPos.m_y;
	PvButton::OnPressed(converted);
	OnPressed(converted);
	if (m_autoDraw == 0) {
		g_nGunButtonsRedrawPending = 1;
	}
}

// 68K 0x1021004e OnButtonUp__9CPVButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x004680c0
void PvButton::OnButtonUp(const VsPoint& p_point, int p_flags)
{
	int converted;

	if (m_pressed != 0) {
		converted = ConvertDoubleClick(p_flags);
		m_clickX = (short) (p_point.m_x - m_relativeTopLeft.m_x);
		m_clickY = (short) (p_point.m_y - m_relativeTopLeft.m_y);
		m_pressed = 0;
		PvButton::OnReleased(converted);
		OnReleased(converted);
		if (m_autoDraw == 0) {
			g_nGunButtonsRedrawPending = 1;
		}
	}
}

// 68K 0x10210100 OnExternalButtonUp__9CPVButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00468130
void PvButton::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
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

// 68K 0x1021019e _OnReleased__9CPVButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00468180
void PvButton::OnReleased(int p_flags)
{
	Message posted;
	int converted;

	if (m_autoDraw == 0) {
		m_forceDrawCount = 1;
	}
	if (m_messageHandler != 0) {
		converted = ConvertDoubleClick(p_flags);
		posted.time = CurrentQueueTimer();
		posted.code = (int) m_controlMessage;
		posted.payload = this;
		posted.type = 0xc;
		posted.source = (void*) converted;
		((BaseQueue*) m_messageHandler)->Post(posted);
	}
}

// 68K 0x10210234 _OnPressed__9CPVButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x004681f0
void PvButton::OnPressed(int p_flags)
{
	Message posted;
	int converted;

	if (m_autoDraw == 0) {
		m_forceDrawCount = 1;
	}
	if (m_messageHandler != 0) {
		converted = ConvertDoubleClick(p_flags);
		posted.type = 0xb;
		posted.time = CurrentQueueTimer();
		posted.code = (int) m_controlMessage;
		posted.payload = this;
		posted.source = (void*) converted;
		((BaseQueue*) m_messageHandler)->Post(posted);
	}
}

// 68K 0x102102ca _OnEnterButton__9CPVButtonFv
// FUNCTION: LEMBALL 0x00468260
void PvButton::OnEnterButton()
{
	Message posted;

	if (m_messageHandler != 0) {
		posted.time = CurrentQueueTimer();
		posted.code = (int) m_controlMessage;
		posted.type = 0xd;
		posted.payload = this;
		((BaseQueue*) m_messageHandler)->Post(posted);
	}
}

// 68K 0x10210330 _OnExitButton__9CPVButtonFv
// FUNCTION: LEMBALL 0x004682b0
void PvButton::OnExitButton()
{
	Message posted;

	if (m_messageHandler != 0) {
		posted.time = CurrentQueueTimer();
		posted.code = (int) m_controlMessage;
		posted.type = 0xe;
		posted.payload = this;
		((BaseQueue*) m_messageHandler)->Post(posted);
	}
}

// 68K 0x10117424 OnPaint__9CPVButtonFRC7CVSRect
// STUB: LEMBALL 0x00469870
void PvButton::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x1020fb9a __dt__9CPVButtonFv
PvButton::~PvButton()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	if (m_primitive != 0) {
		delete m_primitive;
		m_primitive = 0;
	}
}

