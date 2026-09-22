#include "CTimedQueue.h"

#include "Visos/Foundation/Message.h"

// FUNCTION: LEMBALL 0x00458e80
bool CTimedQueue::Post(Message& p_message)
{
	unsigned char* slot;
	unsigned int count;
	unsigned int index;
	Message* message;

	slot = m_readCursor;
	if (m_capacity == m_messageCount) {
		do {
			m_overflowCount = m_overflowCount + 1;
			ProcessNMsgs(1);
		} while (m_capacity == m_messageCount);
	}
	index = 0;
	count = m_messageCount;
	if (count != 0) {
		do {
			message = (Message*) slot;
			if ((int) (p_message.time - message->time) < 0) {
				break;
			}
			slot = slot + sizeof(Message);
			index = index + 1;
			if (m_messageBufferEnd <= slot) {
				slot = m_messageBuffer;
			}
		} while (index < count);
	}
	PutNth(&p_message, index);
	return 1;
}

// FUNCTION: LEMBALL 0x00458ef0
bool CTimedQueue::Send(Message& p_message)
{
	m_sendCount = m_sendCount + 1;
	return (unsigned int) Process(&p_message) >= 1;
}
