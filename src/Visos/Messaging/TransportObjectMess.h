#ifndef LEMBALL_VISOS_MESSAGING_TRANSPORTOBJECTMESS_H
#define LEMBALL_VISOS_MESSAGING_TRANSPORTOBJECTMESS_H

#include "../../AI/Messages/GameObjectMess.h" // complete type
#include "../../Common.h"

// SIZE 0x34
// VTABLE: LEMBALL 0x00493fe8
class TransportObjectMess : public GameObjectMess {
public:
	inline TransportObjectMess() : GameObjectMess(0) {}
	virtual void GetData();         // vtable+0x08
	virtual ~TransportObjectMess(); // vtable+0x14

private:
	unsigned short m_transportObjectId; // 0x30
};

// SYNTHETIC: LEMBALL 0x00417d50
// TransportObjectMess::`scalar deleting destructor'


#endif

