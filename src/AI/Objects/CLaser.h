#ifndef LEMBALL_AI_OBJECTS_CLASER_H
#define LEMBALL_AI_OBJECTS_CLASER_H

#include "../Base/CGlobalGameObject.h"
#include "AI/Base/ObjectTypes.h"

class AiCoord;
class CGameObject;
class CViewData;
// SIZE 0x148
// VTABLE: LEMBALL 0x00496310
class CLaser : public CGlobalGameObject {
public:
	CLaser();
	bool Activate();
	bool CheckHits();
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	int GetViewData(CViewData* p_viewData);
	virtual void Restart(); // vtable+0x104
	virtual ~CLaser();      // vtable+0x00
	void Initialise();
	void Set(unsigned short p_id, const AiCoord& p_position, eObjectType p_orientation);

	friend class CLaserManager;

private:
	unsigned int m_active;       // 0x138
	unsigned int m_enabled;      // 0x13c
	unsigned int m_autoActivate; // 0x140
	CGameObject* m_target;       // 0x144
};

// SYNTHETIC: LEMBALL 0x00429ed0
// CLaser::`vector deleting destructor'

#endif
