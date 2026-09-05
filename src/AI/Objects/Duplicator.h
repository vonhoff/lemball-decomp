#ifndef LEMBALL_AI_OBJECTS_DUPLICATOR_H
#define LEMBALL_AI_OBJECTS_DUPLICATOR_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x00496050
class Duplicator : public GlobalGameObject {
public:
	Duplicator(const AiCoord& p_position);
	virtual AiCoord ActivatePosition();          // vtable+0x2c
	virtual bool Activate(GameObject* p_object); // vtable+0x18
	virtual bool Process();                      // vtable+0x14
	virtual int Usage();                         // vtable+0x28
	virtual void Delete();                       // vtable+0xc8
	virtual void DoActivate();                   // vtable+0x10c
	virtual void Restart();                      // vtable+0x104
	virtual ~Duplicator();                       // vtable+0x00
	void Set(const AiCoord& p_position);

private:
	unsigned int m_terrainCell0Set; // 0x138
	unsigned int m_terrainCell1Set; // 0x13c
	GameObject* m_duplicatedObject; // 0x140
};

// SYNTHETIC: LEMBALL 0x00427aa0
// Duplicator::`scalar deleting destructor'

#endif
