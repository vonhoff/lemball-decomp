#include "CMasterInput.h"

#include <stddef.h>

#include "CMasterInputItem.h"

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
	CNode* item = m_firstItem;
	for (unsigned int i = 0; i < m_itemCount; i++) {
		CNode* next = item->m_next;
		delete item;
		item = next;
	}
}

// FUNCTION: LEMBALL 0x004720c0
bool CMasterInput::AddItem(CMasterInputItem* p_item)
{
	CNode* added = new CNode;
	CNode* last = m_firstItem;
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
bool CMasterInput::RemoveItem(CMasterInputItem* p_item)
{
	unsigned int index;
	unsigned int count = m_itemCount;
	CNode* node = m_firstItem;
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
	CNode* node = m_firstItem;
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
	CNode* item = m_firstItem;
	for (unsigned int i = 0; i < count; i++) {
		if (item == NULL) {
			return false;
		}
		if (item->m_item == NULL) {
			return false;
		}
		item = item->m_next;
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
