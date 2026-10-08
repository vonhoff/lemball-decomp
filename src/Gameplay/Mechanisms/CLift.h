#ifndef LEMBALL_AI_OBJECTS_CLIFT_H
#define LEMBALL_AI_OBJECTS_CLIFT_H

#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Objects/CGlobalGameObject.h"

class AICOORD;
class CGameObject;
class CGround;
enum eLiftDirection {
	LIFT_DIRECTION_LOWERING = -1,
	LIFT_DIRECTION_RISING = 1
};

enum {
	LIFT_LOW_HEIGHT_FOLLOWS_START_HEIGHT = -1
};

enum eLiftActivateType {
	LIFT_ACTIVATE_SWITCH_TOGGLE = 0,
	LIFT_ACTIVATE_STEP = 1,
	LIFT_ACTIVATE_CONTINUOUS = 2,
	LIFT_ACTIVATE_SWITCH_ONCE = 3,
	LIFT_ACTIVATE_STEP_ONCE = 4
};

enum eLiftActivationLatchState {
	LIFT_ACTIVATION_NOT_LATCHED = 0,
	LIFT_ACTIVATION_LATCHED = 1
};

// SIZE 0x190
// VTABLE: LEMBALL 0x00495d60
class CLift : public CGlobalGameObject {
public:
	CLift();
	int Activate();
	int StepOn(const AICOORD& p_position, CGameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual ~CLift();          // vtable+0x00
	void ActivateDeactivate();
	void CalculateCliff();
	void CheckObjects();
	void Edit(int p_height,
			  short p_direction,
			  int p_lowHeight,
			  int p_highHeight,
			  eLiftActivateType p_activateType,
			  unsigned int p_initialActive);
	void Set(tCoord3d& p_start,
			 tCoord3d& p_end,
			 short p_direction,
			 int p_lowHeight,
			 int p_highHeight,
			 eLiftActivateType p_activateType,
			 unsigned int p_initialActive);
	void Set(int p_x,
			 int p_y,
			 int p_z,
			 short p_direction,
			 int p_lowHeight,
			 int p_highHeight,
			 eLiftActivateType p_activateType,
			 unsigned int p_initialActive);

	friend class CLiftManager;

private:
	unsigned short m_liftId;          // 0x138
	tCoord3d m_start;                 // 0x13a
	tCoord3d m_end;                   // 0x140
	int m_lowHeight;                  // 0x148
	int m_highHeight;                 // 0x14c
	int m_movementStartHeight;        // 0x150
	short m_direction;                // 0x154
	unsigned int m_unk0x158;          // 0x158
	eLiftActivateType m_activateType; // 0x15c
	CGround* m_mapCell;               // 0x160
	unsigned int m_active;            // 0x164
	unsigned int m_defaultActive;     // 0x168
	unsigned int m_activationLatched; // 0x16c
	CGameObject* m_objects[8];        // 0x170
};

// SYNTHETIC: LEMBALL 0x00426710
// CLift::`vector deleting destructor'

#endif
