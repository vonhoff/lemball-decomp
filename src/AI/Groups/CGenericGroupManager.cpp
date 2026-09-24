#include "CGenericGroupManager.h"

#include "../Base/Rect.h"
#include "../Managers/CObjectManager.h"
#include "../Navigation/CAI.h"
#include "../Objects/CViewData.h"
#include "AI/Base/CGameObject.h"
#include "CFormationManager.h"
#include "CGenericGroup.h"
#include "Visos/Foundation/CVsRect.h"

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
	m_state = 1;
	memset(m_groups, 0, sizeof(m_groups));
}

// FUNCTION: LEMBALL 0x0041e940
CGenericGroupManager::~CGenericGroupManager()
{
	for (int i = 0; i < 40; i++) {
		if (m_groups[i] != 0) {
			delete m_groups[i];
		}
	}
}

// FUNCTION: LEMBALL 0x0041e970
void CGenericGroupManager::Restart()
{
	CGenericGroup* group = GetFirstGroup();
	while (group != 0) {
		group->Restart();
		group = GetNextGroup();
	}
}

// FUNCTION: LEMBALL 0x0041e9f0
int CGenericGroupManager::Process()
{
	CGenericGroup* group = GetFirstGroup();
	while (group != 0) {
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
		return 0;
	}
	return m_groups[0];
}

// FUNCTION: LEMBALL 0x0041ea50
CGenericGroup* CGenericGroupManager::GetNextGroup()
{
	int index = m_currentGroup + 1;
	m_currentGroup = index;
	if (m_groupCount <= index) {
		return 0;
	}
	return m_groups[index];
}

// FUNCTION: LEMBALL 0x0041ea70
CGenericGroup* CGenericGroupManager::GetNthGroup(int p_index)
{
	m_currentGroup = p_index;
	if (m_groupCount <= p_index) {
		return 0;
	}
	return m_groups[p_index];
}

// FUNCTION: LEMBALL 0x0041ea90
CGenericGroup* CGenericGroupManager::GetCurrentGroup()
{
	if (m_groupCount <= m_currentGroup) {
		return 0;
	}
	return m_groups[m_currentGroup];
}

// FUNCTION: LEMBALL 0x0041eab0
int CGenericGroupManager::GetNumberOfElements()
{
	int total = 0;
	CGenericGroup* group = GetFirstGroup();
	if (group != 0) {
		do {
			total += group->GetElementsInGroup();
			group = GetNextGroup();
		} while (group != 0);
	}
	return total;
}

// FUNCTION: LEMBALL 0x0041eae0
CGameObject* CGenericGroupManager::GetFirstElement()
{
	CGenericGroup* group = GetFirstGroup();
	if (group != 0) {
		return group->GetFirstElementInGroup();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041eb00
CGameObject* CGenericGroupManager::GetNextElement()
{
	CGenericGroup* group = GetCurrentGroup();
	if (group != 0) {
		CGameObject* object = group->GetNextElementInGroup();
		if (object != 0) {
			return object;
		}
		group = GetNextGroup();
		if (group != 0) {
			return group->GetFirstElementInGroup();
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041eb40
CGameObject* CGenericGroupManager::GetCurrentElement()
{
	CGameObject* object = 0;
	CGenericGroup* group = GetCurrentGroup();
	if (group != 0) {
		object = group->GetCurrentElementInGroup();
	}
	return object;
}

// FUNCTION: LEMBALL 0x0041eb60
CGameObject* CGenericGroupManager::GetNthElement(int p_index)
{
	int i = 0;
	CGameObject* object = GetFirstElement();
	while (object != 0 && i < p_index) {
		i++;
		object = GetNextElement();
	}
	return object;
}

// FUNCTION: LEMBALL 0x0041eb90
CGenericGroup* CGenericGroupManager::GetGroupElementIsMemberOf(CGameObject* p_object)
{
	CGenericGroup* group = GetFirstGroup();
	while (group != 0) {
		if (group->ConfirmElementIsInGroup(p_object) == 1) {
			return group;
		}
		group = GetNextGroup();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ec80
void CGenericGroupManager::AddNewGroup(CGenericGroup* p_group)
{
	int count = m_groupCount;

	if (count < 0x28) {
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
		m_groups[index] = 0;
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
	bool groupExists = 1;
	if (p_group != 0) {
		p_group->RemoveElementFromGroup(p_object);
		if (m_state != 0 && p_group->GetElementsInGroup() < 1) {
			delete p_group;
			m_groupCount--;
			groupExists = 0;
			for (int index = 0; index < 40; index++) {
				if (m_groups[index] == p_group) {
					int destination = index;
					if (index < 39) {
						int remaining = 39 - index;
						destination += remaining;
						CGenericGroup** group = m_groups + index;
						do {
							*group = group[1];
							group++;
							remaining--;
						} while (remaining != 0);
					}
					m_groups[destination] = 0;
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
int CGenericGroupManager::GetAllBoundingBoxes(Rect* p_rects)
{
	int count = 0;
	CVsRect bounds;
	CGenericGroup* group = GetFirstGroup();
	if (group != 0) {
		do {
			group->GetBoundingBox(bounds);
			p_rects->m_left = bounds.m_x;
			p_rects->m_top = bounds.m_y;
			p_rects->m_right = bounds.m_x + bounds.m_width;
			p_rects->m_bottom = bounds.m_y + bounds.m_height;
			p_rects++;
			count++;
			group = GetNextGroup();
		} while (group != 0);
	}
	return count;
}

// FUNCTION: LEMBALL 0x0041ee90
int CGenericGroupManager::GetViewData(CViewData* p_viewData)
{
	int total = 0;
	CGenericGroup* group = GetFirstGroup();
	if (group != 0) {
		do {
			total += group->GetViewData(p_viewData + total);
			group = GetNextGroup();
		} while (group != 0);
	}
	return total;
}

// FUNCTION: LEMBALL 0x0041eed0
bool CGenericGroupManager::CheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	CGenericGroup* group = GetFirstGroup();
	while (group != 0) {
		if (group->CheckGroupIntersection(p_rect, p_coordinate) == 1) {
			return 1;
		}
		group = GetNextGroup();
	}
	return 0;
}
