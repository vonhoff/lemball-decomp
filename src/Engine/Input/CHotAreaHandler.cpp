#include "CHotAreaHandler.h"

#include "CMasterInput.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "CHotAreaList.h"
#include "Engine/Queues/Message.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00439960
void CHotAreaHandler::OnButtonDown(const CVSPoint& p_point, int p_flags)
{
}

// FUNCTION: LEMBALL 0x00439970
void CHotAreaHandler::OnButtonUp(const CVSPoint& p_point, int p_flags)
{
}

// FUNCTION: LEMBALL 0x00439980
void CHotAreaHandler::OnExternalButtonUp(const CVSPoint& p_point, int p_flags)
{
	m_buttonState[p_flags + 3] = 0;
	m_buttonState[p_flags] = 0;
}

// FUNCTION: LEMBALL 0x004399a0
void CHotAreaHandler::OnEnter()
{
}

// FUNCTION: LEMBALL 0x004399b0
void CHotAreaHandler::OnExit()
{
	int i;
	unsigned int* state;

	if (m_reserved == 0) {
		state = m_buttonState;
		i = 6;
		while (i != 0) {
			*state = 0;
			++state;
			--i;
		}
	}
}

// FUNCTION: LEMBALL 0x004399d0
void CHotAreaHandler::OnInside(const CVSPoint& p_point)
{
}

// FUNCTION: LEMBALL 0x004399e0
bool CHotAreaHandler::InArea(const CVSPoint& p_point)
{
	short px;
	short top;
	short left;
	short py;

	left = m_bounds.m_x;
	px = p_point.m_x;
	if (left <= px) {
		if (px < (short) (m_bounds.m_width + left)) {
			py = p_point.m_y;
			top = m_bounds.m_y;
			if (py >= top) {
				if (py < (short) (m_bounds.m_height + top)) {
					return true;
				}
			}
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0046a290
CHotAreaHandler::CHotAreaHandler(const CVSRect& p_bounds)
{
	Initialise();
	m_bounds.m_width = p_bounds.m_width;
	m_bounds.m_height = p_bounds.m_height;
	const CVSRect* rect = &p_bounds;
	const short* position;
	if (rect != NULL) {
		position = &rect->m_x;
	}
	else {
		position = NULL;
	}
	m_bounds.m_x = *position;
	m_bounds.m_y = position[1];
	SetActive(1);
}

// FUNCTION: LEMBALL 0x0046a300
CHotAreaHandler::CHotAreaHandler()
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0046a330
void CHotAreaHandler::Initialise()
{
	m_active = 0;
	m_externalEnabled = 0;
	m_reserved = 0;
	m_parent = NULL;
	Reset();
}

// FUNCTION: LEMBALL 0x0046a350
void CHotAreaHandler::Reset()
{
	int i;
	unsigned int* state;

	if (m_entered != 0) {
		m_entered = 0;
		OnExit();
	}
	state = m_buttonState;
	i = 6;
	while (i != 0) {
		*state = 0;
		++state;
		--i;
	}
}

// FUNCTION: LEMBALL 0x0046a380
void CHotAreaHandler::ProcessArea(Message* p_message, const CVSPoint& p_point, class CHotAreaHandler* p_currentHandler)
{
	unsigned short type;
	int button;
	unsigned int payload;

	type = p_message->m_type;
	switch (type) {
	case MESSAGE_CURSOR_BUTTON_DOWN:
	case MESSAGE_CURSOR_BUTTON_UP:
		if ((g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) != 0) {
			return;
		}
	case MESSAGE_MOUSE_BUTTON_UP:
	case MESSAGE_MOUSE_BUTTON_DOWN:
		payload = (unsigned int) p_message->m_payload;
		switch (payload) {
		case INPUT_MOUSE_LEFT:
			button = MOUSE_BUTTON_INDEX_LEFT;
			break;
		case INPUT_MOUSE_RIGHT:
			button = MOUSE_BUTTON_INDEX_RIGHT;
			break;
		case INPUT_MOUSE_MIDDLE:
			button = MOUSE_BUTTON_INDEX_MIDDLE;
			break;
		case INPUT_MOUSE_LEFT_DOUBLE_CLICK:
			button = MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK;
			break;
		case INPUT_MOUSE_RIGHT_DOUBLE_CLICK:
			button = MOUSE_BUTTON_INDEX_RIGHT_DOUBLE_CLICK;
			break;
		case INPUT_MOUSE_MIDDLE_DOUBLE_CLICK:
			button = MOUSE_BUTTON_INDEX_MIDDLE_DOUBLE_CLICK;
			break;
		}
		if (type == MESSAGE_MOUSE_BUTTON_DOWN || type == MESSAGE_CURSOR_BUTTON_DOWN) {
			m_buttonState[button] = 1;
		}
		else {
			m_buttonState[button + 3] = 0;
			m_buttonState[button] = 0;
		}
		if (p_message->m_type != MESSAGE_MOUSE_BUTTON_UP && p_message->m_type != MESSAGE_CURSOR_BUTTON_UP) {
			OnButtonDown(p_point, button);
			return;
		}
		if (m_bounds.m_x <= p_point.m_x && p_point.m_x < (short) (m_bounds.m_width + m_bounds.m_x)) {
			short top = m_bounds.m_y;
			short y = p_point.m_y;
			if (top <= y && y < (short) (m_bounds.m_height + top)) {
				OnButtonUp(p_point, button);
				return;
			}
		}
		OnExternalButtonUp(p_point, button);
		return;
	case MESSAGE_CURSOR_MOVED:
		if ((g_pMasterInput->m_state & MASTER_INPUT_ACTIVE_STATE_MASK) != 0) {
			return;
		}
	case MESSAGE_MOUSE_MOVED:
		if (m_externalEnabled != 0) {
			OnInside(p_point);
		}
		if (m_entered != 0 && this == p_currentHandler) {
			return;
		}
		break;
	default:
		return;
	}
	m_entered = 1;
	OnEnter();
	if (p_currentHandler == NULL) {
		return;
	}
	if (p_currentHandler->m_entered == 0) {
		return;
	}
	p_currentHandler->m_entered = 0;
	p_currentHandler->OnExit();
	return;
}

// FUNCTION: LEMBALL 0x0046a530
void CHotAreaHandler::SetActive(unsigned int p_active)
{
	m_active = p_active;
	if (p_active == 0) {
		Reset();
		return;
	}
	if (m_parent != NULL) {
		m_parent->UpdateHandlers();
	}
}

// FUNCTION: LEMBALL 0x0046a560
void CHotAreaHandler::SetParent(CHotAreaList* p_parent)
{
	m_parent = p_parent;
	p_parent->UpdateHandlers();
}
