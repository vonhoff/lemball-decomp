#ifndef LEMBALL_AI_OBJECTS_CTHEBALLOONPOST_H
#define LEMBALL_AI_OBJECTS_CTHEBALLOONPOST_H

#include "../Base/CGameObject.h" // complete type
#include "AI/Base/ObjectTypes.h"

// SIZE 0x128
// VTABLE: LEMBALL 0x00496488
class CTheBalloonPost : public CGameObject {
public:
	CTheBalloonPost(eObjectType p_arg0, unsigned int p_arg1);

	friend class CBalloonPost;

private:
	unsigned int m_active; // 0x124
};

// SYNTHETIC: LEMBALL 0x0042a7a0
// CTheBalloonPost::`scalar deleting destructor'

#endif
