#ifndef LEMBALL_AI_OBJECTS_DOOR_H
#define LEMBALL_AI_OBJECTS_DOOR_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x14c
// VTABLE: LEMBALL 0x004936f8
class Door : public GlobalGameObject {
public:
	Door();
	int Hits(const AiCoord& p_position, GameObject* p_object);
	virtual bool IsUsable(eAction p_action); // vtable+0xb4
	virtual bool Process();                  // vtable+0x14
	virtual void Delete();                   // vtable+0xc8
	virtual void DoActivate();               // vtable+0x10c
	virtual void Restart();                  // vtable+0x104
	virtual ~Door();                         // vtable+0x00
	void ResetCollision();
	void Set(eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z);
	void SetCollision();
	void Unlock();

private:
	unsigned short m_doorIndex;       // 0x138
	unsigned int m_setTick;           // 0x13c
	short m_doorType;                 // 0x140
	unsigned int m_activationPending; // 0x144
};

// SYNTHETIC: LEMBALL 0x0040ec10
// Door::`scalar deleting destructor'


#endif

