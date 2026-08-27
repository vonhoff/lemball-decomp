#ifndef LEMBALL_AI_OBJECTS_HAND_H
#define LEMBALL_AI_OBJECTS_HAND_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x00496198
class Hand : public GlobalGameObject {
public:
	Hand();
	bool StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~Hand();           // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position);

private:
	unsigned int m_activated; // 0x138
	unsigned int m_enabled;   // 0x13c
	GameObject* m_activator;  // 0x140
};

// SYNTHETIC: LEMBALL 0x00428800
// Hand::`vector deleting destructor'


#endif

