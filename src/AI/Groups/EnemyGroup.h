#ifndef LEMBALL_AI_GROUPS_ENEMYGROUP_H
#define LEMBALL_AI_GROUPS_ENEMYGROUP_H

#include "GenericGroup.h" // complete type

class Ai;
class FormationManager;
class ObjectManager;
// SIZE 0x168
// VTABLE: LEMBALL 0x00495250
class EnemyGroup : public GenericGroup {
public:
	EnemyGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager);
};

// SYNTHETIC: LEMBALL 0x00420b20
// EnemyGroup::`scalar deleting destructor'

#endif
