#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_TIMEDQUEUE_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_TIMEDQUEUE_H

#include "../../Common.h"
#include "BaseQueue.h" // complete type

// SIZE 0x58
// VTABLE: LEMBALL 0x004988b8
class TimedQueue : public BaseQueue {
public:
	virtual bool Post(Message& p_arg0); // vtable+0x08
	virtual bool Send(Message& p_arg0); // vtable+0x0c
	virtual ~TimedQueue();              // vtable+0x04
};

#endif
