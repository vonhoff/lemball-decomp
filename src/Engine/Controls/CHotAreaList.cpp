#include "CHotAreaList.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Queues/PackParam.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Queues/Message.h"
#include "CHotAreaHandler.h"

#include <stddef.h>

// SIZE 0x0c
struct CHotAreaElement {
	CHotAreaHandler* m_handler;  // 0x00
	CHotAreaElement* m_previous; // 0x04
	CHotAreaElement* m_next;     // 0x08
};

class CBaseQueueHandler;

// GLOBAL: LEMBALL 0x004a1ff8
CVSPoint* g_pHotAreaCursor = NULL;

// GLOBAL: LEMBALL 0x004a1ffc
int g_nHotAreaListCount = 0;

extern CVSPoint* g_pHotAreaCursor;
extern int g_nHotAreaListCount;

// FUNCTION: LEMBALL 0x00466370
void CHotAreaList::Set(const CVSRect& p_rect, CVSPoint p_relativeTopLeft, const CVSPoint& p_innerOrigin)
{
	const short* coords;

	m_bounds.m_width = p_rect.m_width;
	m_bounds.m_height = p_rect.m_height;
	if (&p_rect != NULL) {
		coords = &p_rect.m_x;
	}
	else {
		coords = NULL;
	}
	m_bounds.m_x = coords[0];
	m_bounds.m_y = coords[1];
	m_relativeTopLeft.m_x = p_relativeTopLeft.m_x;
	m_relativeTopLeft.m_y = p_relativeTopLeft.m_y;
	m_innerOrigin.m_x = p_innerOrigin.m_x;
	m_innerOrigin.m_y = p_innerOrigin.m_y;
}

// FUNCTION: LEMBALL 0x0046a580
CHotAreaList::CHotAreaList(const CVSRect& p_rect, const CVSPoint& p_relativeTopLeft, const CVSPoint& p_innerOrigin)
	: CHotAreaHandler(p_rect)
{
	int previous;

	previous = g_nHotAreaListCount;
	g_nHotAreaListCount = g_nHotAreaListCount + 1;
	if (previous == 0) {
		g_pHotAreaCursor = new CVSPoint;
	}
	m_relativeTopLeft.m_x = p_relativeTopLeft.m_x;
	m_relativeTopLeft.m_y = p_relativeTopLeft.m_y;
	m_innerOrigin.m_x = p_innerOrigin.m_x;
	m_innerOrigin.m_y = p_innerOrigin.m_y;
	g_pMasterInputQueue->Attach(static_cast<CBaseQueueHandler*>(this), MASTER_INPUT_QUEUE_PRIORITY);
	m_tail = NULL;
	m_head = NULL;
	m_scale = 1;
	m_currentHandler = NULL;
}

// FUNCTION: LEMBALL 0x0046a650
CHotAreaList::~CHotAreaList()
{
	CHotAreaElement* entry;
	CHotAreaElement* next;

	entry = m_head;
	for (;;) {
		if (entry == NULL) {
			break;
		}
		next = entry->m_next;
		DeleteEntry(entry);
		entry = next;
	}
	g_pMasterInputQueue->Detach(static_cast<CBaseQueueHandler*>(this), MASTER_INPUT_QUEUE_PRIORITY);
	g_nHotAreaListCount = g_nHotAreaListCount - 1;
	if (g_nHotAreaListCount == 0) {
		operator delete(g_pHotAreaCursor);
	}
}

// FUNCTION: LEMBALL 0x0046a6d0
void CHotAreaList::UpdateHandlers()
{
	ProcessHandlers(*g_pHotAreaCursor, NULL);
}

// FUNCTION: LEMBALL 0x0046a6e0
void CHotAreaList::DeleteEntry(CHotAreaElement* p_entry)
{
	CHotAreaElement* next;
	CHotAreaElement* previous;

	next = p_entry->m_next;
	previous = p_entry->m_previous;
	if (previous != NULL) {
		previous->m_next = next;
	}
	else {
		m_head = next;
	}
	if (next != NULL) {
		next->m_previous = previous;
	}
	else {
		m_tail = previous;
	}
	operator delete(p_entry);
}

// FUNCTION: LEMBALL 0x0046a710
int CHotAreaList::ProcessMsg(Message* p_message)
{
	switch ((int) p_message->m_type) {
	case MESSAGE_MOUSE_BUTTON_UP:
	case MESSAGE_MOUSE_BUTTON_DOWN:
	case MESSAGE_MOUSE_MOVED:
	case MESSAGE_CURSOR_BUTTON_DOWN:
	case MESSAGE_CURSOR_BUTTON_UP:
	case MESSAGE_CURSOR_MOVED:
		if (p_message->m_source == NULL) {
			CVSPoint point((short) p_message->m_code,
						   (short) ((unsigned int) p_message->m_code >> PACK_PARAM_HIGH_WORD_SHIFT));
			CVSPoint* cursor = g_pHotAreaCursor;
			cursor->m_x = point.m_x;
			cursor->m_y = point.m_y;
			ProcessHandlers(point, p_message);
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046a770
void CHotAreaList::ProcessHandlers(const CVSPoint& p_point, Message* p_message)
{
	CHotAreaHandler* handler;
	CHotAreaElement* entry;
	CHotAreaElement* previous;
	Message fallback;
	unsigned int type;
	const CVSPoint* origin;

	fallback.m_type = MESSAGE_MOUSE_MOVED;
	fallback.m_time = 0;
	fallback.m_code = 0;
	fallback.m_payload = NULL;
	fallback.m_source = NULL;
	if (p_message == NULL) {
		p_message = &fallback;
	}
	type = p_message->m_type;
	origin = &m_bounds;
	CVSPoint localPointValue((short) ((int) (short) (p_point.m_x - origin->m_x) / (int) m_scale),
							 (short) ((int) (short) (p_point.m_y - origin->m_y) / (int) m_scale));
	CVSPoint& localPoint = localPointValue;
	entry = m_tail;
	for (;;) {
		if (entry == NULL) {
			break;
		}
		handler = entry->m_handler;
		entry = entry->m_previous;
		if (handler->m_active != 0 && handler->InArea(localPoint) == 0) {
			if (handler->m_entered != 0) {
				handler->m_entered = 0;
				handler->OnExit();
			}
			if ((type == MESSAGE_MOUSE_BUTTON_UP || type == MESSAGE_CURSOR_BUTTON_UP) && handler->m_reserved != 0) {
				handler->ProcessArea(p_message, localPoint, m_currentHandler);
			}
		}
	}
	short widthValue = m_bounds.m_width;
	short& width = widthValue;
	short heightValue = m_bounds.m_height;
	short& height = heightValue;
	const CVSPoint* boundsOrigin = &m_bounds;
	short scale = (short) m_scale;
	short xValue = (short) (m_relativeTopLeft.m_x * (scale - 1) + boundsOrigin->m_x);
	short& x = xValue;
	short yValue = (short) (m_relativeTopLeft.m_y * (scale - 1) + boundsOrigin->m_y);
	short& y = yValue;
	width = (short) (width * scale);
	height = (short) (height * scale);
	if (p_point.m_x < x || (short) (x + width) <= p_point.m_x || p_point.m_y < y ||
		(short) (height + y) <= p_point.m_y) {
		if (m_entered != 0) {
			m_entered = 0;
			OnExit();
		}
		if ((type == MESSAGE_MOUSE_BUTTON_UP || type == MESSAGE_CURSOR_BUTTON_UP) && m_reserved != 0) {
			ProcessArea(p_message, localPoint, m_currentHandler);
			m_currentHandler = this;
		}
		return;
	}
	m_entered = 1;
	entry = m_tail;
	if (m_tail != NULL) {
		do {
			handler = entry->m_handler;
			previous = entry->m_previous;
			if (handler->m_active != 0 && handler->InArea(localPoint) != 0) {
				handler->ProcessArea(p_message, localPoint, m_currentHandler);
				m_currentHandler = handler;
				break;
			}
			entry = previous;
		} while (previous != NULL);
		if (entry != NULL) {
			return;
		}
	}
	ProcessArea(p_message, localPoint, m_currentHandler);
	m_currentHandler = this;
}

// FUNCTION: LEMBALL 0x0046a9a0
void CHotAreaList::AddToList(CHotAreaHandler* p_handler)
{
	CHotAreaElement* entry;

	entry = (CHotAreaElement*) operator new(sizeof(CHotAreaElement));
	if (entry != NULL) {
		entry->m_handler = p_handler;
		entry->m_next = NULL;
		entry->m_previous = NULL;
	}
	else {
		entry = NULL;
	}
	if (m_head != NULL) {
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

// FUNCTION: LEMBALL 0x0046aa00
void CHotAreaList::RemoveFromList(CHotAreaHandler* p_handler)
{
	CHotAreaElement* entry;

	entry = m_head;
	if (entry != NULL) {
		while (entry->m_handler != p_handler) {
			entry = entry->m_next;
			if (entry == NULL) {
				return;
			}
		}
		if (m_currentHandler == p_handler) {
			m_currentHandler = NULL;
		}
		DeleteEntry(entry);
	}
}

// FUNCTION: LEMBALL 0x0046aa30
void CHotAreaList::OnExit()
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
