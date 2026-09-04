#include "HotAreaList.h"

#include "../Foundation/BaseQueue.h"
#include "HotAreaElement.h"

#include <new.h>

// GLOBAL: LEMBALL 0x004a1ff8
VsPoint* g_pHotAreaCursor = 0;

// GLOBAL: LEMBALL 0x004a1ffc
int g_nHotAreaListCount = 0;

// 68K 0x10212102 __ct__12CHotAreaListFRC7CVSRectRC8CVSPointRC8CVSPoint
// FUNCTION: LEMBALL 0x0046a580
HotAreaList::HotAreaList(const VsRect& p_arg0, const VsPoint& p_arg1, const VsPoint& p_arg2) : HotAreaHandler(p_arg0)
{
	int previous;

	m_point0.m_y = 0;
	m_point0.m_x = 0;
	m_point1.m_y = 0;
	m_point1.m_x = 0;
	previous = g_nHotAreaListCount;
	g_nHotAreaListCount = g_nHotAreaListCount + 1;
	if (previous == 0) {
		g_pHotAreaCursor = (VsPoint*) operator new(4);
		if (g_pHotAreaCursor == 0) {
			g_pHotAreaCursor = 0;
		}
		else {
			g_pHotAreaCursor->m_y = 0;
			g_pHotAreaCursor->m_x = 0;
		}
	}
	m_point0.m_x = p_arg1.m_x;
	m_point0.m_y = p_arg1.m_y;
	m_point1.m_x = p_arg2.m_x;
	m_point1.m_y = p_arg2.m_y;
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Attach(this != 0 ? static_cast<BaseQueueHandler*>(this) : 0, -0x19);
	}
	m_tail = 0;
	m_head = 0;
	m_scale = 1;
	m_currentHandler = 0;
}

// 68K 0x1021221e __dt__12CHotAreaListFv
// FUNCTION: LEMBALL 0x0046a650
HotAreaList::~HotAreaList()
{
	HotAreaElement* entry;
	HotAreaElement* next;

	entry = m_head;
	while (entry != 0) {
		next = entry->m_next;
		DeleteEntry(entry);
		entry = next;
	}
	g_pMasterInputQueue->Detach(this != 0 ? static_cast<BaseQueueHandler*>(this) : 0, -0x19);
	g_nHotAreaListCount = g_nHotAreaListCount - 1;
	if (g_nHotAreaListCount == 0) {
		operator delete(g_pHotAreaCursor);
	}
}

// 68K 0x102122f4 UpdateHandlers__12CHotAreaListFv
// FUNCTION: LEMBALL 0x0046a6d0
void HotAreaList::UpdateHandlers()
{
	ProcessHandlers(*g_pHotAreaCursor, 0);
}

// 68K 0x10212332 DeleteEntry__12CHotAreaListFP15CHotAreaElement
// FUNCTION: LEMBALL 0x0046a6e0
void HotAreaList::DeleteEntry(HotAreaElement* p_entry)
{
	HotAreaElement* next;
	HotAreaElement* previous;

	next = p_entry->m_next;
	previous = p_entry->m_previous;
	if (previous == 0) {
		m_head = next;
	}
	else {
		previous->m_next = next;
	}
	if (next == 0) {
		m_tail = previous;
	}
	else {
		next->m_previous = previous;
	}
	operator delete(p_entry);
}

// 68K 0x102123ae ProcessMsg__12CHotAreaListFP10tagMESSAGE
// FUNCTION: LEMBALL 0x0046a710
int HotAreaList::ProcessMsg(Message* p_message)
{
	VsPoint point;

	if (p_message->type > 4 && p_message->type < 0xb && p_message->source == 0) {
		point.m_x = (short) p_message->code;
		point.m_y = (short) (p_message->code >> 16);
		if (g_pHotAreaCursor != 0) {
			g_pHotAreaCursor->m_x = point.m_x;
			g_pHotAreaCursor->m_y = point.m_y;
		}
		ProcessHandlers(point, p_message);
	}
	return 0;
}

// 68K 0x10212456 ProcessHandlers__12CHotAreaListFRC8CVSPointP10tagMESSAGE
// FUNCTION: LEMBALL 0x0046a770
void HotAreaList::ProcessHandlers(const VsPoint& p_point, Message* p_message)
{
	HotAreaHandler* handler;
	HotAreaElement* entry;
	HotAreaElement* previous;
	VsPoint localPoint;
	Message fallback = {7};
	unsigned short type;
	short scaledX;
	short scaledY;
	short scaledWidth;
	short scaledHeight;

	if (p_message == 0) {
		p_message = &fallback;
	}
	type = p_message->type;
	localPoint.m_y = (short) ((int) (short) (p_point.m_y - m_y) / (int) m_scale);
	localPoint.m_x = (short) ((int) (short) (p_point.m_x - m_x) / (int) m_scale);
	entry = m_tail;
	while (entry != 0) {
		handler = entry->m_handler;
		entry = entry->m_previous;
		if (handler->m_active != 0 && handler->InArea(localPoint) == 0) {
			if (handler->m_entered != 0) {
				handler->m_entered = 0;
				handler->OnExit();
			}
			if ((type == 5 || type == 9) && handler->m_reserved != 0) {
				handler->ProcessArea(p_message, localPoint, m_currentHandler);
			}
		}
	}
	scaledX = (short) (m_point0.m_x * ((short) m_scale - 1) + m_x);
	scaledY = (short) (m_point0.m_y * ((short) m_scale - 1) + m_y);
	scaledWidth = (short) (m_width * (short) m_scale);
	scaledHeight = (short) (m_height * (short) m_scale);
	if (p_point.m_x < scaledX || (short) (scaledX + scaledWidth) <= p_point.m_x || p_point.m_y < scaledY ||
		(short) (scaledHeight + scaledY) <= p_point.m_y) {
		if (m_entered != 0) {
			m_entered = 0;
			OnExit();
		}
		if ((type == 5 || type == 9) && m_reserved != 0) {
			ProcessArea(p_message, localPoint, m_currentHandler);
			m_currentHandler = this;
		}
		return;
	}
	m_entered = 1;
	entry = m_tail;
	if (m_tail != 0) {
		do {
			handler = entry->m_handler;
			previous = entry->m_previous;
			if (handler->m_active != 0 && handler->InArea(localPoint) != 0) {
				handler->ProcessArea(p_message, localPoint, m_currentHandler);
				m_currentHandler = handler;
				previous = entry;
				break;
			}
			entry = previous;
		} while (previous != 0);
		if (previous != 0) {
			return;
		}
	}
	ProcessArea(p_message, localPoint, m_currentHandler);
	m_currentHandler = this;
}

// 68K 0x1021272e AddToList__12CHotAreaListFP15CHotAreaHandler
// FUNCTION: LEMBALL 0x0046a9a0
void HotAreaList::AddToList(HotAreaHandler* p_handler)
{
	HotAreaElement* entry;

	entry = (HotAreaElement*) operator new(sizeof(HotAreaElement));
	if (entry != 0) {
		entry->m_handler = p_handler;
		entry->m_next = 0;
		entry->m_previous = 0;
	}
	else {
		entry = 0;
	}
	if (m_head != 0) {
		m_tail->m_next = entry;
		if (m_tail == m_head) {
			m_head->m_next = entry;
		}
		entry->m_previous = m_tail;
	}
	else {
		m_head = entry;
	}
	m_tail = entry;
	p_handler->SetParent(this);
}

// 68K 0x102127d0 RemoveFromList__12CHotAreaListFP15CHotAreaHandler
// FUNCTION: LEMBALL 0x0046aa00
void HotAreaList::RemoveFromList(HotAreaHandler* p_handler)
{
	HotAreaElement* entry;

	entry = m_head;
	if (entry != 0) {
		while (entry->m_handler != p_handler) {
			entry = entry->m_next;
			if (entry == 0) {
				return;
			}
		}
		if (m_currentHandler == p_handler) {
			m_currentHandler = 0;
		}
		DeleteEntry(entry);
	}
}

// 68K 0x10117b00 OnExit__12CHotAreaListFv
// FUNCTION: LEMBALL 0x0046aa30
void HotAreaList::OnExit()
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
