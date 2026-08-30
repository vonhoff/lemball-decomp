#ifndef LEMBALL_AI_OBJECTS_PAINTGUN_H
#define LEMBALL_AI_OBJECTS_PAINTGUN_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x00496740
class PaintGun : public GlobalGameObject {
public:
	PaintGun();
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~PaintGun();       // vtable+0x00
	void Set(unsigned short p_id, const AiCoord& p_position, int p_direction);

private:
	unsigned int m_enabled; // 0x138
	unsigned int m_active;  // 0x13c
	int m_direction;        // 0x140
};

// SYNTHETIC: LEMBALL 0x0042c9e0
// PaintGun::`scalar deleting destructor'

#endif
