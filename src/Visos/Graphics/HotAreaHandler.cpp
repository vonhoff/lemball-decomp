#include "HotAreaHandler.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/MasterInput.h"
#include "../Foundation/VsPoint.h"
#include "../Foundation/VsRect.h"
#include "HotAreaList.h"

// 68K 0x1010219a OnButtonDown__15CHotAreaHandlerFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00439960
unsigned int HotAreaHandler::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	return 0;
}

// 68K 0x101021e0 OnButtonUp__15CHotAreaHandlerFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00439970
void HotAreaHandler::OnButtonUp(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x10102224 OnExternalButtonUp__15CHotAreaHandlerFRC8CVSPoint12BUTTON_FLAGS
// STUB: LEMBALL 0x00439980
void HotAreaHandler::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
}

// 68K 0x1010228a OnEnter__15CHotAreaHandlerFv
// FUNCTION: LEMBALL 0x004399a0
void HotAreaHandler::OnEnter()
{
}

// 68K 0x101022b2 OnExit__15CHotAreaHandlerFv
// STUB: LEMBALL 0x004399b0
void HotAreaHandler::OnExit()
{
}

// 68K 0x101022f2 OnInside__15CHotAreaHandlerFRC8CVSPoint
// FUNCTION: LEMBALL 0x004399d0
void HotAreaHandler::OnInside(const VsPoint& p_point)
{
}

// 68K 0x10102326 InArea__15CHotAreaHandlerFRC8CVSPoint
// STUB: LEMBALL 0x004399e0
bool HotAreaHandler::InArea(const VsPoint& p_point)
{
	short px;
	short py;
	short left;
	short top;

	px = p_point.m_x;
	left = m_x;
	if (left <= px) {
		if (px < (short) (m_width + left)) {
			py = p_point.m_y;
			top = m_y;
			if (py >= top) {
				if (py < (short) (m_height + top)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

// 68K 0x10100b76 __dt__15CHotAreaHandlerFv
HotAreaHandler::~HotAreaHandler()
{
}

// 68K 0x10211c8a __ct__15CHotAreaHandlerFRC7CVSRect
// STUB: LEMBALL 0x0046a290
HotAreaHandler::HotAreaHandler(const VsRect& p_arg0)
{
	m_height = p_arg0.m_height;
	m_width = p_arg0.m_width;
	m_y = p_arg0.m_y;
	m_x = p_arg0.m_x;
	Initialise();
}

// 68K 0x10211d28 __ct__15CHotAreaHandlerFv
// STUB: LEMBALL 0x0046a300
HotAreaHandler::HotAreaHandler()
{
	m_height = 0;
	m_width = 0;
	m_y = 0;
	m_x = 0;
	Initialise();
}

// 68K 0x10211d84 Initialise__15CHotAreaHandlerFv
// FUNCTION: LEMBALL 0x0046a330
void HotAreaHandler::Initialise()
{
	m_active = 0;
	m_externalEnabled = 0;
	m_reserved = 0;
	m_parent = 0;
	Reset();
}

// 68K 0x10211dce Reset__15CHotAreaHandlerFv
// FUNCTION: LEMBALL 0x0046a350
void HotAreaHandler::Reset()
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

// 68K 0x10211e2a ProcessArea__15CHotAreaHandlerFP10tagMESSAGERC8CVSPointP15CHotAreaHandler
// FUNCTION: LEMBALL 0x0046a380
unsigned int HotAreaHandler::ProcessArea(Message* p_message,
										 const VsPoint& p_point,
										 class HotAreaHandler* p_currentHandler)
{
	unsigned short type;
	int button;
	unsigned int payload;

	type = p_message->type;
	switch (type) {
	case 7:
		goto mouseMove;
	case 8:
	case 9:
		if (g_pMasterInput != 0 && (g_pMasterInput->m_state & 1) != 0) {
			return 0;
		}
	case 5:
	case 6:
		payload = (unsigned int) p_message->payload;
		button = 0;
		if (payload == 0x43) {
			button = 0;
		}
		else if (payload == 0x44) {
			button = 1;
		}
		else if (payload == 0x45) {
			button = 2;
		}
		else if (payload == 0x46) {
			button = 3;
		}
		else if (payload == 0x47) {
			button = 4;
		}
		else if (payload == 0x48) {
			button = 5;
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
			return 0;
		}
		if (m_x <= p_point.m_x && p_point.m_x < (short) (m_width + m_x)) {
			if (m_y <= p_point.m_y && p_point.m_y < (short) (m_height + m_y)) {
				OnButtonUp(p_point, button);
				return 0;
			}
		}
		OnExternalButtonUp(p_point, button);
		return 0;
	case 10:
		if (g_pMasterInput != 0 && (g_pMasterInput->m_state & 1) != 0) {
			return 0;
		}
		break;
	default:
		return 0;
	}
mouseMove:
	if (m_externalEnabled != 0) {
		OnInside(p_point);
	}
	if (m_entered != 0 && this == p_currentHandler) {
		return 0;
	}
	m_entered = 1;
	OnEnter();
	if (p_currentHandler == 0) {
		return 0;
	}
	if (p_currentHandler->m_entered == 0) {
		return 0;
	}
	p_currentHandler->m_entered = 0;
	p_currentHandler->OnExit();
	return 0;
}

// 68K 0x10212060 SetActive__15CHotAreaHandlerFUc
// FUNCTION: LEMBALL 0x0046a530
void HotAreaHandler::SetActive(unsigned int p_active)
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

// 68K 0x102120b6 SetParent__15CHotAreaHandlerFP12CHotAreaList
// FUNCTION: LEMBALL 0x0046a560
void HotAreaHandler::SetParent(HotAreaList* p_parent)
{
	m_parent = p_parent;
	p_parent->UpdateHandlers();
}

