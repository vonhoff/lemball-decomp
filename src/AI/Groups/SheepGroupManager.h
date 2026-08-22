#ifndef LEMBALL_SCAFFOLD_AI_GROUPS_SHEEPGROUPMANAGER_H
#define LEMBALL_SCAFFOLD_AI_GROUPS_SHEEPGROUPMANAGER_H

#include "../../Common.h"
#include "GenericGroupManager.h" // complete type

// SIZE 0xb0
// VTABLE: LEMBALL 0x00494d70
class SheepGroupManager : public GenericGroupManager {
public:
	SheepGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	int Process();
	void LoadLevel(LoadSheepData* p_data, unsigned long p_dataSize, unsigned char p_skip);
	void Restart();
};

#endif
