#include "BaseQueue.h"

#include "BaseQueueHandler.h"

#define WIN32_LEAN_AND_MEAN
#include <new.h>
#include <windows.h>

struct QueueHandlerNode {
	BaseQueueHandler* handler;
	int priority;
	QueueHandlerNode* next;
};

BaseQueue::BaseQueue()
{
}

// 68K 0x102049f4 __ct__10CBaseQueueFUi
// FUNCTION: LEMBALL 0x00463020
BaseQueue::BaseQueue(unsigned int p_capacity)
{
	unsigned char* buffer;

	InitializeCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	buffer = (unsigned char*) operator new(p_capacity * sizeof(Message));
	m_messageBuffer = buffer;
	m_capacity = p_capacity;
	m_messageBufferEnd = buffer + p_capacity * sizeof(Message);
	m_writeCursor = buffer;
	m_readCursor = buffer;
	m_messageCount = 0;
	m_handlerCount = 0;
	m_overflowCount = 0;
	m_postCount = 0;
	m_sendCount = 0;
	m_unhandledCount = 0;
	m_nextSequence = 0;
	m_handlerList = 0;
}

// 68K 0x10204aac __ct__10CBaseQueueFUiPc
// FUNCTION: LEMBALL 0x004630a0
BaseQueue::BaseQueue(unsigned int p_capacity, char* p_name)
{
	unsigned char* buffer;

	(void) p_name;
	InitializeCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	buffer = (unsigned char*) operator new(p_capacity * sizeof(Message));
	m_messageBuffer = buffer;
	m_capacity = p_capacity;
	m_messageBufferEnd = buffer + p_capacity * sizeof(Message);
	m_writeCursor = buffer;
	m_readCursor = buffer;
	m_messageCount = 0;
	m_handlerCount = 0;
	m_overflowCount = 0;
	m_postCount = 0;
	m_sendCount = 0;
	m_unhandledCount = 0;
	m_nextSequence = 0;
	m_handlerList = 0;
}

// 68K 0x10204b66 __dt__10CBaseQueueFv
// FUNCTION: LEMBALL 0x00463120
BaseQueue::~BaseQueue()
{
	QueueHandlerNode* node;
	QueueHandlerNode* next;
	unsigned int index;

	ProcessNMsgs(m_messageCount);
	operator delete(m_messageBuffer);
	node = (QueueHandlerNode*) m_handlerList;
	index = 0;
	if (m_handlerCount != 0) {
		do {
			next = node->next;
			operator delete(node);
			index = index + 1;
			node = next;
		} while (index < m_handlerCount);
	}
	DeleteCriticalSection((CRITICAL_SECTION*) m_criticalSection);
}

// 68K 0x10204bfa Post__10CBaseQueueFR10tagMESSAGE
// FUNCTION: LEMBALL 0x004631a0
bool BaseQueue::Post(Message& p_message)
{
	unsigned char* write;
	unsigned int* dest;
	unsigned int* src;
	int i;
	bool result;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	m_postCount = m_postCount + 1;
	p_message.time = m_nextSequence;
	m_nextSequence = m_nextSequence + 1;
	if (m_capacity == m_messageCount) {
		m_overflowCount = m_overflowCount + 1;
		ProcessNMsgs(1);
		result = Post(p_message);
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return result;
	}
	m_messageCount = m_messageCount + 1;
	write = m_writeCursor;
	dest = (unsigned int*) write;
	src = (unsigned int*) &p_message;
	for (i = 5; i != 0; i = i - 1) {
		*dest = *src;
		src = src + 1;
		dest = dest + 1;
	}
	write = m_writeCursor;
	m_writeCursor = write + sizeof(Message);
	if (m_messageBufferEnd <= write + sizeof(Message)) {
		m_writeCursor = m_messageBuffer;
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x10204cd8 Send__10CBaseQueueFR10tagMESSAGE
// FUNCTION: LEMBALL 0x00463230
bool BaseQueue::Send(Message& p_message)
{
	bool result;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	m_sendCount = m_sendCount + 1;
	p_message.time = m_nextSequence;
	m_nextSequence = m_nextSequence + 1;
	result = Process(&p_message);
	if (result != 0) {
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return 1;
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 0;
}

// 68K 0x10204d68 StreamOut__10CBaseQueueFR10CVSOStream
// FUNCTION: LEMBALL 0x00463280
VsOStream& BaseQueue::StreamOut(VsOStream& p_stream)
{
	return p_stream;
}

// 68K 0x10204d9e Attach__10CBaseQueueFP17CBaseQueueHandleri
// FUNCTION: LEMBALL 0x004632a0
bool BaseQueue::Attach(BaseQueueHandler* p_handler, int p_priority)
{
	QueueHandlerNode* node;
	QueueHandlerNode* current;
	QueueHandlerNode* previous;
	unsigned int index;
	unsigned int count;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	node = (QueueHandlerNode*) operator new(0xc);
	node->handler = p_handler;
	node->priority = p_priority;
	if (m_handlerList == 0) {
		m_handlerList = node;
		node->next = 0;
		m_handlerCount = 1;
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return 1;
	}
	count = m_handlerCount;
	if (count == 0) {
		m_handlerList = node;
		m_handlerCount = 1;
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return 1;
	}
	index = 0;
	previous = (QueueHandlerNode*) m_handlerList;
	current = previous;
	if (count != 0) {
		do {
			if (p_priority < current->priority) {
				if (index == 0) {
					node->next = (QueueHandlerNode*) m_handlerList;
					m_handlerList = node;
				}
				else {
					node->next = previous->next;
					previous->next = node;
				}
				m_handlerCount = m_handlerCount + 1;
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 1;
			}
			if (current->next == 0) {
				node->next = 0;
				current->next = node;
				m_handlerCount = m_handlerCount + 1;
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 1;
			}
			index = index + 1;
			previous = current;
			current = current->next;
		} while (index < count);
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 0;
}

// 68K 0x10204ee0 Detach__10CBaseQueueFP17CBaseQueueHandleri
// FUNCTION: LEMBALL 0x004633b0
bool BaseQueue::Detach(BaseQueueHandler* p_handler, int p_priority)
{
	QueueHandlerNode* current;
	QueueHandlerNode* previous;
	unsigned int index;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	previous = (QueueHandlerNode*) m_handlerList;
	current = previous;
	index = 0;
	if (m_handlerCount != 0) {
		do {
			if (current->priority == p_priority && current->handler == p_handler) {
				if (index != 0) {
					previous->next = current->next;
					operator delete(current);
					m_handlerCount = m_handlerCount - 1;
					LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
					return 1;
				}
				m_handlerList = current->next;
				operator delete(current);
				m_handlerCount = m_handlerCount - 1;
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 1;
			}
			index = index + 1;
			previous = current;
			current = current->next;
		} while (index < m_handlerCount);
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 0;
}

// 68K 0x10204fbe GetNth__10CBaseQueueFP10tagMESSAGEUi
// FUNCTION: LEMBALL 0x00463570
bool BaseQueue::GetNth(Message* p_message, unsigned int p_index)
{
	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	PeekNth(p_message, p_index);
	DeleteNth(p_index);
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x1020503c PeekNth__10CBaseQueueFP10tagMESSAGEUi
// FUNCTION: LEMBALL 0x004635b0
bool BaseQueue::PeekNth(Message* p_message, unsigned int p_index)
{
	unsigned char* slot;
	unsigned int* dest;
	unsigned int* src;
	int i;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	slot = m_readCursor + p_index * sizeof(Message);
	if (m_messageBufferEnd <= slot) {
		slot = m_messageBuffer + (((int) slot - (int) m_messageBufferEnd) / sizeof(Message)) * sizeof(Message);
	}
	dest = (unsigned int*) p_message;
	src = (unsigned int*) slot;
	for (i = 5; i != 0; i = i - 1) {
		*dest = *src;
		src = src + 1;
		dest = dest + 1;
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x102050dc PutNth__10CBaseQueueFP10tagMESSAGEUi
// FUNCTION: LEMBALL 0x00463610
bool BaseQueue::PutNth(Message* p_message, unsigned int p_index)
{
	unsigned char* slot;
	unsigned char* dest;
	unsigned char* src;
	unsigned int* copyDest;
	unsigned int* copySrc;
	unsigned int shifted;
	unsigned int remain;
	int i;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	slot = m_readCursor + p_index * sizeof(Message);
	if (m_messageBufferEnd <= slot) {
		slot = m_messageBuffer + (((int) slot - (int) m_messageBufferEnd) / sizeof(Message)) * sizeof(Message);
	}
	if (p_index < m_messageCount) {
		dest = m_writeCursor;
		src = dest - sizeof(Message);
		shifted = 0;
		remain = m_messageCount - p_index;
		if (remain != 0) {
			do {
				if (src < m_messageBuffer) {
					src = m_messageBufferEnd - sizeof(Message);
				}
				if (dest < m_messageBuffer) {
					dest = m_messageBufferEnd - sizeof(Message);
				}
				copyDest = (unsigned int*) dest;
				copySrc = (unsigned int*) src;
				for (i = 5; i != 0; i = i - 1) {
					*copyDest = *copySrc;
					copySrc = copySrc + 1;
					copyDest = copyDest + 1;
				}
				src = src - sizeof(Message);
				dest = dest - sizeof(Message);
				shifted = shifted + 1;
			} while (shifted < m_messageCount - p_index);
		}
	}
	copyDest = (unsigned int*) slot;
	copySrc = (unsigned int*) p_message;
	for (i = 5; i != 0; i = i - 1) {
		*copyDest = *copySrc;
		copySrc = copySrc + 1;
		copyDest = copyDest + 1;
	}
	slot = m_writeCursor;
	m_writeCursor = slot + sizeof(Message);
	if (m_messageBufferEnd <= slot + sizeof(Message)) {
		m_writeCursor = m_messageBuffer;
	}
	m_messageCount = m_messageCount + 1;
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x102051fe DeleteNth__10CBaseQueueFUi
// FUNCTION: LEMBALL 0x004636e0
bool BaseQueue::DeleteNth(unsigned int p_index)
{
	unsigned char* slot;
	unsigned char* dest;
	unsigned char* src;
	unsigned char* read;
	unsigned char* end;
	unsigned int* copyDest;
	unsigned int* copySrc;
	unsigned int count;
	int i;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	read = m_readCursor;
	end = m_messageBufferEnd;
	slot = read + p_index * sizeof(Message);
	if (end <= slot) {
		slot = m_messageBuffer + (((int) slot - (int) end) / sizeof(Message)) * sizeof(Message);
	}
	count = m_messageCount;
	if (count == 1) {
		m_messageCount = 0;
		m_readCursor = m_writeCursor;
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return 1;
	}
	if (slot == read) {
		m_readCursor = read + sizeof(Message);
		if (end <= read + sizeof(Message)) {
			m_readCursor = m_messageBuffer;
		}
		m_messageCount = count - 1;
		LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
		return 1;
	}
	src = slot;
	dest = slot + sizeof(Message);
	if (end <= dest) {
		dest = m_messageBuffer;
	}
	if (p_index < count) {
		do {
			if (m_messageBufferEnd <= dest) {
				dest = m_messageBuffer;
			}
			if (m_messageBufferEnd <= src) {
				src = m_messageBuffer;
			}
			copyDest = (unsigned int*) dest;
			copySrc = (unsigned int*) src;
			for (i = 5; i != 0; i = i - 1) {
				*copyDest = *copySrc;
				copySrc = copySrc + 1;
				copyDest = copyDest + 1;
			}
			src = src + sizeof(Message);
			dest = dest + sizeof(Message);
			p_index = p_index + 1;
		} while (p_index < m_messageCount);
	}
	slot = m_writeCursor;
	m_writeCursor = slot - sizeof(Message);
	if (m_writeCursor <= m_messageBuffer) {
		m_writeCursor = m_messageBufferEnd - sizeof(Message);
	}
	m_messageCount = m_messageCount - 1;
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x10205344 ProcessNMsgs__10CBaseQueueFUi
// FUNCTION: LEMBALL 0x00463810
bool BaseQueue::ProcessNMsgs(unsigned int p_count)
{
	Message message;
	unsigned int available;
	unsigned int index;

	index = 0;
	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	available = m_messageCount;
	if (p_count != 0) {
		do {
			if (available <= index) {
				break;
			}
			if (GetNth(&message, 0) == 0) {
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 0;
			}
			if (Process(&message) == 0) {
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 0;
			}
			index = index + 1;
		} while (index < p_count);
	}
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// 68K 0x10205402 Process__10CBaseQueueFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004638a0
bool BaseQueue::Process(Message* p_message)
{
	QueueHandlerNode* node;
	unsigned int index;
	int result;

	EnterCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	node = (QueueHandlerNode*) m_handlerList;
	index = 0;
	if (m_handlerCount != 0) {
		do {
			result = node->handler->ProcessMsg(p_message);
			if (result == 1) {
				LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
				return 1;
			}
			node = node->next;
			index = index + 1;
		} while (index < m_handlerCount);
	}
	m_unhandledCount = m_unhandledCount + 1;
	LeaveCriticalSection((CRITICAL_SECTION*) m_criticalSection);
	return 1;
}

// GLOBAL: LEMBALL 0x004a9360
BaseQueue* g_pMasterInputQueue;
