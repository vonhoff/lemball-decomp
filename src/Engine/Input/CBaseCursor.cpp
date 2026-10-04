#include "Engine/Input/CBaseCursor.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Input/CMasterInput.h"
#include "Engine/Math/CVector.h"
#include "Engine/Time/VsTime.h"
#include "Engine/Queues/PackParam.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Resources/Types/CResBase.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Engine/Graphics/Primitives/CZRLE.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Queues/Message.h"
#include "Engine/Graphics/Primitives/CPushActive.h"

enum {
	CURSOR_FRAME_UNSELECTED = -1,
	CURSOR_FOCUS_FLAG_ACTIVE = 0x01
};

// GLOBAL: LEMBALL 0x004a9bec
CGWnd* g_pCursorLastWindow = NULL;

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
	g_pMasterInputQueue->Detach(this, MASTER_INPUT_QUEUE_PRIORITY);
	if (m_resource != NULL) {
		m_resource->UnLoad();
	}
	delete[] m_renderState;
}

// FUNCTION: LEMBALL 0x0046afd0
void CBaseCursor::Initialise()
{
	enum {
		CURSOR_INPUT_ACCELERATION_PER_20MS = 0x199,
		CURSOR_MAX_VELOCITY_FIXED = 0x8000
	};
	m_resourceId = 0;
	m_renderState = new CZRLE[1];
	for (int i = 0; i < 1; i++) {
		CZRLE* state = &m_renderState[i];
		state->m_x = 0;
		state->m_y = 0;
		state->m_resource = NULL;
		state->m_flags = 0;
		state->m_remap = NULL;
	}
	g_pMasterInputQueue->Attach(this, MASTER_INPUT_QUEUE_PRIORITY);
	m_changingCursor = 0;
	m_keyboardInput = 0;
	m_mouseInput = 0;
	m_active = 0;
	m_drawn = 0;
	m_systemCursorVisible = 1;
	m_resource = NULL;
	m_pushActive.m_activeMarker = 1;
	m_keys[0] = 3;
	m_keys[1] = 4;
	m_keys[2] = 1;
	m_keys[5] = 0;
	m_keys[3] = 2;
	m_keys[4] = 0x1f;
	m_keys[6] = 0x49;
	m_maxSpeed = CURSOR_INPUT_ACCELERATION_PER_20MS;
	m_acceleration = CURSOR_MAX_VELOCITY_FIXED;
	m_fixedX = (int) m_position.m_x << FIXED_POINT_FRACTION_BITS;
	m_velocityX = 0;
	m_velocityY = 0;
	m_directionX = 0;
	m_directionY = 0;
	m_fixedY = (int) m_position.m_y << FIXED_POINT_FRACTION_BITS;
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
	time = p_message->m_time;
	switch ((int) p_message->m_type) {
	default:
	done:
		return 0;
	case MESSAGE_KEY_UP:
	case MESSAGE_KEY_DOWN: {
		Message posted;
		if (m_keyboardInput == 0) {
			goto done;
		}
		now = CurrentMilliTimer();
		code = p_message->m_code;
		if (m_changingCursor == 0) {
			goto skipAction;
		}
		match = 0;
		if (m_keys[4] == code) {
			posted.m_payload = (void*) 0x43;
			match = 1;
		}
		else if (m_keys[6] == code) {
			posted.m_payload = (void*) 0x44;
			match = 1;
		}
		else if (m_keys[5] == code) {
			posted.m_payload = (void*) 0x45;
			match = 1;
		}
		if (match != 0) {
			posted.m_type = MESSAGE_CURSOR_BUTTON_DOWN;
			if (p_message->m_type != MESSAGE_KEY_DOWN) {
				posted.m_type = MESSAGE_CURSOR_BUTTON_UP;
			}
			posted.m_time = time;
			posted.m_code = PackParam(m_position.m_x, m_position.m_y);
			posted.m_source = NULL;
			g_pMasterInputQueue->Post(posted);
			return 0;
		}
	skipAction:
		if (p_message->m_type == MESSAGE_KEY_UP) {
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
	case MESSAGE_MOUSE_MOVED:
		if (m_mouseInput == 0) {
			return 0;
		}
		if (p_message->m_source == NULL) {
			CVSPoint position((short) p_message->m_code,
							  (short) ((unsigned int) p_message->m_code >> PACK_PARAM_HIGH_WORD_SHIFT));
			SetPos(position);
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046b2c0
void CBaseCursor::SetPos(const CVSPoint& p_position)
{
	m_position.m_x = p_position.m_x;
	m_position.m_y = p_position.m_y;
	m_fixedX = (int) m_position.m_x << FIXED_POINT_FRACTION_BITS;
	m_fixedY = (int) m_position.m_y << FIXED_POINT_FRACTION_BITS;
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
	m_resource = NULL;
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
		m_frame = CURSOR_FRAME_UNSELECTED;
		if (p_resourceId != 0) {
			m_resource = CResANIM::Load(p_resourceId);
		}
		else {
			m_resource = NULL;
		}
	}
	if (m_frame != p_frame && m_resource != NULL) {
		m_frame = p_frame;
		for (int i = 0; i < 1; i++) {
			m_renderState[i].m_resource = &((CResANIM*) m_resource)->m_animationEntries[m_frame];
		}
	}
}

// FUNCTION: LEMBALL 0x0046b460
bool CBaseCursor::InWindow(CGWnd* p_window)
{
	short cursorX;
	short cursorY;
	CVSRect bounds(p_window->m_rect);
	short& width = bounds.m_width;
	short& x = bounds.m_x;
	short& y = bounds.m_y;
	short& height = bounds.m_height;
	short innerWidth;
	short clipX;
	short clipY;
	short innerHeight;
	CVSPoint* innerXY;

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
	cursorX = m_position.m_x;
	if (x <= cursorX && cursorX < (short) (x + width) && y <= (cursorY = m_position.m_y) &&
		cursorY < (short) (height + y)) {
		return true;
	}
	return false;
}

inline CVSPoint operator-(const CVSPoint& p_left, const CVSPoint& p_right)
{
	return CVSPoint((short) (p_left.m_x - p_right.m_x), (short) (p_left.m_y - p_right.m_y));
}

inline CVSPoint operator/(const CVSPoint& p_point, int p_divisor)
{
	return CVSPoint((short) (p_point.m_x / p_divisor), (short) (p_point.m_y / p_divisor));
}

// FUNCTION: LEMBALL 0x0046b5c0
void CBaseCursor::Draw(CGWnd* p_window)
{
	short innerHeight;
	CVSPoint* innerXY;
	short clipX;
	short clipY;
	int zoom;
	CGDI* gdi;
	CSurface* surface;

	if ((m_mouseInput == 0 || (g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) == 0) &&
		(m_keyboardInput == 0 || (g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) == 0)) {
		return;
	}
	if ((g_cursorFocusFlag & CURSOR_FOCUS_FLAG_ACTIVE) == 0) {
		g_cursorFocusFlag = (unsigned char) (g_cursorFocusFlag | CURSOR_FOCUS_FLAG_ACTIVE);
		g_pCursorLastWindow = p_window;
	}
	if (p_window->IsFocusWindow() == 0) {
		if (p_window == g_pCursorLastWindow) {
			RestoreSystemCursor();
			g_pCursorLastWindow = NULL;
		}
	}
	else {
		g_pCursorLastWindow = p_window;
	}
	if (m_active == 0) {
		return;
	}
	CVSRect bounds(p_window->m_rect);
	short& width = bounds.m_width;
	short& height = bounds.m_height;
	short& x = bounds.m_x;
	short& y = bounds.m_y;
	{
		CVSSize innerSize(p_window->m_innerRect);
		short& innerWidth = innerSize.m_width;
		innerHeight = innerSize.m_height;
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
				clipX = (short) (clipX - x);
				clipX = (short) (clipX + innerWidth);
				width = clipX;
			}
			if (y < clipY) {
				height = (short) (height + (y - clipY));
				y = clipY;
			}
			if ((short) (clipY + innerHeight) < (short) (height + y)) {
				innerHeight = (short) (innerHeight - y);
				innerHeight = (short) (innerHeight + clipY);
				height = innerHeight;
			}
			if (width <= 0 || height <= 0) {
				height = 0;
				width = 0;
				y = 0;
				x = 0;
			}
		}
	}
	if (InWindow(p_window) == 0) {
		return;
	}
	if (m_systemCursorVisible != 0) {
		KillSystemCursor();
	}
	m_drawn = 1;
	if (m_resource == NULL) {
		return;
	}
	zoom = (int) p_window->m_zoom;
	{
		CVSPoint destination = (m_position - bounds) / zoom - m_hotspot;
		gdi = p_window->m_gdi;
		surface = gdi->m_renderTarget;
		surface->GetChangeList();
		surface->GetCurrDB();
		CZRLE* state = m_renderState;
		state->m_x = destination.m_x;
		state->m_y = destination.m_y;
		surface->GetCurrDB();
		m_renderState->Draw(gdi);
	}
}

// FUNCTION: LEMBALL 0x0046b810
void CBaseCursor::Process()
{
	enum {
		CURSOR_ACCELERATION_INTERVAL_MS = 20
	};
	unsigned long now;
	short boundRight;
	short boundBottom;

	if (m_drawn == 0 && m_systemCursorVisible == 0) {
		RestoreSystemCursor();
	}
	if ((m_mouseInput == 0 || (g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) == 0) &&
		(m_keyboardInput == 0 || (g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) == 0)) {
		return;
	}
	if (m_active == 0) {
		return;
	}
	now = CurrentMilliTimer();
	if (m_directionX != 0) {
		m_velocityX += (int) (m_directionX * (now - m_lastInputX)) / CURSOR_ACCELERATION_INTERVAL_MS;
		m_lastInputX = now;
		if (m_velocityX > m_acceleration) {
			m_velocityX = m_acceleration;
		}
		if (m_velocityX < -m_acceleration) {
			m_velocityX = -m_acceleration;
		}
	}
	if (m_directionY != 0) {
		m_velocityY += (int) ((now - m_lastInputY) * m_directionY) / CURSOR_ACCELERATION_INTERVAL_MS;
		m_lastInputY = now;
		if (m_velocityY > m_acceleration) {
			m_velocityY = m_acceleration;
		}
		if (m_velocityY < -m_acceleration) {
			m_velocityY = -m_acceleration;
		}
	}
	CVSPoint oldPosition(m_position);
	int x = m_velocityX + m_fixedX;
	int y = m_velocityY + m_fixedY;
	m_fixedX = x;
	m_fixedY = y;
	m_position.m_x = (short) (x >> FIXED_POINT_FRACTION_BITS);
	m_position.m_y = (short) (y >> FIXED_POINT_FRACTION_BITS);
	if (m_keyboardInput != 0 && !m_position.Equals(oldPosition)) {
		Message posted;
		posted.m_type = MESSAGE_CURSOR_MOVED;
		posted.m_time = CurrentQueueTimer();
		posted.m_code = PackParam(m_position.m_x, m_position.m_y);
		posted.m_payload = NULL;
		posted.m_source = NULL;
		g_pMasterInputQueue->Post(posted);
	}
	if ((int) m_bounds.m_width * (int) m_bounds.m_height != 0) {
		if (m_position.m_x < m_bounds.m_x || (short) (m_bounds.m_width + m_bounds.m_x) <= m_position.m_x ||
			m_bounds.m_y > m_position.m_y || (short) (m_bounds.m_y + m_bounds.m_height) <= m_position.m_y) {
			CVSPoint* minimum = &m_bounds;
			if (m_position.m_x < minimum->m_x) {
				m_position.m_x = minimum->m_x;
			}
			if (m_position.m_y < minimum->m_y) {
				m_position.m_y = minimum->m_y;
			}
			boundRight = (short) (m_bounds.m_width + m_bounds.m_x - 1);
			boundBottom = (short) (m_bounds.m_y + m_bounds.m_height - 1);
			if (boundRight < m_position.m_x) {
				m_position.m_x = boundRight;
			}
			if (boundBottom < m_position.m_y) {
				m_position.m_y = boundBottom;
			}
			CVector clipped((long) m_position.m_x, (long) m_position.m_y);
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
