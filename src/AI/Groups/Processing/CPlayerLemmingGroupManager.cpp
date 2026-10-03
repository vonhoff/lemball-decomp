#include "../CPlayerLemmingGroupManager.h"

#include "../../Objects/CPlayerLemming.h"
#include "../CPlayerLemmingGroup.h"

// FUNCTION: LEMBALL 0x00418650
void CPlayerLemmingGroupManager::Process()
{
	CPlayerLemming* lemming;
	CGenericGroupManager* manager = this;
	bool controlledGroupDeleted = false;
	CGenericGroup* genericGroup = manager->CGenericGroupManager::GetFirstGroup();
	while (genericGroup != NULL) {
		genericGroup->Process();
		genericGroup = manager->CGenericGroupManager::GetNextGroup();
	}

	CPlayerLemmingGroup* group = (CPlayerLemmingGroup*) manager->CGenericGroupManager::GetFirstGroup();
	while (group != NULL) {
		lemming = group->GetFirstDeadLemming();
		if (lemming != NULL) {
			do {
				group->RemoveLemmingFromGroup(lemming);
				m_dead[m_deadCount] = lemming;
				m_deadCount++;
				if (group->GetElementsInGroup() == 0) {
					lemming = NULL;
					if (GetPlayerControlledGroup() == group) {
						controlledGroupDeleted = true;
					}
					DeleteGroup(group);
				}
				else {
					lemming = group->GetFirstDeadLemming();
				}
			} while (lemming != NULL);
		}
		group = (CPlayerLemmingGroup*) manager->CGenericGroupManager::GetNextGroup();
	}
	if (controlledGroupDeleted) {
		MakePreviousGroupPlayerControlled();
	}
	ProcessDead();
}
