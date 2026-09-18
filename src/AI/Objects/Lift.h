#ifndef LEMBALL_AI_OBJECTS_LIFT_H
#define LEMBALL_AI_OBJECTS_LIFT_H

#include "../Base/Coord3d.h"
#include "../Base/GlobalGameObject.h" // complete type

class AiCoord;
class GameObject;
class Ground;
enum eLiftActivateType {
	LIFT_ACTIVATE_SWITCH_TOGGLE = 0,
	LIFT_ACTIVATE_STEP = 1,
	LIFT_ACTIVATE_CONTINUOUS = 2,
	LIFT_ACTIVATE_SWITCH_ONCE = 3,
	LIFT_ACTIVATE_STEP_ONCE = 4
};

// SIZE 0x190
// VTABLE: LEMBALL 0x00495d60
class Lift : public GlobalGameObject {
public:
	Lift();
	int Activate();
	int StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual ~Lift();           // vtable+0x00
	void ActivateDeactivate();
	void CalculateCliff();
	void CheckObjects();
	void Edit(int p_height,
			  short p_direction,
			  int p_lowHeight,
			  int p_highHeight,
			  eLiftActivateType p_activateType,
			  unsigned int p_initialActive);
	void Set(const Coord3d& p_start,
			 const Coord3d& p_end,
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

	friend class LiftManager;

private:
	unsigned short m_liftId;          // 0x138
	Coord3d m_start;                  // 0x13a
	Coord3d m_end;                    // 0x140
	int m_lowHeight;                  // 0x148
	int m_highHeight;                 // 0x14c
	int m_movementStartHeight;        // 0x150
	short m_direction;                // 0x154
	unsigned int m_unk0x158;          // 0x158
	eLiftActivateType m_activateType; // 0x15c
	Ground* m_mapCell;                // 0x160
	unsigned int m_active;            // 0x164
	unsigned int m_defaultActive;     // 0x168
	unsigned int m_activationLatched; // 0x16c
	GameObject* m_objects[8];         // 0x170
};

// SYNTHETIC: LEMBALL 0x00426710
// Lift::`vector deleting destructor'

#endif
