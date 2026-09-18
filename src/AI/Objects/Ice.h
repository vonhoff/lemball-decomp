#ifndef LEMBALL_AI_OBJECTS_ICE_H
#define LEMBALL_AI_OBJECTS_ICE_H

#include "../Base/Coord3d.h"
#include "../Base/GlobalGameObject.h" // complete type

class AiCoord;
class GameObject;
class IceManager;
class PlayerLemming;
class Switch;
// SIZE 0x188
// VTABLE: LEMBALL 0x004968b8
class Ice : public GlobalGameObject {
public:
	Ice();
	bool StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual bool Process(); // vtable+0x14
	virtual void Restart(); // vtable+0x104
	virtual ~Ice();         // vtable+0x00
	void Initialise();
	void Leave(PlayerLemming* p_lemming);
	void Set(unsigned short p_id,
			 const Coord3d& p_cornerA,
			 const Coord3d& p_cornerB,
			 int p_velocityX,
			 int p_velocityY,
			 unsigned int p_initialSwitched);
	void Switch();
	void Switched();

	friend class IceManager;

private:
	unsigned int m_switched;        // 0x138
	unsigned int m_enabled;         // 0x13c
	unsigned int m_unk0x140;        // 0x140
	int m_velocityX;                // 0x144
	int m_velocityY;                // 0x148
	unsigned int m_initialSwitched; // 0x14c
	int m_objectCount;              // 0x150
	GameObject* m_objects[10];      // 0x154
	Coord3d m_min;                  // 0x17c
	Coord3d m_max;                  // 0x182
};

// SYNTHETIC: LEMBALL 0x0042e500
// Ice::`vector deleting destructor'

#endif
