#ifndef LEMBALL_AI_OBJECTS_MINE_H
#define LEMBALL_AI_OBJECTS_MINE_H

#include "../../Common.h"
#include "../Base/AiCoord.h"          // complete type
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x150
// VTABLE: LEMBALL 0x00495be8
class Mine : public GlobalGameObject {
public:
	Mine();
	virtual bool IsUsable(eAction p_action); // vtable+0xb4
	virtual bool Process();                  // vtable+0x14
	virtual void DoActivate();               // vtable+0x08
	virtual void Restart();                  // vtable+0x104
	virtual ~Mine();                         // vtable+0x00
	void Initialise();
	void OnGround();
	void Set(AiCoord p_position);
	void SetTerrain();
	void StepOn(GameObject* p_object);
	void Trigger(int p_delay);

private:
	unsigned int m_enabled;        // 0x138
	unsigned int m_activated;      // 0x13c
	unsigned int m_terrainSet;     // 0x140
	unsigned int m_triggerPending; // 0x144
	int m_triggerDelay;            // 0x148
	int m_managerIndex;            // 0x14c
};

#endif
