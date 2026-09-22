#include "CHotAreaHandler.h"

#include "../Foundation/CMasterInput.h"
#include "../Foundation/CVsPoint.h"
#include "../Foundation/CVsRect.h"
#include "CHotAreaList.h"
#include "Visos/Foundation/Message.h"

// FUNCTION: LEMBALL 0x00439960
void CHotAreaHandler::OnButtonDown(const CVsPoint& p_point, int p_flags)
{
}

// FUNCTION: LEMBALL 0x00439970
void CHotAreaHandler::OnButtonUp(const CVsPoint& p_point, int p_flags)
{
}

// FUNCTION: LEMBALL 0x00439980
void CHotAreaHandler::OnExternalButtonUp(const CVsPoint& p_point, int p_flags)
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
void CHotAreaHandler::OnInside(const CVsPoint& p_point)
{
}

// FUNCTION: LEMBALL 0x004399e0
bool CHotAreaHandler::InArea(const CVsPoint& p_point)
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
					return 1;
				}
			}
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046a290
CHotAreaHandler::CHotAreaHandler(const CVsRect& p_arg0)
{
	Initialise();
	m_bounds.m_width = p_arg0.m_width;
	m_bounds.m_height = p_arg0.m_height;
	const CVsRect* rect = &p_arg0;
	const short* position;
	if (rect != 0) {
		position = &rect->m_x;
	}
	else {
		position = 0;
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
	m_parent = 0;
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
void CHotAreaHandler::ProcessArea(Message* p_message, const CVsPoint& p_point, class CHotAreaHandler* p_currentHandler)
{
	unsigned short type;
	int button;
	unsigned int payload;

	type = p_message->type;
	switch (type) {
	case 8:
	case 9:
		if ((g_pMasterInput->m_state & 1) != 0) {
			return;
		}
	case 5:
	case 6:
		payload = (unsigned int) p_message->payload;
		switch (payload) {
		case 0x43:
			button = 0;
			break;
		case 0x44:
			button = 1;
			break;
		case 0x45:
			button = 2;
			break;
		case 0x46:
			button = 3;
			break;
		case 0x47:
			button = 4;
			break;
		case 0x48:
			button = 5;
			break;
		}
		if (type == 6 || type == 8) {
			m_buttonState[button] = 1;
		}
		else {
			m_buttonState[button + 3] = 0;
			m_buttonState[button] = 0;
		}
		if (p_message->type != 5 && p_message->type != 9) {
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
	case 10:
		if ((g_pMasterInput->m_state & 1) != 0) {
			return;
		}
	case 7:
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
	if (p_currentHandler == 0) {
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
	if (m_parent != 0) {
		m_parent->UpdateHandlers();
	}
}

// FUNCTION: LEMBALL 0x0046a560
void CHotAreaHandler::SetParent(CHotAreaList* p_parent)
{
	m_parent = p_parent;
	p_parent->UpdateHandlers();
}
