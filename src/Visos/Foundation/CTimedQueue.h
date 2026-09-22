#ifndef LEMBALL_VISOS_FOUNDATION_CTIMEDQUEUE_H
#define LEMBALL_VISOS_FOUNDATION_CTIMEDQUEUE_H

#include "CBaseQueue.h" // complete type

// SIZE 0x58
// VTABLE: LEMBALL 0x004988b0 CCritical
// VTABLE: LEMBALL 0x004988b8 CArenaBase
class CTimedQueue : public CBaseQueue {
public:
	CTimedQueue(unsigned int p_capacity, char* p_name) : CBaseQueue(p_capacity, p_name) {}
	virtual bool Post(Message& p_message); // vtable+0x08
	virtual bool Send(Message& p_message); // vtable+0x0c
};

// SYNTHETIC: LEMBALL 0x004599b0
// CTimedQueue::`scalar deleting destructor'

#endif
