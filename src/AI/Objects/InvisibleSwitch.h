#ifndef LEMBALL_AI_OBJECTS_INVISIBLESWITCH_H
#define LEMBALL_AI_OBJECTS_INVISIBLESWITCH_H

#include "../../Common.h"
#include "../Base/Coord3d.h"          // complete type
#include "../Base/GlobalGameObject.h" // complete type
#include "InvisibleSwitchTarget.h"    // complete type

// SIZE 0x2b8
// VTABLE: LEMBALL 0x00493120
class InvisibleSwitch : public GlobalGameObject {
public:
	InvisibleSwitch();
	virtual bool Process();     // vtable+0x14
	virtual void DoActivate();  // vtable+0x10c
	virtual void Restart();     // vtable+0x104
	virtual ~InvisibleSwitch(); // vtable+0x00
	void AddObject(GameObject* p_object);
	void Initialise();
	void Load(unsigned char*& p_data);
	void Set(const Coord3d& p_cornerA, const Coord3d& p_cornerB);
	void StepOn(const AiCoord& p_position, GameObject* p_object);
	void VerifyObjects();

	friend class InvisibleSwitchManager;

private:
	Coord3d m_minCorner;                 // 0x138
	Coord3d m_maxCorner;                 // 0x13e
	unsigned int m_repeatable;           // 0x144
	unsigned int m_triggered;            // 0x148
	unsigned int m_scoreAwarded;         // 0x14c
	unsigned short m_targetCount;        // 0x150
	InvisibleSwitchTarget m_targets[32]; // 0x154
	int m_objectCount;                   // 0x254
	GameObject* m_objects[24];           // 0x258
};

// SYNTHETIC: LEMBALL 0x0040ac60
// InvisibleSwitch::`vector deleting destructor'

#endif
