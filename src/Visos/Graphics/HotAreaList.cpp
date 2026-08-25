#include "HotAreaList.h"

#include "HotAreaElement.h"

#include <new.h>

// 68K 0x10212102 __ct__12CHotAreaListFRC7CVSRectRC8CVSPointRC8CVSPoint
// STUB: LEMBALL 0x0046a580
HotAreaList::HotAreaList(const VsRect& p_arg0, const VsPoint& p_arg1, const VsPoint& p_arg2)
{
}

// 68K 0x102122f4 UpdateHandlers__12CHotAreaListFv
// STUB: LEMBALL 0x0046a6d0
void HotAreaList::UpdateHandlers()
{
}

// 68K 0x10212332 DeleteEntry__12CHotAreaListFP15CHotAreaElement
// STUB: LEMBALL 0x0046a6e0
void HotAreaList::DeleteEntry(HotAreaElement* p_entry)
{
}

// 68K 0x102123ae ProcessMsg__12CHotAreaListFP10tagMESSAGE
// STUB: LEMBALL 0x0046a710
int HotAreaList::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10212456 ProcessHandlers__12CHotAreaListFRC8CVSPointP10tagMESSAGE
// STUB: LEMBALL 0x0046a770
void HotAreaList::ProcessHandlers(const VsPoint& p_point, Message* p_message)
{
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
// STUB: LEMBALL 0x0046aa00
bool HotAreaList::RemoveFromList(HotAreaHandler* p_handler)
{
	return 0;
}

// 68K 0x10117b00 OnExit__12CHotAreaListFv
// STUB: LEMBALL 0x0046aa30
void HotAreaList::OnExit()
{
}

// 68K 0x1021221e __dt__12CHotAreaListFv
// SYNTHETIC: LEMBALL 0x0046aa50
// HotAreaList::`scalar deleting destructor'
HotAreaList::~HotAreaList()
{
}
