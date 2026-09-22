#ifndef LEMBALL_AI_OBJECTS_CMINE_H
#define LEMBALL_AI_OBJECTS_CMINE_H

#include "../Base/AiCoord.h"           // complete type
#include "../Base/CGlobalGameObject.h" // complete type
#include "AI/Base/ObjectActions.h"

class CGameObject;
// SIZE 0x150
// VTABLE: LEMBALL 0x00495be8
class CMine : public CGlobalGameObject {
public:
	CMine();
	virtual bool IsUsable(eAction p_action); // vtable+0xb4
	virtual bool Process();                  // vtable+0x14
	virtual void DoActivate();               // vtable+0x08
	virtual void Restart();                  // vtable+0x104
	void Initialise();
	void OnGround();
	void Set(AiCoord p_position);
	void SetTerrain();
	void StepOn(CGameObject* p_object);
	void Trigger(int p_delay);

	friend class CMineManager;

private:
	unsigned int m_enabled;        // 0x138
	unsigned int m_activated;      // 0x13c
	unsigned int m_terrainSet;     // 0x140
	unsigned int m_triggerPending; // 0x144
	int m_triggerDelay;            // 0x148
	int m_managerIndex;            // 0x14c
};

// SYNTHETIC: LEMBALL 0x00424c60
// CMine::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x00424cf0
// CMine::~CMine

extern short g_mineTerrainOffsets[4];

#endif
