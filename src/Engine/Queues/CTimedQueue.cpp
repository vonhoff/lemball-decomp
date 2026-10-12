#include "CTimedQueue.h"

#include "tagMESSAGE.h"

// FUNCTION: LEMBALL 0x00458e80
bool CTimedQueue::Post(tagMESSAGE& p_message)
{
	tagMESSAGE* slot;
	unsigned int count;
	unsigned int index;

	slot = m_readCursor;
	if (m_capacity == m_messageCount) {
		do {
			m_overflowCount = m_overflowCount + 1;
			ProcessNMsgs(1);
		} while (m_capacity == m_messageCount);
	}
	count = m_messageCount;
	for (index = 0; index < count; ++index) {
		if ((int) (p_message.m_time - slot->m_time) < 0) {
			break;
		}
		slot = slot + 1;
		if (m_messageBufferEnd <= slot) {
			slot = m_messageBuffer;
		}
	}
	PutNth(&p_message, index);
	return true;
}

// FUNCTION: LEMBALL 0x00458ef0
bool CTimedQueue::Send(tagMESSAGE& p_message)
{
	m_sendCount = m_sendCount + 1;
	return Process(&p_message) != 0;
}
