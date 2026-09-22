#ifndef LEMBALL_AI_MESSAGES_COBJECTPOSMESS_H
#define LEMBALL_AI_MESSAGES_COBJECTPOSMESS_H

#include "CGameObjectMess.h" // complete type

class CObjectPosMess : public CGameObjectMess {
public:
	CObjectPosMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
};

// SYNTHETIC: LEMBALL 0x00417c70
// CObjectPosMess::`scalar deleting destructor'

#endif
