#ifndef LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H

#include "CGenericGroupManager.h"

class CAI;
class CFormationManager;
class CSheepGroup;
class CObjectManager;
struct tagLoadSheepData;
// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d70
class CSheepGroupManager : public CGenericGroupManager {
public:
	CSheepGroupManager(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	void AddSheepGroup(int p_x, int p_y, int p_z);
	void RemoveSheepGroup(CSheepGroup* p_group);
	int Process();
	void LoadLevel(tagLoadSheepData* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
};

#endif
