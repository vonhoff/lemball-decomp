#ifndef LEMBALL_AI_OBJECTS_CICE_H
#define LEMBALL_AI_OBJECTS_CICE_H

#include "../Base/CGlobalGameObject.h"
#include "../Base/Coord3d.h"

class AiCoord;
class CGameObject;
class CIceManager;
class CPlayerLemming;
class CSwitch;
// SIZE 0x188
// VTABLE: LEMBALL 0x004968b8
class CIce : public CGlobalGameObject {
public:
	CIce();
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual bool Process(); // vtable+0x14
	virtual void Restart(); // vtable+0x104
	virtual ~CIce();        // vtable+0x00
	void Initialise();
	void Leave(CPlayerLemming* p_lemming);
	void Set(unsigned short p_id,
			 const Coord3d& p_cornerA,
			 const Coord3d& p_cornerB,
			 int p_velocityX,
			 int p_velocityY,
			 unsigned int p_initialSwitched);
	void Switch();
	void Switched();

	friend class CIceManager;

private:
	unsigned int m_switched;        // 0x138
	unsigned int m_enabled;         // 0x13c
	unsigned int m_unk0x140;        // 0x140
	int m_velocityX;                // 0x144
	int m_velocityY;                // 0x148
	unsigned int m_initialSwitched; // 0x14c
	int m_objectCount;              // 0x150
	CGameObject* m_objects[10];     // 0x154
	Coord3d m_min;                  // 0x17c
	Coord3d m_max;                  // 0x182
};

// SYNTHETIC: LEMBALL 0x0042e500
// CIce::`vector deleting destructor'

#endif
