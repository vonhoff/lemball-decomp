#ifndef LEMBALL_AI_MANAGERS_CLASERMANAGER_H
#define LEMBALL_AI_MANAGERS_CLASERMANAGER_H

#include "../Base/ObjectTypes.h"
#include "CBaseObjectManager.h" // complete type

class CAI;
class AiCoord;
class CGameObject;
class CLaser;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496458
class CLaserManager : public CBaseObjectManager {
public:
	CLaserManager(CAI* p_ai, int p_capacity);
	bool StepOn(const AiCoord& p_position, CGameObject* p_object);
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual void Process();                         // vtable+0x1c
	virtual ~CLaserManager();                       // vtable+0x14
	void Restart();
	void Add(unsigned short p_id, int p_x, int p_y, int p_z, eObjectType p_orientation);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);

	friend class CAI;

private:
	int m_capacity;   // 0x30
	int m_count;      // 0x34
	CLaser* m_lasers; // 0x38
	CAI* m_ai;        // 0x3c
};

// SYNTHETIC: LEMBALL 0x00429ea0
// CLaserManager::`scalar deleting destructor'

#endif
