#ifndef LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H

#include "CGenericGroupManager.h" // complete type

class CAI;
class CFormationManager;
class CObjectManager;
struct LoadSheepData;
// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d70
class CSheepGroupManager : public CGenericGroupManager {
public:
	CSheepGroupManager(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
	int Process();
	void LoadLevel(LoadSheepData* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
};

#endif
