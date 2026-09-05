#ifndef LEMBALL_VISOS_FOUNDATION_TIMEDQUEUE_H
#define LEMBALL_VISOS_FOUNDATION_TIMEDQUEUE_H

#include "../../Common.h"
#include "BaseQueue.h" // complete type

// SIZE 0x58
// VTABLE: LEMBALL 0x004988b8
class TimedQueue : public BaseQueue {
public:
	TimedQueue(unsigned int p_capacity, char* p_name) : BaseQueue(p_capacity, p_name) {}
	virtual bool Post(Message& p_message); // vtable+0x08
	virtual bool Send(Message& p_message); // vtable+0x0c
	virtual ~TimedQueue();              // vtable+0x04
};

// SYNTHETIC: LEMBALL 0x004599b0
// TimedQueue::`scalar deleting destructor'

#endif
