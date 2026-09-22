#include "CMasterInput.h"

#include "../Target/CMasterInputItem.h"

// FUNCTION: LEMBALL 0x00472070
CMasterInput::CMasterInput(CBaseQueue* p_arg0)
{
	m_itemCount = 0;
	m_firstItem = 0;
	m_queue = p_arg0;
	m_state = 0;
}

// FUNCTION: LEMBALL 0x00472090
CMasterInput::~CMasterInput()
{
	void* item = m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		// The original list link is at +0x04; the remaining item layout is unknown.
		void* next = ((void**) item)[1];
		operator delete(item);
		item = next;
	}
}

// FUNCTION: LEMBALL 0x004720c0
bool CMasterInput::AddItem(void* p_item)
{
	struct CNode {
		void* item;
		CNode* next;
	};
	CNode* added = new CNode;
	CNode* last = (CNode*) m_firstItem;
	if (m_itemCount == 0) {
		added->item = p_item;
		m_firstItem = added;
		m_itemCount++;
		return true;
	}
	for (unsigned int i = 1; i < m_itemCount; i++) {
		last = last->next;
	}
	added->item = p_item;
	last->next = added;
	m_itemCount++;
	return true;
}

// FUNCTION: LEMBALL 0x00472190
bool CMasterInput::ProcessItems()
{
	if (m_itemCount == 0) {
		return false;
	}
	struct CNode {
		CMasterInputItem* item;
		CNode* next;
	};
	CNode* node = (CNode*) m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		if (node->item->IsReady() == 1) {
			if (node->item->ProcessQueue(m_queue) == 0) {
				return false;
			}
		}
		node = node->next;
	}
	return true;
}

// FUNCTION: LEMBALL 0x004721e0
bool CMasterInput::IsEmpty()
{
	unsigned int i;
	unsigned int count = m_itemCount;
	if (count == 0) {
		return true;
	}
	void** item = (void**) m_firstItem;
	for (i = 0; i < count; i++) {
		if (item == 0) {
			return false;
		}
		if (*item == 0) {
			return false;
		}
		item = (void**) item[1];
	}
	return false;
}

// FUNCTION: LEMBALL 0x00472210
CVsOStream& CMasterInput::StreamOut(CVsOStream& p_arg0)
{
	return p_arg0;
}

// GLOBAL: LEMBALL 0x004a279c
CMasterInput* g_pMasterInput = 0;
