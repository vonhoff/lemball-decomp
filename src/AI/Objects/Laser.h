#ifndef LEMBALL_AI_OBJECTS_LASER_H
#define LEMBALL_AI_OBJECTS_LASER_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x148
// VTABLE: LEMBALL 0x00496310
class Laser : public GlobalGameObject {
public:
	Laser();
	bool Activate();
	bool CheckHits();
	bool StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual bool Process();                         // vtable+0x14
	virtual void DoActivate();                      // vtable+0x10c
	virtual void GetViewData(ViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                         // vtable+0x104
	virtual ~Laser();                               // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position, eObjectType p_orientation);

private:
	unsigned int m_active;       // 0x138
	unsigned int m_enabled;      // 0x13c
	unsigned int m_autoActivate; // 0x140
	GameObject* m_target;        // 0x144
};

// SYNTHETIC: LEMBALL 0x00429ed0
// Laser::`vector deleting destructor'


#endif

