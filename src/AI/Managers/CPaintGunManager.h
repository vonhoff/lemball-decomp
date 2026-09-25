#ifndef LEMBALL_AI_MANAGERS_CPAINTGUNMANAGER_H
#define LEMBALL_AI_MANAGERS_CPAINTGUNMANAGER_H

#include "CBaseObjectManager.h"

class CAI;
class CPaintGun;
// SIZE 0x40
// VTABLE: LEMBALL 0x00496888
class CPaintGunManager : public CBaseObjectManager {
public:
	CPaintGunManager(CAI* p_ai, int p_capacity);
	int GetViewData(CViewData* p_viewData);
	virtual ~CPaintGunManager(); // vtable+0x14
	void Add(unsigned short p_id, int p_x, int p_y, int p_z, int p_direction);
	void Initialise(int p_capacity);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Process();
	void Restart();
	void ResetCount();

private:
	int m_capacity;         // 0x30
	int m_count;            // 0x34
	CPaintGun* m_paintGuns; // 0x38
	CAI* m_ai;              // 0x3c
};

// SYNTHETIC: LEMBALL 0x0042c9b0
// CPaintGunManager::`scalar deleting destructor'

#endif
