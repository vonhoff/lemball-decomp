#ifndef LEMBALL_AI_BASE_BASEGLOBALOBJECT_H
#define LEMBALL_AI_BASE_BASEGLOBALOBJECT_H

#include "../../Common.h"
#include "AiCoord.h"          // complete type
#include "GlobalGameObject.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x00493580
class BaseGlobalObject : public GlobalGameObject {
public:
	// FUNCTION: LEMBALL 0x0041c380
	inline BaseGlobalObject(const AiCoord& p_position, eObjectType p_type) : GlobalGameObject(p_type, 0, 0)
	{
		m_initialPosition = p_position;
	}
	virtual void Restart();      // vtable+0x104
	virtual ~BaseGlobalObject(); // vtable+0x00
	void OldRestart();

private:
	AiCoord m_initialPosition; // 0x138
};

// SYNTHETIC: LEMBALL 0x0040ce50
// BaseGlobalObject::`scalar deleting destructor'

#endif
