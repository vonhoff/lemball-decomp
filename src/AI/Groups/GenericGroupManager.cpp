#include "GenericGroupManager.h"

#include "GenericGroup.h"

GenericGroupManager::GenericGroupManager()
{
}

// 68K 0x1060d1da __ct__20CGenericGroupManagerFP3CAIP14CObjectManagerP17CFormationManager
// STUB: LEMBALL 0x0041e8f0
GenericGroupManager::GenericGroupManager(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
{
}

// 68K 0x1060d272 __dt__20CGenericGroupManagerFv
GenericGroupManager::~GenericGroupManager()
{
}

// 68K 0x1060d2f6 Restart__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041e970
void GenericGroupManager::Restart()
{
	GenericGroup* group = GetFirstGroup();
	while (group != 0) {
		group->Restart();
		group = GetNextGroup();
	}
}

// 68K 0x1060d368 Process__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041e9f0
int GenericGroupManager::Process()
{
	GenericGroup* group = GetFirstGroup();
	while (group != 0) {
		group->Process();
		group = GetNextGroup();
	}
	return 0;
}

// 68K 0x1060d3dc GetNumberOfGroups__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041ea20
int GenericGroupManager::GetNumberOfGroups()
{
	return m_groupCount;
}

// 68K 0x1060d41c GetFirstGroup__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041ea30
GenericGroup* GenericGroupManager::GetFirstGroup()
{
	m_currentGroup = 0;
	if (m_groupCount == 0) {
		return 0;
	}
	return m_groups[0];
}

// 68K 0x1060d464 GetNextGroup__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041ea50
GenericGroup* GenericGroupManager::GetNextGroup()
{
	int index = m_currentGroup + 1;
	m_currentGroup = index;
	if (m_groupCount <= index) {
		return 0;
	}
	return m_groups[index];
}

// 68K 0x1060d4b4 GetNthGroup__20CGenericGroupManagerFi
// FUNCTION: LEMBALL 0x0041ea70
GenericGroup* GenericGroupManager::GetNthGroup(int p_index)
{
	m_currentGroup = p_index;
	if (m_groupCount <= p_index) {
		return 0;
	}
	return m_groups[p_index];
}

// 68K 0x1060d506 GetCurrentGroup__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041ea90
GenericGroup* GenericGroupManager::GetCurrentGroup()
{
	if (m_groupCount <= m_currentGroup) {
		return 0;
	}
	return m_groups[m_currentGroup];
}

// 68K 0x1060d556 GetNumberOfElements__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041eab0
int GenericGroupManager::GetNumberOfElements()
{
	int total = 0;
	GenericGroup* group = GetFirstGroup();
	if (group != 0) {
		do {
			total += group->GetElementsInGroup();
			group = GetNextGroup();
		} while (group != 0);
	}
	return total;
}

// 68K 0x1060d5da GetFirstElement__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041eae0
GameObject* GenericGroupManager::GetFirstElement()
{
	GenericGroup* group = GetFirstGroup();
	if (group != 0) {
		return group->GetFirstElementInGroup();
	}
	return 0;
}

// 68K 0x1060d640 GetNextElement__20CGenericGroupManagerFv
// STUB: LEMBALL 0x0041eb00
GameObject* GenericGroupManager::GetNextElement()
{
	return 0;
}

// 68K 0x1060d6e2 GetCurrentElement__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041eb40
GameObject* GenericGroupManager::GetCurrentElement()
{
	GameObject* object = 0;
	GenericGroup* group = GetCurrentGroup();
	if (group != 0) {
		object = group->GetCurrentElementInGroup();
	}
	return object;
}

// 68K 0x1060d750 GetNthElement__20CGenericGroupManagerFi
// STUB: LEMBALL 0x0041eb60
GameObject* GenericGroupManager::GetNthElement(int p_index)
{
	return 0;
}

// 68K 0x1060d7c8 GetGroupElementIsMemberOf__20CGenericGroupManagerFP11CGameObject
// STUB: LEMBALL 0x0041eb90
GenericGroup* GenericGroupManager::GetGroupElementIsMemberOf(GameObject* p_object)
{
	return 0;
}

// 68K 0x1060d86a CreateNewGroup__20CGenericGroupManagerFUsPUs
// STUB: LEMBALL 0x0041ebe0
void GenericGroupManager::CreateNewGroup(unsigned short p_count, unsigned short* p_objectIds)
{
}

// 68K 0x1060d93e AddNewGroup__20CGenericGroupManagerFP13CGenericGroup
// FUNCTION: LEMBALL 0x0041ec80
void GenericGroupManager::AddNewGroup(GenericGroup* p_group)
{
	int count = m_groupCount;

	if (count < 0x28) {
		m_groups[count] = p_group;
		m_groupCount = m_groupCount + 1;
	}
}

// 68K 0x1060d998 CreateNewGroup__20CGenericGroupManagerFv
// FUNCTION: LEMBALL 0x0041eca0
void GenericGroupManager::CreateNewGroup()
{
}

// 68K 0x1060d9cc DeleteGroup__20CGenericGroupManagerFP13CGenericGroup
// STUB: LEMBALL 0x0041ecb0
void GenericGroupManager::DeleteGroup(GenericGroup* p_group)
{
}

// 68K 0x1060da66 AddElementToGroup__20CGenericGroupManagerFP11CGameObjectP13CGenericGroup
// FUNCTION: LEMBALL 0x0041ed20
void GenericGroupManager::AddElementToGroup(GameObject* p_object, GenericGroup* p_group)
{
	FindElementInGroupAndRemoveIt(p_object);
	p_group->AddElementToGroup(p_object);
}

// 68K 0x1060dae2 RemoveElementFromGroup__20CGenericGroupManagerFP11CGameObjectP13CGenericGroup
// STUB: LEMBALL 0x0041ed40
bool GenericGroupManager::RemoveElementFromGroup(GameObject* p_object, GenericGroup* p_group)
{
	return 0;
}

// 68K 0x1060dbd2 FindElementInGroupAndRemoveIt__20CGenericGroupManagerFP11CGameObject
// FUNCTION: LEMBALL 0x0041ede0
void GenericGroupManager::FindElementInGroupAndRemoveIt(GameObject* p_object)
{
	RemoveElementFromGroup(p_object, GetGroupElementIsMemberOf(p_object));
}

// 68K 0x1060dc5a GetAllBoundingBoxes__20CGenericGroupManagerFP5tRect
// STUB: LEMBALL 0x0041ee00
int GenericGroupManager::GetAllBoundingBoxes(Rect* p_rects)
{
	return 0;
}

// 68K 0x1060dd46 GetViewData__20CGenericGroupManagerFP9CViewData
// STUB: LEMBALL 0x0041ee90
int GenericGroupManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1060ddde CheckGroupIntersection__20CGenericGroupManagerFP7CVSRectP7AICOORD
// STUB: LEMBALL 0x0041eed0
bool GenericGroupManager::CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return 0;
}
