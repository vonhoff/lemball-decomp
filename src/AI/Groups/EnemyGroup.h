#ifndef LEMBALL_SCAFFOLD_AI_GROUPS_ENEMYGROUP_H
#define LEMBALL_SCAFFOLD_AI_GROUPS_ENEMYGROUP_H

#include "../../Common.h"
#include "GenericGroup.h" // complete type

// SIZE 0x168
// VTABLE: LEMBALL 0x00495250
class EnemyGroup : public GenericGroup {
public:
	EnemyGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2);
	virtual ~EnemyGroup(); // vtable+0x00
};

#endif
