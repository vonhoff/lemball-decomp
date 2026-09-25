#ifndef LEMBALL_AI_OBJECTS_CHAND_H
#define LEMBALL_AI_OBJECTS_CHAND_H

#include "../Base/CGlobalGameObject.h"

class AiCoord;
class CGameObject;
// SIZE 0x144
// VTABLE: LEMBALL 0x00496198
class CHand : public CGlobalGameObject {
public:
	CHand();
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~CHand();          // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position);

	friend class CHandManager;

private:
	unsigned int m_activated; // 0x138
	unsigned int m_enabled;   // 0x13c
	CGameObject* m_target;    // 0x140
};

// SYNTHETIC: LEMBALL 0x00428800
// CHand::`vector deleting destructor'

#endif
