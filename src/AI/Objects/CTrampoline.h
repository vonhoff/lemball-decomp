#ifndef LEMBALL_AI_OBJECTS_CTRAMPOLINE_H
#define LEMBALL_AI_OBJECTS_CTRAMPOLINE_H

#include "../Base/CGlobalGameObject.h"

class AiCoord;
class CGameObject;
// SIZE 0x144
// VTABLE: LEMBALL 0x004965c8
class CTrampoline : public CGlobalGameObject {
public:
	CTrampoline();
	int Hit(const AiCoord& p_position, CGameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~CTrampoline();    // vtable+0x00
	void Set(unsigned short p_id, const AiCoord& p_position);
	int TryEnableNearPosition(const AiCoord& p_position, CGameObject* p_object);

	friend class CTrampolineManager;

private:
	unsigned int m_enabled; // 0x138
	unsigned int m_active;  // 0x13c
	int m_relocationZ;      // 0x140
};

// SYNTHETIC: LEMBALL 0x0042ba40
// CTrampoline::`vector deleting destructor'

#endif
