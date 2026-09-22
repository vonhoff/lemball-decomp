#include "CBaseCursor.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CMasterInput.h"
#include "../Foundation/CVector.h"
#include "../Foundation/VsTime.h"
#include "../Messaging/PackParam.h"
#include "../Resources/CResANIM.h"
#include "../Resources/CResBase.h"
#include "../Resources/CResZRLE.h"
#include "CGDI.h"
#include "CGWnd.h"
#include "CSurface.h"
#include "CZRLE.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CPushActive.h"

// GLOBAL: LEMBALL 0x004a9bec
CGWnd* g_pCursorLastWindow = 0;

// GLOBAL: LEMBALL 0x004a9bf0
unsigned char g_cursorFocusFlag = 0;

// FUNCTION: LEMBALL 0x0046aec0
CBaseCursor::CBaseCursor()
	: m_maxSpeed((int) 0xaa55aa55), m_acceleration((int) 0xaa55aa55), m_fixedX((int) 0xaa55aa55),
	  m_fixedY((int) 0xaa55aa55), m_velocityX((int) 0xaa55aa55), m_velocityY((int) 0xaa55aa55),
	  m_directionX((int) 0xaa55aa55), m_directionY((int) 0xaa55aa55)
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0046af30
CBaseCursor::~CBaseCursor()
{
	g_pMasterInputQueue->Detach(this, -0x19);
	if (m_resource != 0) {
		m_resource->UnLoad();
	}
	delete[] m_renderState;
}

// FUNCTION: LEMBALL 0x0046afd0
void CBaseCursor::Initialise()
{
	m_resourceId = 0;
	m_renderState = new CZRLE[1];
	for (int i = 0; i < 1; i++) {
		CZRLE* state = &m_renderState[i];
		state->m_x = 0;
		state->m_y = 0;
		state->m_resource = 0;
		state->m_flags = 0;
		state->m_remap = 0;
	}
	g_pMasterInputQueue->Attach(this, -0x19);
	m_changingCursor = 0;
	m_keyboardInput = 0;
	m_mouseInput = 0;
	m_active = 0;
	m_drawn = 0;
	m_systemCursorVisible = 1;
	m_resource = 0;
	m_pushActive.m_activeMarker = 1;
	m_keys[0] = 3;
	m_keys[1] = 4;
	m_keys[2] = 1;
	m_keys[5] = 0;
	m_keys[3] = 2;
	m_keys[4] = 0x1f;
	m_keys[6] = 0x49;
	m_maxSpeed = 0x199;
	m_acceleration = 0x8000;
	m_fixedX = (int) m_position.m_x << 12;
	m_velocityX = 0;
	m_velocityY = 0;
	m_directionX = 0;
	m_directionY = 0;
	m_fixedY = (int) m_position.m_y << 12;
	m_lastInputX = CurrentMilliTimer();
	m_lastInputY = m_lastInputX;
}

// FUNCTION: LEMBALL 0x0046b0e0
int CBaseCursor::ProcessMsg(Message* p_message)
{
	int code;
	unsigned long now;
	unsigned long time;
	int match;

	if (m_active == 0) {
		goto done;
	}
	time = p_message->time;
	switch ((int) p_message->type) {
	default:
	done:
		return 0;
	case 3:
	case 4: {
		Message posted;
		if (m_keyboardInput == 0) {
			goto done;
		}
		now = CurrentMilliTimer();
		code = p_message->code;
		if (m_changingCursor == 0) {
			goto skipAction;
		}
		match = 0;
		if (m_keys[4] == code) {
			posted.payload = (void*) 0x43;
			match = 1;
		}
		else if (m_keys[6] == code) {
			posted.payload = (void*) 0x44;
			match = 1;
		}
		else if (m_keys[5] == code) {
			posted.payload = (void*) 0x45;
			match = 1;
		}
		if (match != 0) {
			posted.type = 8;
			if (p_message->type != 4) {
				posted.type = 9;
			}
			posted.time = time;
			posted.code = PackParam(m_position.m_x, m_position.m_y);
			posted.source = 0;
			g_pMasterInputQueue->Post(posted);
			return 0;
		}
	skipAction:
		if (p_message->type == 3) {
			if (m_keys[2] == code || m_keys[3] == code) {
				m_velocityY = 0;
				m_directionY = 0;
				break;
			}
			if (m_keys[0] == code || m_keys[1] == code) {
				m_velocityX = 0;
				m_directionX = 0;
				break;
			}
		}
		else {
			if (m_keys[2] != code) {
				if (m_keys[3] != code) {
					if (m_keys[0] != code) {
						if (m_keys[1] == code) {
							m_directionX = m_maxSpeed;
							m_lastInputX = now;
							return 0;
						}
					}
					else {
						m_directionX = -m_maxSpeed;
						m_lastInputX = now;
						return 0;
					}
				}
				else {
					m_directionY = m_maxSpeed;
					m_lastInputY = now;
					return 0;
				}
			}
			else {
				m_directionY = -m_maxSpeed;
				m_lastInputY = now;
			}
		}
		return 0;
	}
	case 7:
		if (m_mouseInput == 0) {
			return 0;
		}
		if (p_message->source == 0) {
			CVsPoint position((short) p_message->code, (short) ((unsigned int) p_message->code >> 16));
			SetPos(position);
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046b2c0
void CBaseCursor::SetPos(const CVsPoint& p_position)
{
	m_position.m_x = p_position.m_x;
	m_position.m_y = p_position.m_y;
	m_fixedX = (int) m_position.m_x << 12;
	m_fixedY = (int) m_position.m_y << 12;
}

// FUNCTION: LEMBALL 0x0046b310
void CBaseCursor::SetMainID(unsigned int p_resourceId)
{
	if (p_resourceId == m_resourceId) {
		return;
	}
	if (m_resourceId != 0) {
		m_resource->UnLoad();
	}
	m_resourceId = p_resourceId;
	if (p_resourceId != 0) {
		m_resource = CResZRLE::Load(p_resourceId);
		for (int i = 0; i < 1; i++) {
			m_renderState[i].m_resource = m_resource;
		}
		return;
	}
	m_resource = 0;
}

// FUNCTION: LEMBALL 0x0046b370
void CBaseCursor::SetActive(unsigned int p_active)
{
	if (m_active == p_active) {
		return;
	}
	m_active = p_active;
	if (p_active == 0) {
		RestoreSystemCursor();
		return;
	}
	m_velocityX = 0;
	m_velocityY = 0;
	m_directionX = 0;
	m_directionY = 0;
	m_lastInputX = CurrentMilliTimer();
	m_lastInputY = m_lastInputX;
}

// FUNCTION: LEMBALL 0x0046b3b0
void CBaseCursor::SetMainID(unsigned int p_resourceId, int p_frame)
{
	if (p_resourceId != m_resourceId) {
		if (m_resourceId != 0) {
			m_resource->UnLoad();
		}
		m_resourceId = p_resourceId;
		m_frame = -1;
		if (p_resourceId != 0) {
			m_resource = CResANIM::Load(p_resourceId);
		}
		else {
			m_resource = 0;
		}
	}
	if (m_frame != p_frame && m_resource != 0) {
		m_frame = p_frame;
		for (int i = 0; i < 1; i++) {
			m_renderState[i].m_resource = &((CResANIM*) m_resource)->m_animationEntries[m_frame];
		}
	}
}

// FUNCTION: LEMBALL 0x0046b460
bool CBaseCursor::InWindow(CGWnd* p_window)
{
	CVsRect bounds(p_window->m_rect);
	short& width = bounds.m_width;
	short& height = bounds.m_height;
	short& x = bounds.m_x;
	short& y = bounds.m_y;
	short innerWidth;
	short innerHeight;
	short clipX;
	short clipY;
	CVsPoint* innerXY;

	innerHeight = p_window->m_innerRect.m_height;
	innerWidth = p_window->m_innerRect.m_width;
	innerXY = &p_window->m_innerRect;
	clipX = innerXY->m_x;
	clipY = innerXY->m_y;
	if ((int) innerHeight * (int) innerWidth != 0) {
		clipX = (short) (clipX + x);
		clipY = (short) (clipY + y);
		if (x < clipX) {
			width = (short) (width + (x - clipX));
			x = clipX;
		}
		if ((short) (innerWidth + clipX) < (short) (x + width)) {
			width = (short) ((clipX - x) + innerWidth);
		}
		if (y < clipY) {
			height = (short) (height + (y - clipY));
			y = clipY;
		}
		if ((short) (clipY + innerHeight) < (short) (height + y)) {
			height = (short) ((innerHeight - y) + clipY);
		}
		if (width <= 0 || height <= 0) {
			height = 0;
			width = 0;
			y = 0;
			x = 0;
		}
	}
	if (x <= m_position.m_x && m_position.m_x < (short) (x + width) && y <= m_position.m_y &&
		m_position.m_y < (short) (height + y)) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046b5c0
void CBaseCursor::Draw(CGWnd* p_window)
{
	short innerWidth;
	short innerHeight;
	CVsPoint* innerXY;
	short clipX;
	short clipY;
	int zoom;
	CGDI* gdi;
	CSurface* surface;

	if ((m_mouseInput == 0 || (g_pMasterInput->m_state & 1) == 0) &&
		(m_keyboardInput == 0 || (g_pMasterInput->m_state & 6) == 0)) {
		return;
	}
	if ((g_cursorFocusFlag & 1) == 0) {
		g_cursorFocusFlag = (unsigned char) (g_cursorFocusFlag | 1);
		g_pCursorLastWindow = p_window;
	}
	if (p_window->IsFocusWindow() == 0) {
		if (p_window == g_pCursorLastWindow) {
			RestoreSystemCursor();
			g_pCursorLastWindow = 0;
		}
	}
	else {
		g_pCursorLastWindow = p_window;
	}
	if (m_active == 0) {
		return;
	}
	CVsRect bounds(p_window->m_rect);
	short& width = bounds.m_width;
	short& height = bounds.m_height;
	short& x = bounds.m_x;
	short& y = bounds.m_y;
	innerWidth = p_window->m_innerRect.m_width;
	innerHeight = p_window->m_innerRect.m_height;
	innerXY = &p_window->m_innerRect;
	clipX = innerXY->m_x;
	clipY = innerXY->m_y;
	if ((int) innerHeight * (int) innerWidth != 0) {
		clipX = (short) (clipX + x);
		clipY = (short) (clipY + y);
		if (x < clipX) {
			width = (short) (width + (x - clipX));
			x = clipX;
		}
		if ((short) (innerWidth + clipX) < (short) (x + width)) {
			width = (short) ((clipX - x) + innerWidth);
		}
		if (y < clipY) {
			height = (short) (height + (y - clipY));
			y = clipY;
		}
		if ((short) (clipY + innerHeight) < (short) (height + y)) {
			height = (short) ((innerHeight - y) + clipY);
		}
		if (width <= 0 || height <= 0) {
			height = 0;
			width = 0;
			y = 0;
			x = 0;
		}
	}
	if (InWindow(p_window) == 0) {
		return;
	}
	if (m_systemCursorVisible != 0) {
		KillSystemCursor();
	}
	m_drawn = 1;
	if (m_resource == 0) {
		return;
	}
	zoom = (int) p_window->m_zoom;
	CVsPoint destination;
	destination.m_x = (short) ((int) (short) (m_position.m_x - x) / zoom) - m_hotspot.m_x;
	gdi = p_window->m_gdi;
	destination.m_y = (short) ((int) (short) (m_position.m_y - y) / zoom) - m_hotspot.m_y;
	surface = gdi->m_renderTarget;
	surface->GetChangeList();
	surface->GetCurrDB();
	CZRLE* state = m_renderState;
	state->m_x = destination.m_x;
	state->m_y = destination.m_y;
	surface->GetCurrDB();
	m_renderState->Draw(gdi);
}

// FUNCTION: LEMBALL 0x0046b810
void CBaseCursor::Process()
{
	unsigned long now;
	int speed;
	int cap;
	short oldX;
	short oldY;
	short boundX;
	short boundY;
	short boundRight;
	short boundBottom;
	Message posted;
	CVector clipped;

	if (m_drawn == 0 && m_systemCursorVisible == 0) {
		RestoreSystemCursor();
	}
	if ((m_mouseInput == 0 || (g_pMasterInput->m_state & 1) == 0) &&
		(m_keyboardInput == 0 || (g_pMasterInput->m_state & 6) == 0)) {
		return;
	}
	if (m_active == 0) {
		return;
	}
	now = CurrentMilliTimer();
	if (m_directionX != 0) {
		speed = m_velocityX + (int) (m_directionX * (now - m_lastInputX)) / 0x14;
		m_lastInputX = now;
		cap = m_acceleration;
		m_velocityX = speed;
		if (cap < speed) {
			m_velocityX = cap;
		}
		if (m_velocityX < -cap) {
			m_velocityX = -cap;
		}
	}
	if (m_directionY != 0) {
		speed = m_velocityY + (int) ((now - m_lastInputY) * m_directionY) / 0x14;
		m_lastInputY = now;
		cap = m_acceleration;
		m_velocityY = speed;
		if (cap < speed) {
			m_velocityY = cap;
		}
		if (m_velocityY < -cap) {
			m_velocityY = -cap;
		}
	}
	oldX = m_position.m_x;
	oldY = m_position.m_y;
	m_fixedX = m_velocityX + m_fixedX;
	m_fixedY = m_velocityY + m_fixedY;
	m_position.m_x = (short) (m_fixedX >> 12);
	m_position.m_y = (short) (m_fixedY >> 12);
	if (m_keyboardInput != 0 && (m_position.m_x != oldX || m_position.m_y != oldY)) {
		posted.type = 10;
		posted.time = CurrentQueueTimer();
		posted.code = PackParam(m_position.m_x, m_position.m_y);
		posted.payload = 0;
		posted.source = 0;
		g_pMasterInputQueue->Post(posted);
	}
	if ((int) m_bounds.m_width * (int) m_bounds.m_height != 0) {
		boundX = m_bounds.m_x;
		boundY = m_bounds.m_y;
		if (boundX > m_position.m_x || (short) (m_bounds.m_width + boundX) <= m_position.m_x ||
			boundY > m_position.m_y || (short) (boundY + m_bounds.m_height) <= m_position.m_y) {
			if (m_position.m_x < boundX) {
				m_position.m_x = boundX;
			}
			if (m_position.m_y < boundY) {
				m_position.m_y = boundY;
			}
			boundRight = (short) (m_bounds.m_width + boundX - 1);
			boundBottom = (short) (m_bounds.m_y + m_bounds.m_height - 1);
			if (boundRight < m_position.m_x) {
				m_position.m_x = boundRight;
			}
			if (boundBottom < m_position.m_y) {
				m_position.m_y = boundBottom;
			}
			clipped.SetIntegers((int) m_position.m_x, (int) m_position.m_y);
			m_fixedX = clipped.m_xFixed;
			m_fixedY = clipped.m_yFixed;
		}
	}
	m_drawn = 0;
}

// FUNCTION: LEMBALL 0x0046ba20
void CBaseCursor::RefreshPos()
{
}
