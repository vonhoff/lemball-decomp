#include "EnemyGroup.h"

#include "AI/Groups/GenericGroup.h"

// 68K 0x106088ac __ct__11CEnemyGroupFP3CAIP14CObjectManagerP17CFormationManager
// FUNCTION: LEMBALL 0x00420ae0
EnemyGroup::EnemyGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager)
	: GenericGroup(p_ai, p_objectManager, p_formationManager)
{
}
