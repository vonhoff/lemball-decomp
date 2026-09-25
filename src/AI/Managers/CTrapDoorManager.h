#ifndef LEMBALL_AI_MANAGERS_CTRAPDOORMANAGER_H
#define LEMBALL_AI_MANAGERS_CTRAPDOORMANAGER_H

#include "CBaseObjectManager.h"

class AiCoord;
class CTrapDoor;
// SIZE 0x54
// VTABLE: LEMBALL 0x004936c8
class CTrapDoorManager : public CBaseObjectManager {
public:
	CTrapDoorManager();
	virtual int GetViewData(CViewData* p_viewData); // vtable+0x24
	virtual void Process();                         // vtable+0x1c
	virtual ~CTrapDoorManager();                    // vtable+0x14
	void Restart();
	void ClearAllTrapDoors();
	void AddNewDoor(unsigned short p_id, const AiCoord& p_position, unsigned int p_mode, unsigned long p_deadline);
	int GetTrapDoorPosition(AiCoord& p_position, int p_index);
	void SetTrapDoorPosition(int p_x, int p_y, int p_z, int p_index);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned int p_skip);

private:
	CTrapDoor* m_doors[8]; // 0x30
	int m_count;           // 0x50
};

// SYNTHETIC: LEMBALL 0x0040ced0
// CTrapDoorManager::`scalar deleting destructor'

#endif
