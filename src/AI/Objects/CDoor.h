#ifndef LEMBALL_AI_OBJECTS_CDOOR_H
#define LEMBALL_AI_OBJECTS_CDOOR_H

#include "../Base/CGlobalGameObject.h" // complete type
#include "../Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"

class AiCoord;
class CGameObject;
// SIZE 0x14c
// VTABLE: LEMBALL 0x004936f8
class CDoor : public CGlobalGameObject {
public:
	CDoor();
	int Hits(const AiCoord& p_position, CGameObject* p_object);
	virtual bool IsUsable(eAction p_action); // vtable+0xb4
	bool TryBeginActivation();
	virtual bool Process();    // vtable+0x14
	virtual void Delete();     // vtable+0xc8
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	void ResetCollision();
	void Set(eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z);
	void SetCollision();
	void Unlock();

	friend class CDoorManager;

private:
	unsigned short m_doorIndex;       // 0x138
	unsigned int m_setTick;           // 0x13c
	short m_doorType;                 // 0x140
	unsigned int m_activationPending; // 0x144
	unsigned int m_unk0x148;          // 0x148
};

// SYNTHETIC: LEMBALL 0x0040ec10
// CDoor::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x0040eca0
// CDoor::~CDoor

#endif
