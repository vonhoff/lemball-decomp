#include "TimedQueue.h"

// 68K 0x10212ec6 Post__11CTimedQueueFR10tagMESSAGE
// FUNCTION: LEMBALL 0x00458e80
bool TimedQueue::Post(Message& p_arg0)
{
	unsigned char* slot;
	unsigned int index;
	unsigned int count;
	int stamp;
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
	stamp = *(int*) &p_arg0.reserved[1];
	if (count != 0) {
		do {
			message = (Message*) slot;
			if ((int) (stamp - *(int*) &message->reserved[1]) < 0) {
				break;
			}
			slot = slot + sizeof(Message);
			index = index + 1;
			if (m_messageBufferEnd <= slot) {
				slot = m_messageBuffer;
			}
		} while (index < count);
	}
	PutNth(&p_arg0, index);
	return 1;
}

// 68K 0x10212f5e Send__11CTimedQueueFR10tagMESSAGE
// FUNCTION: LEMBALL 0x00458ef0
bool TimedQueue::Send(Message& p_arg0)
{
	m_sendCount = m_sendCount + 1;
	return Process(&p_arg0);
}

// 68K 0x10117d58 __dt__11CTimedQueueFv
TimedQueue::~TimedQueue()
{
}
