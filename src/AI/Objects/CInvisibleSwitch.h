#ifndef LEMBALL_AI_OBJECTS_CINVISIBLESWITCH_H
#define LEMBALL_AI_OBJECTS_CINVISIBLESWITCH_H

#include "../Base/CGlobalGameObject.h" // complete type
#include "../Base/Coord3d.h"           // complete type
#include "InvisibleSwitchTarget.h"     // complete type

class AiCoord;
class CGameObject;
// SIZE 0x2b8
// VTABLE: LEMBALL 0x00493120
class CInvisibleSwitch : public CGlobalGameObject {
public:
	CInvisibleSwitch();
	virtual bool Process();      // vtable+0x14
	virtual void DoActivate();   // vtable+0x10c
	virtual void Restart();      // vtable+0x104
	virtual ~CInvisibleSwitch(); // vtable+0x00
	void AddObject(CGameObject* p_object);
	void Initialise();
	void Load(unsigned char*& p_data);
	void Set(const Coord3d& p_cornerA, const Coord3d& p_cornerB);
	void SetPointFromIntegers(short p_x, short p_y, short p_z);
	void StepOn(const AiCoord& p_position, CGameObject* p_object);
	void VerifyObjects();

	friend class CInvisibleSwitchManager;

private:
	Coord3d m_minCorner;                 // 0x138
	Coord3d m_maxCorner;                 // 0x13e
	unsigned int m_repeatable;           // 0x144
	unsigned int m_triggered;            // 0x148
	unsigned int m_scoreAwarded;         // 0x14c
	unsigned short m_targetCount;        // 0x150
	InvisibleSwitchTarget m_targets[32]; // 0x154
	int m_objectCount;                   // 0x254
	CGameObject* m_objects[24];          // 0x258
};

// SYNTHETIC: LEMBALL 0x0040ac60
// CInvisibleSwitch::`vector deleting destructor'

#endif
