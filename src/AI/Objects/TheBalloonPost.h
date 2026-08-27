#ifndef LEMBALL_AI_OBJECTS_THEBALLOONPOST_H
#define LEMBALL_AI_OBJECTS_THEBALLOONPOST_H

#include "../../Common.h"
#include "../Base/GameObject.h" // complete type

// SIZE 0x128
// VTABLE: LEMBALL 0x00496488
class TheBalloonPost : public GameObject {
public:
	TheBalloonPost(eObjectType p_arg0, unsigned char p_arg1);
	virtual ~TheBalloonPost(); // vtable+0x00

private:
	unsigned int m_active; // 0x124
};

// SYNTHETIC: LEMBALL 0x0042a7a0
// TheBalloonPost::`scalar deleting destructor'


#endif

