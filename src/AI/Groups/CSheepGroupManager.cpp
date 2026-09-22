#include "CSheepGroupManager.h"

#include "../Base/LoadSheepData.h"
#include "../Managers/CObjectManager.h"
#include "../Navigation/CAi.h"
#include "../Objects/CSheep.h"
#include "AI/Base/CGameObject.h"
#include "AI/Groups/CGenericGroupManager.h"
#include "CFormationManager.h"
#include "CGenericGroup.h"
#include "CSheepGroup.h"

// FUNCTION: LEMBALL 0x0041f0b0
CSheepGroupManager::CSheepGroupManager(CAi* p_arg0, CObjectManager* p_arg1, CFormationManager* p_arg2)
	: CGenericGroupManager(p_arg0, p_arg1, p_arg2)
{
}

// FUNCTION: LEMBALL 0x0041f0e0
void CSheepGroupManager::Restart()
{
	CGenericGroup** group;
	int groupIndex = 0;
	if (groupIndex < m_groupCount) {
		group = m_groups;
		do {
			int elementCount = (*group)->GetElementsInGroup();
			int elementIndex = 0;
			if (elementCount > 0) {
				do {
					(*group)->GetNthElementInGroup(elementIndex)->Restart();
					elementIndex++;
				} while (elementIndex < elementCount);
			}
			group++;
			groupIndex++;
		} while (groupIndex < m_groupCount);
	}
}

// FUNCTION: LEMBALL 0x0041f140
int CSheepGroupManager::Process()
{
	CGenericGroup* group = GetFirstGroup();
	while (group != 0) {
		group->Process();
		group = GetNextGroup();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041f2e0
void CSheepGroupManager::LoadLevel(LoadSheepData* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	int count = p_dataSize / sizeof(LoadSheepData);
	if (p_skip != 0) {
		return;
	}

	for (int record = 0; record < count; record++) {
		int sheepCount;
		int x;
		int y;
		int formationIndex;

		sheepCount = p_data->m_sheepCount;
		x = p_data->m_x;
		y = p_data->m_y;
		formationIndex = p_data->m_formationIndex;

		CSheepGroup* group =
			new CSheepGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		group->Restart();
		CGenericGroupManager::AddNewGroup(group);
		group->SetFormationIndex(formationIndex);

		for (int i = 0; i < sheepCount; i++) {
			CSheep* sheep = new CSheep(g_pGenericGroupAI, x - i * 20, y - i * 20, 0, 0);
			sheep->Restart();
			CGenericGroupManager::AddElementToGroup(sheep, group);
		}

		group->ReformAlteredGroup(g_pGenericGroupFormationManager);
		p_data++;
	}
}
