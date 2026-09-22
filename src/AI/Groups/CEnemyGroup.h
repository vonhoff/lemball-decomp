#ifndef LEMBALL_AI_GROUPS_CENEMYGROUP_H
#define LEMBALL_AI_GROUPS_CENEMYGROUP_H

#include "CGenericGroup.h" // complete type

class CAi;
class CFormationManager;
class CObjectManager;
// SIZE 0x168
// VTABLE: LEMBALL 0x00495250
class CEnemyGroup : public CGenericGroup {
public:
	CEnemyGroup(CAi* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager);
};

// SYNTHETIC: LEMBALL 0x00420b20
// CEnemyGroup::`scalar deleting destructor'

#endif
