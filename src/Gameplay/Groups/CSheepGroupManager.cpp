#include "CSheepGroupManager.h"

#include "Level/tagLoadSheepData.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Characters/CSheep.h"
#include "Gameplay/Objects/CGameObject.h"
#include "CGenericGroupManager.h"
#include "CFormationManager.h"
#include "CGenericGroup.h"
#include "CSheepGroup.h"

#define SHEEP_INITIAL_SPACING 20

// FUNCTION: LEMBALL 0x0041f0b0
CSheepGroupManager::CSheepGroupManager(CAI* p_ai,
									   CObjectManager* p_objectManager,
									   CFormationManager* p_formationManager)
	: CGenericGroupManager(p_ai, p_objectManager, p_formationManager)
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
	while (group != NULL) {
		group->Process();
		group = GetNextGroup();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041f170
void CSheepGroupManager::AddSheepGroup(int p_x, int p_y, int p_z)
{
	CSheepGroup* group =
		new CSheepGroup(g_pGenericGroupAI, g_pGenericGroupObjectManager, g_pGenericGroupFormationManager);
	group->Restart();
	group->SetFormationIndex(1);
	CGenericGroupManager::AddNewGroup(group);

	for (int i = 0; i < 4; i++) {
		CSheep* sheep = new CSheep(g_pGenericGroupAI, p_x, p_y, p_z, 0);
		p_x -= 12;
		p_y -= 12;
		sheep->Restart();
		CGenericGroupManager::AddElementToGroup(sheep, group);
	}

	group->ReformAlteredGroup(g_pGenericGroupFormationManager);
}

// FUNCTION: LEMBALL 0x0041f250
void CSheepGroupManager::RemoveSheepGroup(CSheepGroup* p_group)
{
	int index = 0;
	FindElementInGroupAndRemoveIt(p_group);

	int& count = g_pGenericGroupAI->m_objectCount;
	int originalCount = count;
	if (index < originalCount) {
		CGameObject** object = g_pGenericGroupAI->m_objects;
		do {
			if (*object == p_group) {
				count--;
				if (index < count) {
					int offset = index * sizeof(*object);
					do {
						index++;
						object = (CGameObject**) ((char*) g_pGenericGroupAI->m_objects + offset);
						offset += sizeof(*object);
						*object = object[1];
					} while (index < count);
				}
				g_pGenericGroupAI->m_objects[count] = NULL;
				break;
			}
			object++;
			index++;
		} while (index < originalCount);
	}

	p_group->Delete();
	delete p_group;
}

// FUNCTION: LEMBALL 0x0041f2e0
void CSheepGroupManager::LoadLevel(tagLoadSheepData* p_data, unsigned long p_dataSize, unsigned int p_skip)
{
	int count = p_dataSize / sizeof(tagLoadSheepData);
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
			CSheep* sheep =
				new CSheep(g_pGenericGroupAI, x - i * SHEEP_INITIAL_SPACING, y - i * SHEEP_INITIAL_SPACING, 0, 0);
			sheep->Restart();
			CGenericGroupManager::AddElementToGroup(sheep, group);
		}

		group->ReformAlteredGroup(g_pGenericGroupFormationManager);
		p_data++;
	}
}
