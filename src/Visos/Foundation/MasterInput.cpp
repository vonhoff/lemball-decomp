#define LEMBALL_OUTLINE_INPUT_HELPERS
#include "MasterInput.h"

// 68K 0x10201326 __ct__12CMasterInputFP10CBaseQueue
// FUNCTION: LEMBALL 0x00472070
MasterInput::MasterInput(BaseQueue* p_arg0)
{
	m_itemCount = 0;
	m_firstItem = 0;
	m_queue = p_arg0;
	m_state = 0;
}

// 68K 0x1020137c __dt__12CMasterInputFv
// FUNCTION: LEMBALL 0x00472090
MasterInput::~MasterInput()
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
bool MasterInput::AddItem(void* p_item)
{
	struct Node {
		void* item;
		Node* next;
	};
	Node* added = new Node;
	Node* last = (Node*) m_firstItem;
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

// FUNCTION: LEMBALL 0x004721e0
bool MasterInput::IsEmpty()
{
	unsigned int count = m_itemCount;
	if (count == 0) {
		return true;
	}
	void** item = (void**) m_firstItem;
	for (unsigned int i = 0; i < count; i++) {
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

// 68K 0x102013e8 StreamOut__12CMasterInputFR10CVSOStream
// FUNCTION: LEMBALL 0x00472210
VsOStream& MasterInput::StreamOut(VsOStream& p_arg0)
{
	return p_arg0;
}

// GLOBAL: LEMBALL 0x004a279c
MasterInput* g_pMasterInput = 0;
