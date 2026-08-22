#ifndef LEMBALL_AI_OBJECTS_ROCKET_H
#define LEMBALL_AI_OBJECTS_ROCKET_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x00495ed8
class Rocket : public GlobalGameObject {
public:
	Rocket();
	int StepOn(const AiCoord& p_position, GameObject* p_object);
	virtual bool Process();                         // vtable+0x14
	virtual void DoActivate();                      // vtable+0x10c
	virtual void GetViewData(ViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                         // vtable+0x104
	virtual ~Rocket();                              // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position);

private:
	unsigned int m_enabled; // 0x138
	unsigned int m_active;  // 0x13c
	int m_launchBaseZ;      // 0x140
};

#endif
