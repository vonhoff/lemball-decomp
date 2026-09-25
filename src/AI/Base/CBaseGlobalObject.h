#ifndef LEMBALL_AI_BASE_CBASEGLOBALOBJECT_H
#define LEMBALL_AI_BASE_CBASEGLOBALOBJECT_H

#include "AI/Base/ObjectTypes.h"
#include "AiCoord.h"
#include "CGlobalGameObject.h"

// SIZE 0x144
// VTABLE: LEMBALL 0x00493580
class CBaseGlobalObject : public CGlobalGameObject {
public:
	// FUNCTION: LEMBALL 0x0041c380
	inline CBaseGlobalObject(const AiCoord& p_position, eObjectType p_type) : CGlobalGameObject(p_type, 0, 0)
	{
		m_initialPosition = p_position;
	}
	virtual void Restart(); // vtable+0x104
	void OldRestart();

private:
	AiCoord m_initialPosition; // 0x138
};

// SYNTHETIC: LEMBALL 0x0040ce50
// CBaseGlobalObject::`scalar deleting destructor'

#endif
