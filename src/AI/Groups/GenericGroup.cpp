#include "GenericGroup.h"

#include "../Navigation/AiDestinationList.h"

#include <string.h>

#pragma intrinsic(memset)

GenericGroup::GenericGroup()
{
}

// 68K 0x10118774 GetGroupState__13CGenericGroupFv
// FUNCTION: LEMBALL 0x00414c60
eGroupState GenericGroup::GetGroupState()
{
	return m_groupState;
}

// 68K 0x101187a8 SetGroupState__13CGenericGroupF11eGroupState
// FUNCTION: LEMBALL 0x00414c70
void GenericGroup::SetGroupState(eGroupState p_state)
{
	m_groupState = p_state;
}

// 68K 0x1060c298 __ct__13CGenericGroupFP3CAIP14CObjectManagerP17CFormationManagerii
// FUNCTION: LEMBALL 0x0041dda0
GenericGroup::GenericGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager)
	: GameObject((eObjectType) 0x10, 0, 0x14)
{
	m_minY = 0;
	m_minX = 0;
	m_maxY = 0;
	m_maxX = 0;
	g_pUnknown0x4a7824 = p_ai;
	g_pUnknown0x4a7820 = p_formationManager;
	g_pUnknown0x4a781c = p_objectManager;
	m_currentElement = 0;
	m_elementCount = 0;
	m_groupState = 0;
	memset(m_elements, 0, sizeof(m_elements));
	m_minY = 9999;
	m_minX = 9999;
	m_maxY = 9999;
	m_maxX = 9999;
}

// 68K 0x1060c44c Restart__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041de80
void GenericGroup::Restart()
{
	GameObject::Restart();
	memset(m_elements, 0, sizeof(m_elements));
	m_elementCount = 0;
}

// 68K 0x1060c4a0 Process__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041deb0
bool GenericGroup::Process()
{
	CalculateBoundingBox(24);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	return 0;
}

// 68K 0x1060c50a GetElementsInGroup__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041def0
int GenericGroup::GetElementsInGroup()
{
	return m_elementCount;
}

// 68K 0x1060c544 GetFirstElementInGroup__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041df00
GameObject* GenericGroup::GetFirstElementInGroup()
{
	m_currentElement = 0;
	return m_elements[0];
}

// 68K 0x1060c588 GetNextElementInGroup__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041df20
GameObject* GenericGroup::GetNextElementInGroup()
{
	int index = m_currentElement + 1;
	m_currentElement = index;
	if (m_elementCount <= index) {
		return 0;
	}
	return m_elements[index];
}

// 68K 0x1060c5de GetCurrentElementInGroup__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041df40
GameObject* GenericGroup::GetCurrentElementInGroup()
{
	if (m_elementCount <= m_currentElement) {
		return 0;
	}
	return m_elements[m_currentElement];
}

// 68K 0x1060c634 GetNthElementInGroup__13CGenericGroupFi
// FUNCTION: LEMBALL 0x0041df60
GameObject* GenericGroup::GetNthElementInGroup(int p_index)
{
	m_currentElement = p_index;
	if (m_elementCount <= p_index) {
		return 0;
	}
	return m_elements[p_index];
}

// 68K 0x1060c68c SwapElements__13CGenericGroupFP11CGameObjectP11CGameObject
// FUNCTION: LEMBALL 0x0041dfc0
void GenericGroup::SwapElements(GameObject* p_first, GameObject* p_second)
{
	int firstIndex;
	int secondIndex = -1;
	int index = 0;

	if (m_elementCount > index) {
		do {
			GameObject* element = m_elements[index];
			if (p_first == element) {
				firstIndex = index;
			}
			if (element == p_second) {
				secondIndex = index;
			}
			index++;
		} while (index < m_elementCount);
	}
	m_elements[firstIndex] = p_second;
	m_elements[secondIndex] = p_first;
}

// 68K 0x1060c71c AddElementToGroup__13CGenericGroupFP11CGameObject
// FUNCTION: LEMBALL 0x0041e020
void GenericGroup::AddElementToGroup(GameObject* p_object)
{
	m_elements[m_elementCount] = p_object;
	m_altered = 1;
	m_elementCount = m_elementCount + 1;
}

// 68K 0x1060c776 RemoveElementFromGroup__13CGenericGroupFP11CGameObject
// FUNCTION: LEMBALL 0x0041e050
void GenericGroup::RemoveElementFromGroup(GameObject* p_object)
{
	int index = 0;
	GameObject** element = m_elements;

	do {
		if (*element == p_object) {
			if (index < 9) {
				element = &m_elements[index];
				int remaining = 9 - index;
				index += remaining;
				do {
					GameObject* copy = element[1];
					element++;
					remaining--;
					element[-1] = copy;
				} while (remaining != 0);
			}
			m_elements[index] = 0;
			m_elementCount--;
			m_altered = 1;
			return;
		}
		element++;
		index++;
	} while (index < 10);
}

// 68K 0x1060c814 ConfirmElementIsInGroup__13CGenericGroupFP11CGameObject
// FUNCTION: LEMBALL 0x0041e0c0
bool GenericGroup::ConfirmElementIsInGroup(GameObject* p_object)
{
	for (int i = 0; i < m_elementCount; i++) {
		if (m_elements[i] == p_object) {
			return 1;
		}
	}
	return 0;
}

// 68K 0x1060c880 ConfirmElementIsInGroup__13CGenericGroupFUs
// FUNCTION: LEMBALL 0x0041e100
bool GenericGroup::ConfirmElementIsInGroup(unsigned short p_objectId)
{
	for (int i = 0; i < m_elementCount; i++) {
		GameObject* object = m_elements[i];
		if (object != 0 && object->m_objectId == p_objectId) {
			return 1;
		}
	}
	return 0;
}

// 68K 0x1060c8f0 GetBoundingBox__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041e140
VsRect GenericGroup::GetBoundingBox()
{
	VsRect result;
	result.m_width = m_minX;
	result.m_height = m_minY;
	result.m_x = m_maxX;
	result.m_y = m_maxY;
	return result;
}

// 68K 0x1060c974 GetBoundingBox__13CGenericGroupFR7CVSRect
// FUNCTION: LEMBALL 0x0041e180
void GenericGroup::GetBoundingBox(VsRect& p_rect)
{
	const short* coords;

	p_rect.m_width = m_minX;
	p_rect.m_height = m_minY;
	if (&m_minX != 0) {
		coords = &m_maxX;
	}
	else {
		coords = 0;
	}
	p_rect.m_x = *coords;
	p_rect.m_y = coords[1];
}

// 68K 0x1060c9dc CalculateBoundingBox__13CGenericGroupFi
// STUB: LEMBALL 0x0041e1c0
void GenericGroup::CalculateBoundingBox(int p_radius)
{
}

#include "../Navigation/AiDestinationEntry.h"

// 68K 0x1060cb04 AddNewWaypoint__13CGenericGroupF7AICOORDP17CFormationManager
// FUNCTION: LEMBALL 0x0041e290
void GenericGroup::AddNewWaypoint(AiCoord p_coordinate, FormationManager* p_formationManager)
{
	g_pUnknown0x4a7820 = p_formationManager;
	unsigned short count;
	AiDestinationList* list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		AiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = (eDestinationType) 1;
		entry->m_coordinate.m_xFixed = p_coordinate.m_xFixed;
		entry->m_coordinate.m_yFixed = p_coordinate.m_yFixed;
		entry->m_coordinate.m_zFixed = p_coordinate.m_zFixed;
	}
}

// 68K 0x1060cb96 SendNewWaypoint__13CGenericGroupF7AICOORD
// STUB: LEMBALL 0x0041e2e0
void GenericGroup::SendNewWaypoint(AiCoord p_coordinate)
{
}

// 68K 0x1060cd04 OverideExistingWaypoints__13CGenericGroupF7AICOORD
// FUNCTION: LEMBALL 0x0041e3c0
void GenericGroup::OverideExistingWaypoints(AiCoord p_coordinate)
{
}

// 68K 0x1060cd42 ClearExistingWaypoints__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041e3d0
void GenericGroup::ClearExistingWaypoints()
{
	GameObject* object;

	m_destinationList->m_count = 0;
	object = GetFirstElementInGroup();
	if (object != 0) {
		do {
			object->ResetInstructions();
			object = GetNextElementInGroup();
		} while (object != 0);
	}
}

// 68K 0x1060cdbc SetFormationIndex__13CGenericGroupFi
// FUNCTION: LEMBALL 0x0041e400
void GenericGroup::SetFormationIndex(int p_formationIndex)
{
	m_formationIndex = p_formationIndex;
}

// 68K 0x1060cdf6 GetFormationIndex__13CGenericGroupFv
// FUNCTION: LEMBALL 0x0041e410
int GenericGroup::GetFormationIndex()
{
	return m_formationIndex;
}

// 68K 0x1060ce2e ReformAlteredGroup__13CGenericGroupFP17CFormationManager
// STUB: LEMBALL 0x0041e420
void GenericGroup::ReformAlteredGroup(FormationManager* p_formationManager)
{
}

// 68K 0x1060cff2 CheckGroupIntersection__13CGenericGroupFP7CVSRectP7AICOORD
// STUB: LEMBALL 0x0041e530
bool GenericGroup::CheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return 0;
}

// 68K 0x1060d14a GetViewData__13CGenericGroupFP9CViewData
// STUB: LEMBALL 0x0041e640
void GenericGroup::GetViewData(ViewData* p_viewData)
{
}

// 68K 0x1060c3aa __dt__13CGenericGroupFv
GenericGroup::~GenericGroup()
{
}

// GLOBAL: LEMBALL 0x004a781c
ObjectManager* g_pUnknown0x4a781c;

// GLOBAL: LEMBALL 0x004a7820
FormationManager* g_pUnknown0x4a7820;

// GLOBAL: LEMBALL 0x004a7824
Ai* g_pUnknown0x4a7824;
