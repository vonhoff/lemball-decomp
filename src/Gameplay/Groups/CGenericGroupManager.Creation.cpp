#include "CGenericGroupManager.h"

#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "CFormationManager.h"
#include "CGenericGroup.h"

// FUNCTION: LEMBALL 0x0041ebe0
void CGenericGroupManager::CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds)
{
	if (m_groupCount < GENERIC_GROUP_CAPACITY) {
		CGenericGroup* group =
			new CGenericGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		m_groups[m_groupCount] = group;
		m_groupCount++;
		for (unsigned int index = 0; index < p_count; index++) {
			unsigned short objectId = *p_objectIds;
			p_objectIds++;
			AddElementToGroup(g_pObjects[objectId], group);
		}
	}
}
