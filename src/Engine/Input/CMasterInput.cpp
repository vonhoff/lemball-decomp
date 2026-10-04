#include "CMasterInput.h"

#include "CMasterInputItem.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00472070
CMasterInput::CMasterInput(CBaseQueue* p_queue)
{
	m_itemCount = 0;
	m_firstItem = NULL;
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

// FUNCTION: LEMBALL 0x00472110
bool CMasterInput::RemoveItem(void* p_item)
{
	struct CNode {
		void* m_item;
		CNode* m_next;
	};

	unsigned int index;
	unsigned int count = m_itemCount;
	CNode* node = (CNode*) m_firstItem;
	CNode* previous;
	for (index = 0; index < count; ++index) {
		if (node->m_item == p_item) {
			if (index == 0) {
				m_firstItem = node->m_next;
				delete node;
				--m_itemCount;
				return true;
			}
			else {
				previous->m_next = node->m_next;
			}
			delete node;
			--m_itemCount;
			return true;
		}
		previous = node;
		node = node->m_next;
	}
	return false;
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
	unsigned int count = m_itemCount;
	if (count == 0) {
		return true;
	}
	void** item = (void**) m_firstItem;
	for (unsigned int i = 0; i < count; i++) {
		if (item == NULL) {
			return false;
		}
		if (*item == NULL) {
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
CMasterInput* g_pMasterInput = NULL;
