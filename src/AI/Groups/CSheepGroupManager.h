#ifndef LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H
#define LEMBALL_AI_GROUPS_CSHEEPGROUPMANAGER_H

#include "CGenericGroupManager.h" // complete type

class CAi;
class CFormationManager;
class CObjectManager;
struct LoadSheepData;
// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d70
class CSheepGroupManager : public CGenericGroupManager {
public:
	CSheepGroupManager(CAi* p_arg0, CObjectManager* p_arg1, CFormationManager* p_arg2);
	int Process();
	void LoadLevel(LoadSheepData* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
};

#endif
