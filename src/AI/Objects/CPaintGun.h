#ifndef LEMBALL_AI_OBJECTS_CPAINTGUN_H
#define LEMBALL_AI_OBJECTS_CPAINTGUN_H

#include "../Base/CGlobalGameObject.h"

class AiCoord;
// SIZE 0x144
// VTABLE: LEMBALL 0x00496740
class CPaintGun : public CGlobalGameObject {
public:
	CPaintGun();
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~CPaintGun();      // vtable+0x00
	void Set(unsigned short p_id, const AiCoord& p_position, int p_direction);

	friend class CPaintGunManager;

private:
	unsigned int m_enabled; // 0x138
	unsigned int m_active;  // 0x13c
	int m_direction;        // 0x140
};

// SYNTHETIC: LEMBALL 0x0042c9e0
// CPaintGun::`vector deleting destructor'

#endif
