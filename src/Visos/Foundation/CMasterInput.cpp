#include "CMasterInput.h"

#include "../Target/Input/CMasterInputItem.h"

// FUNCTION: LEMBALL 0x00472070
CMasterInput::CMasterInput(CBaseQueue* p_queue)
{
	m_itemCount = 0;
	m_firstItem = 0;
	m_queue = p_queue;
	m_state = 0;
}

// FUNCTION: LEMBALL 0x00472090
CMasterInput::~CMasterInput()
{
	void* item = m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		void* next = ((void**) item)[1];
		operator delete(item);
		item = next;
	}
}

// FUNCTION: LEMBALL 0x004720c0
bool CMasterInput::AddItem(void* p_item)
{
	struct CNode {
		void* m_item;
		CNode* m_next;
	};
	CNode* added = new CNode;
	CNode* last = (CNode*) m_firstItem;
	if (m_itemCount == 0) {
		added->m_item = p_item;
		m_firstItem = added;
		m_itemCount++;
		return true;
	}
	for (unsigned int i = 1; i < m_itemCount; i++) {
		last = last->m_next;
	}
	added->m_item = p_item;
	last->m_next = added;
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
		CMasterInputItem* m_item;
		CNode* m_next;
	};
	CNode* node = (CNode*) m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		if (node->m_item->IsReady() == 1) {
			if (node->m_item->ProcessQueue(m_queue) == 0) {
				return false;
			}
		}
		node = node->m_next;
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
CVSOStream& CMasterInput::StreamOut(CVSOStream& p_stream)
{
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a279c
CMasterInput* g_pMasterInput = 0;
