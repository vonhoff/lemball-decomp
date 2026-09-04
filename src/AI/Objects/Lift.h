#ifndef LEMBALL_AI_OBJECTS_LIFT_H
#define LEMBALL_AI_OBJECTS_LIFT_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

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
			  unsigned char p_initialActive);
	void Set(const Coord3d& p_start,
			 const Coord3d& p_end,
			 short p_direction,
			 int p_lowHeight,
			 int p_highHeight,
			 eLiftActivateType p_activateType,
			 unsigned char p_initialActive);
	void Set(int p_x,
			 int p_y,
			 int p_z,
			 short p_direction,
			 int p_lowHeight,
			 int p_highHeight,
			 eLiftActivateType p_activateType,
			 unsigned char p_initialActive);

	friend class LiftManager;

private:
	unsigned short m_liftId;          // 0x138
	short m_startX;                   // 0x13a
	short m_startY;                   // 0x13c
	short m_currentHeight;            // 0x13e
	short m_endX;                     // 0x140
	short m_endY;                     // 0x142
	short m_endZ;                     // 0x144
	int m_lowHeight;                  // 0x148
	int m_highHeight;                 // 0x14c
	int m_movementStartHeight;        // 0x150
	short m_direction;                // 0x154
	undefined4 m_unk0x158;            // 0x158
	eLiftActivateType m_activateType; // 0x15c
	void* m_mapCell;                  // 0x160
	unsigned int m_active;            // 0x164
	unsigned int m_defaultActive;     // 0x168
	unsigned int m_activationLatched; // 0x16c
	GameObject* m_objects[8];         // 0x170
};

// SYNTHETIC: LEMBALL 0x00426710
// Lift::`scalar deleting destructor'

#endif
