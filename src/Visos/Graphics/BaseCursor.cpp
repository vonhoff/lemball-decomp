#include "BaseCursor.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/MasterInput.h"
#include "../Foundation/Vector.h"
#include "../Foundation/VsTime.h"
#include "../Messaging/PackParam.h"
#include "../Resources/ResAnim.h"
#include "../Resources/ResBase.h"
#include "../Resources/ResZrle.h"
#include "Gdi.h"
#include "GWnd.h"
#include "VsGdi.h"
#include "Zrle.h"

// GLOBAL: LEMBALL 0x004a9bec
GWnd* g_pCursorLastWindow = 0;

// GLOBAL: LEMBALL 0x004a9bf0
unsigned char g_cursorFocusFlag = 0;

// 68K 0x10205a9a __ct__11CBaseCursorFv
// FUNCTION: LEMBALL 0x0046aec0
BaseCursor::BaseCursor()
{
	m_position.m_y = 0;
	m_position.m_x = 0;
	m_hotspot.m_y = 0;
	m_hotspot.m_x = 0;
	m_pushActive.m_activeMarker = 0;
	m_maxSpeed = (int) 0xaa55aa55;
	m_acceleration = (int) 0xaa55aa55;
	m_fixedX = (int) 0xaa55aa55;
	m_fixedY = (int) 0xaa55aa55;
	m_velocityX = (int) 0xaa55aa55;
	m_bounds.m_height = 0;
	m_velocityY = (int) 0xaa55aa55;
	m_bounds.m_width = 0;
	m_directionX = (int) 0xaa55aa55;
	m_bounds.m_y = 0;
	m_directionY = (int) 0xaa55aa55;
	m_bounds.m_x = 0;
	Initialise();
}

// 68K 0x10205c5c Initialise__11CBaseCursorFv
// FUNCTION: LEMBALL 0x0046afd0
void BaseCursor::Initialise()
{
	m_resourceId = 0;
	m_renderState = new Zrle[1];
	m_renderState[0].m_x = 0;
	m_renderState[0].m_y = 0;
	m_renderState[0].m_resource = 0;
	m_renderState[0].m_flags = 0;
	m_renderState[0].m_remap = 0;
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

// 68K 0x10205de2 ProcessMsg__11CBaseCursorFP10tagMESSAGE
// FUNCTION: LEMBALL 0x0046b0e0
int BaseCursor::ProcessMsg(Message* p_message)
{
	unsigned short type;
	int code;
	unsigned long now;
	VsPoint position;
	Message posted;
	int action;
	int match;

	if (m_active == 0) {
		return 0;
	}
	type = p_message->type;
	if (type < 3) {
		return 0;
	}
	if (4 < type) {
		if (type != 7) {
			return 0;
		}
		if (m_mouseInput != 0) {
			if (p_message->payload == 0) {
				position.m_x = (short) p_message->code;
				position.m_y = (short) (p_message->code >> 16);
				SetPos(position);
			}
			return 0;
		}
		return 0;
	}
	if (m_keyboardInput == 0) {
		return 0;
	}
	now = CurrentMilliTimer();
	code = p_message->code;
	if (m_changingCursor == 0) {
		goto skipAction;
	}
	match = 0;
	action = 0;
	if (m_keys[4] == code) {
		action = 0x43;
		match = 1;
	}
	else if (m_keys[6] == code) {
		action = 0x44;
		match = 1;
	}
	else if (m_keys[5] == code) {
		action = 0x45;
		match = 1;
	}
	if (match != 0) {
		posted.type = 8;
		if (p_message->type != 4) {
			posted.type = 9;
		}
		posted.time = p_message->time;
		posted.code = PackParam(m_position.m_x, m_position.m_y);
		posted.payload = (void*) action;
		posted.source = 0;
		g_pMasterInputQueue->Post(posted);
		return 0;
	}
skipAction:
	if (p_message->type == 3) {
		if (m_keys[2] == code || m_keys[3] == code) {
			m_velocityY = 0;
			m_directionY = 0;
			return 0;
		}
		if (m_keys[0] == code || m_keys[1] == code) {
			m_velocityX = 0;
			m_directionX = 0;
			return 0;
		}
	}
	else if (m_keys[2] == code) {
		m_lastInputY = now;
		m_directionY = -m_maxSpeed;
	}
	else {
		if (m_keys[3] == code) {
			m_directionY = m_maxSpeed;
			m_lastInputY = now;
			return 0;
		}
		if (m_keys[0] == code) {
			m_directionX = -m_maxSpeed;
			m_lastInputX = now;
			return 0;
		}
		if (m_keys[1] == code) {
			m_directionX = m_maxSpeed;
			m_lastInputX = now;
			return 0;
		}
	}
	return 0;
}

// 68K 0x10206040 SetPos__11CBaseCursorFRC8CVSPoint
// FUNCTION: LEMBALL 0x0046b2c0
void BaseCursor::SetPos(const VsPoint& p_position)
{
	m_position.m_x = p_position.m_x;
	m_position.m_y = p_position.m_y;
	m_fixedX = (int) p_position.m_x << 12;
	m_fixedY = (int) p_position.m_y << 12;
}

// 68K 0x102060aa SetMainID__11CBaseCursorFUl
// FUNCTION: LEMBALL 0x0046b310
void BaseCursor::SetMainId(unsigned int p_resourceId)
{
	if (p_resourceId == m_resourceId) {
		return;
	}
	if (m_resourceId != 0) {
		m_resource->UnLoad();
	}
	m_resourceId = p_resourceId;
	if (p_resourceId != 0) {
		m_resource = ResZrle::Load(p_resourceId);
		m_renderState[0].m_resource = m_resource;
		return;
	}
	m_resource = 0;
}

// 68K 0x10206134 SetActive__11CBaseCursorFUc
// FUNCTION: LEMBALL 0x0046b370
void BaseCursor::SetActive(unsigned char p_active)
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

// 68K 0x102061ca SetMainID__11CBaseCursorFUli
// FUNCTION: LEMBALL 0x0046b3b0
void BaseCursor::SetMainId(unsigned int p_resourceId, int p_frame)
{
	ResZrle* entries;

	if (p_resourceId != m_resourceId) {
		if (m_resourceId != 0) {
			m_resource->UnLoad();
		}
		m_resourceId = p_resourceId;
		m_frame = -1;
		if (p_resourceId != 0) {
			m_resource = ResAnim::Load(p_resourceId);
		}
		else {
			m_resource = 0;
		}
	}
	if (m_frame == p_frame) {
		return;
	}
	if (m_resource == 0) {
		return;
	}
	m_frame = p_frame;
	entries = ((ResAnim*) m_resource)->m_animationEntries;
	m_renderState[0].m_resource = &entries[m_frame];
}

// 68K 0x10206282 InWindow__11CBaseCursorFP5CGWnd
// FUNCTION: LEMBALL 0x0046b460
bool BaseCursor::InWindow(GWnd* p_window)
{
	short width;
	short height;
	short x;
	short y;
	short innerWidth;
	short innerHeight;
	short clipX;
	short clipY;

	width = p_window->m_rect.m_width;
	height = p_window->m_rect.m_height;
	x = p_window->m_rect.m_x;
	y = p_window->m_rect.m_y;
	innerWidth = p_window->m_innerRect.m_width;
	innerHeight = p_window->m_innerRect.m_height;
	if ((int) innerHeight * (int) innerWidth != 0) {
		clipX = (short) (p_window->m_innerRect.m_x + x);
		clipY = (short) (p_window->m_innerRect.m_y + y);
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
		if (width < 1 || height < 1) {
			height = 0;
			width = 0;
			y = 0;
			x = 0;
		}
	}
	if (x > m_position.m_x) {
		return 0;
	}
	if ((short) (x + width) <= m_position.m_x) {
		return 0;
	}
	if (y > m_position.m_y) {
		return 0;
	}
	if ((short) (height + y) <= m_position.m_y) {
		return 0;
	}
	return 1;
}

// 68K 0x1020646c Draw__11CBaseCursorFP5CGWnd
// FUNCTION: LEMBALL 0x0046b5c0
void BaseCursor::Draw(GWnd* p_window)
{
	short width;
	short height;
	short x;
	short y;
	short innerWidth;
	short innerHeight;
	VsPoint* innerXY;
	short clipX;
	short clipY;
	short destX;
	short destY;
	int zoom;
	Gdi* gdi;
	Surface* surface;

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
	width = p_window->m_rect.m_width;
	height = p_window->m_rect.m_height;
	if (p_window != (GWnd*) -8) {
		x = p_window->m_rect.m_x;
		y = p_window->m_rect.m_y;
	}
	else {
		x = ((VsPoint*) 0)->m_x;
		y = ((VsPoint*) 0)->m_y;
	}
	innerWidth = p_window->m_innerRect.m_width;
	innerHeight = p_window->m_innerRect.m_height;
	if (p_window != (GWnd*) -16) {
		innerXY = (VsPoint*) &p_window->m_innerRect.m_x;
	}
	else {
		innerXY = 0;
	}
	if ((int) innerHeight * (int) innerWidth != 0) {
		clipX = (short) (innerXY->m_x + x);
		clipY = (short) (innerXY->m_y + y);
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
		if (width < 1 || height < 1) {
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
	destX = (short) ((int) (short) (m_position.m_x - x) / zoom) - m_hotspot.m_x;
	gdi = p_window->m_gdi;
	destY = (short) ((int) (short) (m_position.m_y - y) / zoom) - m_hotspot.m_y;
	surface = gdi->m_renderTarget;
	surface->GetChangeList();
	surface->GetCurrDb();
	m_renderState->m_x = destX;
	m_renderState->m_y = destY;
	surface->GetCurrDb();
	m_renderState->Draw(gdi);
}

// 68K 0x102067b8 Process__11CBaseCursorFv
// FUNCTION: LEMBALL 0x0046b810
void BaseCursor::Process()
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
	Vector clipped;

	if (m_drawn == 0 && m_systemCursorVisible == 0) {
		RefreshPos();
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

// 68K 0x10106076 RefreshPos__11CBaseCursorFv
// FUNCTION: LEMBALL 0x0046ba20
void BaseCursor::RefreshPos()
{
}

// 68K 0x10205b80 __dt__11CBaseCursorFv
BaseCursor::~BaseCursor()
{
}
