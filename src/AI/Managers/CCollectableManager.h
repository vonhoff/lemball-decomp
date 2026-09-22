#ifndef LEMBALL_AI_MANAGERS_CCOLLECTABLEMANAGER_H
#define LEMBALL_AI_MANAGERS_CCOLLECTABLEMANAGER_H

#include "../Base/ObjectTypes.h"
#include "CBaseObjectManager.h" // complete type

class CAi;
class CCollectable;
class CGameObject;
// SIZE 0x40
// VTABLE: LEMBALL 0x004955a0
class CCollectableManager : public CBaseObjectManager {
public:
	CCollectableManager(CAi* p_arg0, int p_arg1);
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual ~CCollectableManager();                 // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_type);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip);
	void Process();
	void Restart();
	void RemoveCollectable(CGameObject* p_object);

private:
	CAi* m_ai;                     // 0x30
	CCollectable** m_collectables; // 0x34
	int m_capacity;                // 0x38
	int m_count;                   // 0x3c
};

// SYNTHETIC: LEMBALL 0x00422f70
// CCollectableManager::`scalar deleting destructor'

#endif
