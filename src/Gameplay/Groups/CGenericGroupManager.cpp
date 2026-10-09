#include "CGenericGroupManager.h"

#include "CFormationManager.h"
#include "CGenericGroup.h"
#include "Engine/Math/CVSRect.h"
#include "Gameplay/Geometry/tRect.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Simulation/CAI.h"

#include <string.h>

#pragma intrinsic(memset)

// FUNCTION: LEMBALL 0x0041e8f0
CGenericGroupManager::CGenericGroupManager(CAI* p_ai,
										   CObjectManager* p_objectManager,
										   CFormationManager* p_formationManager)
{
	g_pGenericGroupAI = p_ai;
	g_pGenericGroupObjectManager = p_objectManager;
	g_pGenericGroupFormationManager = p_formationManager;
	m_groupCount = 0;
	m_currentGroup = 0;
	m_deleteEmptyGroups = true;
	memset(m_groups, 0, sizeof(m_groups));
}

// FUNCTION: LEMBALL 0x0041e940
CGenericGroupManager::~CGenericGroupManager()
{
	for (int i = 0; i < GENERIC_GROUP_CAPACITY; i++) {
		if (m_groups[i] != NULL) {
			delete m_groups[i];
		}
	}
}

// FUNCTION: LEMBALL 0x0041e970
void CGenericGroupManager::Restart()
{
	CGenericGroup* group = GetFirstGroup();
	while (group != NULL) {
		group->Restart();
		group = GetNextGroup();
	}
}

// FUNCTION: LEMBALL 0x0041e9a0
void CGenericGroupManager::ClearGroups()
{
	for (int index = 0; index < m_groupCount; ++index) {
		if (m_groups[index] != 0) {
			delete m_groups[index];
		}
		m_groups[index] = 0;
	}
	m_groupCount = 0;
	m_currentGroup = 0;
}

// FUNCTION: LEMBALL 0x0041e9f0
int CGenericGroupManager::Process()
{
	CGenericGroup* group = GetFirstGroup();
	while (group != NULL) {
		group->Process();
		group = GetNextGroup();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ea20
int CGenericGroupManager::GetNumberOfGroups()
{
	return m_groupCount;
}

// FUNCTION: LEMBALL 0x0041ea30
CGenericGroup* CGenericGroupManager::GetFirstGroup()
{
	m_currentGroup = 0;
	if (m_groupCount == 0) {
		return NULL;
	}
	return m_groups[0];
}

// FUNCTION: LEMBALL 0x0041ea50
CGenericGroup* CGenericGroupManager::GetNextGroup()
{
	int index = m_currentGroup + 1;
	m_currentGroup = index;
	if (m_groupCount <= index) {
		return NULL;
	}
	return m_groups[index];
}

// FUNCTION: LEMBALL 0x0041ea70
CGenericGroup* CGenericGroupManager::GetNthGroup(int p_index)
{
	m_currentGroup = p_index;
	if (m_groupCount <= p_index) {
		return NULL;
	}
	return m_groups[p_index];
}

// FUNCTION: LEMBALL 0x0041ea90
CGenericGroup* CGenericGroupManager::GetCurrentGroup()
{
	if (m_groupCount <= m_currentGroup) {
		return NULL;
	}
	return m_groups[m_currentGroup];
}

// FUNCTION: LEMBALL 0x0041eab0
int CGenericGroupManager::GetNumberOfElements()
{
	int total = 0;
	CGenericGroup* group = GetFirstGroup();
	if (group != NULL) {
		do {
			total += group->GetElementsInGroup();
			group = GetNextGroup();
		} while (group != NULL);
	}
	return total;
}

// FUNCTION: LEMBALL 0x0041eae0
CGameObject* CGenericGroupManager::GetFirstElement()
{
	CGenericGroup* group = GetFirstGroup();
	if (group != NULL) {
		return group->GetFirstElementInGroup();
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0041eb00
CGameObject* CGenericGroupManager::GetNextElement()
{
	CGenericGroup* group = GetCurrentGroup();
	if (group != NULL) {
		CGameObject* object = group->GetNextElementInGroup();
		if (object != NULL) {
			return object;
		}
		group = GetNextGroup();
		if (group != NULL) {
			return group->GetFirstElementInGroup();
		}
		return NULL;
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0041eb40
CGameObject* CGenericGroupManager::GetCurrentElement()
{
	CGameObject* object = NULL;
	CGenericGroup* group = GetCurrentGroup();
	if (group != NULL) {
		object = group->GetCurrentElementInGroup();
	}
	return object;
}

// FUNCTION: LEMBALL 0x0041eb60
CGameObject* CGenericGroupManager::GetNthElement(int p_index)
{
	int i = 0;
	CGameObject* object = GetFirstElement();
	while (object != NULL && i < p_index) {
		i++;
		object = GetNextElement();
	}
	return object;
}

// FUNCTION: LEMBALL 0x0041eb90
CGenericGroup* CGenericGroupManager::GetGroupElementIsMemberOf(CGameObject* p_object)
{
	CGenericGroup* group = GetFirstGroup();
	while (group != NULL) {
		if (group->ConfirmElementIsInGroup(p_object) == true) {
			return group;
		}
		group = GetNextGroup();
	}
	return NULL;
}

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

// FUNCTION: LEMBALL 0x0041ec80
void CGenericGroupManager::AddNewGroup(CGenericGroup* p_group)
{
	int count = m_groupCount;

	if (count < GENERIC_GROUP_CAPACITY) {
		m_groups[count] = p_group;
		m_groupCount = m_groupCount + 1;
	}
}

// FUNCTION: LEMBALL 0x0041eca0
void CGenericGroupManager::CreateNewGroup()
{
}

// FUNCTION: LEMBALL 0x0041ecb0
void CGenericGroupManager::DeleteGroup(CGenericGroup* p_group)
{
	int index = 0;
	for (; index < m_groupCount; index++) {
		if (m_groups[index] != p_group) {
			continue;
		}
		delete p_group;
		m_groupCount--;
		if (index < m_groupCount) {
			do {
				m_groups[index] = m_groups[index + 1];
				index++;
			} while (index < m_groupCount);
		}
		m_groups[index] = NULL;
		return;
	}
}

// FUNCTION: LEMBALL 0x0041ed20
void CGenericGroupManager::AddElementToGroup(CGameObject* p_object, CGenericGroup* p_group)
{
	FindElementInGroupAndRemoveIt(p_object);
	p_group->AddElementToGroup(p_object);
}

// FUNCTION: LEMBALL 0x0041ed40
bool CGenericGroupManager::RemoveElementFromGroup(CGameObject* p_object, CGenericGroup* p_group)
{
	bool groupExists = true;
	if (p_group != NULL) {
		p_group->RemoveElementFromGroup(p_object);
		if (m_deleteEmptyGroups && p_group->GetElementsInGroup() < 1) {
			delete p_group;
			m_groupCount--;
			groupExists = false;
			for (int index = 0; index < GENERIC_GROUP_CAPACITY; index++) {
				if (m_groups[index] == p_group) {
					int destination = index;
					if (index < GENERIC_GROUP_CAPACITY - 1) {
						int remaining = GENERIC_GROUP_CAPACITY - 1 - index;
						destination += remaining;
						CGenericGroup** group = m_groups + index;
						do {
							*group = group[1];
							group++;
							remaining--;
						} while (remaining != 0);
					}
					m_groups[destination] = NULL;
				}
			}
		}
	}
	return groupExists;
}

// FUNCTION: LEMBALL 0x0041ede0
void CGenericGroupManager::FindElementInGroupAndRemoveIt(CGameObject* p_object)
{
	RemoveElementFromGroup(p_object, GetGroupElementIsMemberOf(p_object));
}

// FUNCTION: LEMBALL 0x0041ee00
int CGenericGroupManager::GetAllBoundingBoxes(tRect* p_rects)
{
	tRect* output;
	int count = 0;
	CVSRect bounds;
	CGenericGroup* group = GetFirstGroup();
	if (group != NULL) {
		output = p_rects;
		do {
			group->GetBoundingBox(bounds);
			output->m_left = bounds.m_x;
			output->m_top = bounds.m_y;
			output->m_right = bounds.m_x + bounds.m_width;
			output->m_bottom = bounds.m_y + bounds.m_height;
			output++;
			count++;
			group = GetNextGroup();
		} while (group != NULL);
	}
	return count;
}

// FUNCTION: LEMBALL 0x0041ee90
int CGenericGroupManager::GetViewData(CViewData* p_viewData)
{
	int total = 0;
	CGenericGroup* group = GetFirstGroup();
	if (group != NULL) {
		do {
			total += group->GetViewData(p_viewData + total);
			group = GetNextGroup();
		} while (group != NULL);
	}
	return total;
}

// FUNCTION: LEMBALL 0x0041eed0
bool CGenericGroupManager::CheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	CGenericGroup* group = GetFirstGroup();
	while (group != NULL) {
		if (group->CheckGroupIntersection(p_rect, p_coordinate) == true) {
			return true;
		}
		group = GetNextGroup();
	}
	return false;
}
