#include "SheepGroupManager.h"

#include "../Base/LoadSheepData.h"
#include "../Managers/ObjectManager.h"
#include "../Navigation/Ai.h"
#include "../Objects/Sheep.h"
#include "FormationManager.h"
#include "GenericGroup.h"
#include "SheepGroup.h"

// 68K 0x1061e91c __ct__18CSheepGroupManagerFP3CAIP14CObjectManagerP17CFormationManager
// STUB: LEMBALL 0x0041f0b0
SheepGroupManager::SheepGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
{
}

// 68K 0x1061e998 Restart__18CSheepGroupManagerFv
// STUB: LEMBALL 0x0041f0e0
void SheepGroupManager::Restart()
{
}

// 68K 0x1061ea1c Process__18CSheepGroupManagerFv
// FUNCTION: LEMBALL 0x0041f140
int SheepGroupManager::Process()
{
	GenericGroup* group = GetFirstGroup();
	while (group != 0) {
		group->Process();
		group = GetNextGroup();
	}
	return 0;
}

// 68K 0x1061ea8c LoadLevel__18CSheepGroupManagerFP16tagLoadSheepDataUlUc
// FUNCTION: LEMBALL 0x0041f2e0
void SheepGroupManager::LoadLevel(LoadSheepData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	int count = p_dataSize / sizeof(LoadSheepData);
	if (*(unsigned int*) &p_skip != 0) {
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

		SheepGroup* group =
			new SheepGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
		group->Restart();
		GenericGroupManager::AddNewGroup(group);
		group->SetFormationIndex(formationIndex);

		for (int i = 0; i < sheepCount; i++) {
			Sheep* sheep = new Sheep(g_pGenericGroupAI, x - i * 20, y - i * 20, 0, 0);
			sheep->Restart();
			GenericGroupManager::AddElementToGroup(sheep, group);
		}

		group->ReformAlteredGroup(g_pGenericGroupFormationManager);
		p_data++;
	}
}
