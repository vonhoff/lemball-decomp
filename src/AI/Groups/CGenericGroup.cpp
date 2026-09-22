#include "CGenericGroup.h"

#include "../../Visos/Foundation/CVSMath.h"
#include "../Navigation/CAiDestinationList.h"
#include "CFormationManager.h"

#include <string.h>

#pragma intrinsic(memset)

// FUNCTION: LEMBALL 0x00414c60
eGroupState CGenericGroup::GetGroupState()
{
	return m_groupState;
}

// FUNCTION: LEMBALL 0x00414c70
void CGenericGroup::SetGroupState(eGroupState p_state)
{
	m_groupState = p_state;
}

// FUNCTION: LEMBALL 0x0041dda0
CGenericGroup::CGenericGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager)
	: CGameObject(OBJECT_GROUP, 0, 0x14)
{
	g_pGroupAI = p_ai;
	g_pGroupFormationManager = p_formationManager;
	g_pGroupObjectManager = p_objectManager;
	m_currentElement = 0;
	m_elementCount = 0;
	m_groupState = GROUP_STATE_IDLE;
	memset(m_elements, 0, sizeof(m_elements));
	m_bounds.m_height = 9999;
	m_bounds.m_width = 9999;
	m_bounds.m_y = 9999;
	m_bounds.m_x = 9999;
}

// FUNCTION: LEMBALL 0x0041de40
CGenericGroup::~CGenericGroup()
{
	for (int i = 0; i < 10; i++) {
		delete m_elements[i];
	}
}

// FUNCTION: LEMBALL 0x0041de80
void CGenericGroup::Restart()
{
	CGameObject::Restart();
	memset(m_elements, 0, sizeof(m_elements));
	m_elementCount = 0;
}

// FUNCTION: LEMBALL 0x0041deb0
bool CGenericGroup::Process()
{
	CalculateBoundingBox(24);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041def0
int CGenericGroup::GetElementsInGroup()
{
	return m_elementCount;
}

// FUNCTION: LEMBALL 0x0041df00
CGameObject* CGenericGroup::GetFirstElementInGroup()
{
	m_currentElement = 0;
	return m_elements[0];
}

// FUNCTION: LEMBALL 0x0041df20
CGameObject* CGenericGroup::GetNextElementInGroup()
{
	int index = m_currentElement + 1;
	m_currentElement = index;
	if (m_elementCount <= index) {
		return 0;
	}
	return m_elements[index];
}

// FUNCTION: LEMBALL 0x0041df40
CGameObject* CGenericGroup::GetCurrentElementInGroup()
{
	if (m_elementCount <= m_currentElement) {
		return 0;
	}
	return m_elements[m_currentElement];
}

// FUNCTION: LEMBALL 0x0041df60
CGameObject* CGenericGroup::GetNthElementInGroup(int p_index)
{
	m_currentElement = p_index;
	if (m_elementCount <= p_index) {
		return 0;
	}
	return m_elements[p_index];
}

// FUNCTION: LEMBALL 0x0041dfc0
void CGenericGroup::SwapElements(CGameObject* p_first, CGameObject* p_second)
{
	int firstIndex;
	int secondIndex = -1;
	int index = 0;

	if (m_elementCount > index) {
		do {
			CGameObject* element = m_elements[index];
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

// FUNCTION: LEMBALL 0x0041e020
void CGenericGroup::AddElementToGroup(CGameObject* p_object)
{
	m_elements[m_elementCount] = p_object;
	m_altered = 1;
	m_elementCount = m_elementCount + 1;
}

// FUNCTION: LEMBALL 0x0041e050
void CGenericGroup::RemoveElementFromGroup(CGameObject* p_object)
{
	int index = 0;
	CGameObject** element = m_elements;

	do {
		if (*element == p_object) {
			if (index < 9) {
				element = &m_elements[index];
				int remaining = 9 - index;
				index += remaining;
				do {
					CGameObject* copy = element[1];
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

// FUNCTION: LEMBALL 0x0041e0c0
bool CGenericGroup::ConfirmElementIsInGroup(CGameObject* p_object)
{
	for (int i = 0; i < m_elementCount; i++) {
		if (m_elements[i] == p_object) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e100
bool CGenericGroup::ConfirmElementIsInGroup(unsigned short p_objectId)
{
	for (int i = 0; i < m_elementCount; i++) {
		CGameObject* object = m_elements[i];
		if (object != 0 && object->m_objectId == p_objectId) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e140
CVsRect CGenericGroup::GetBoundingBox()
{
	return m_bounds;
}

// FUNCTION: LEMBALL 0x0041e180
void CGenericGroup::GetBoundingBox(CVsRect& p_rect)
{
	p_rect.m_width = m_bounds.m_width;
	p_rect.m_height = m_bounds.m_height;
	const CVsPoint* position = &m_bounds;
	p_rect.m_x = position->m_x;
	p_rect.m_y = position->m_y;
}

// FUNCTION: LEMBALL 0x0041e1c0
void CGenericGroup::CalculateBoundingBox(int p_radius)
{
	int minY = 99999;
	int minX = 99999;
	int maxY = -1;
	int maxX = -1;
	if (m_elementCount > 0) {
		int radius = p_radius;
		CGameObject** element = m_elements;
		int count = m_elementCount;
		do {
			CGameObject* object = *element;
			if (object != 0) {
				int x = object->m_position.m_xFixed >> 12;
				int y = object->m_position.m_yFixed >> 12;
				int right = radius + x;
				x -= radius;
				int bottom = y + radius;
				y -= radius;
				if (x < minX) {
					minX = x;
				}
				if (y < minY) {
					minY = y;
				}
				if (maxX < right) {
					maxX = right;
				}
				if (bottom > maxY) {
					maxY = bottom;
				}
			}
			element++;
			count--;
		} while (count != 0);
	}
	maxX -= minX;
	maxY -= minY;
	m_bounds.m_x = (short) minX;
	m_bounds.m_y = (short) minY;
	m_bounds.m_width = (short) maxX;
	m_bounds.m_height = (short) maxY;
}

#include "../Navigation/CAiDestinationEntry.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectTypes.h"
#include "Visos/Foundation/CVector.h"
#include "Visos/Foundation/CVsRect.h"

// FUNCTION: LEMBALL 0x0041e290
void CGenericGroup::AddNewWaypoint(AiCoord p_coordinate, CFormationManager* p_formationManager)
{
	g_pGroupFormationManager = p_formationManager;
	unsigned short count;
	CAiDestinationList* list = m_destinationList;
	count = list->m_count;
	if (count < list->m_capacity) {
		list->m_count = count + 1;
		CAiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = DESTINATION_COORD;
		entry->m_coordinate.m_xFixed = p_coordinate.m_xFixed;
		entry->m_coordinate.m_yFixed = p_coordinate.m_yFixed;
		entry->m_coordinate.m_zFixed = p_coordinate.m_zFixed;
	}
}

// FUNCTION: LEMBALL 0x0041e2e0
void CGenericGroup::SendNewWaypoint(AiCoord p_coordinate)
{
	AiCoord destination;
	CGameObject* object = GetFirstElementInGroup();
	if (object != 0) {
		unsigned int direction = ReturnFacingDirection(object->m_position.m_xFixed >> 12,
													   object->m_position.m_yFixed >> 12,
													   p_coordinate.m_xFixed >> 12,
													   p_coordinate.m_yFixed >> 12);
		int index = 0;
		g_pGroupFormationManager->TransformFormation(m_formationIndex, (direction - 2) * 0x40);
		int count = GetElementsInGroup();
		int height = p_coordinate.m_zFixed;
		while (index < count) {
			CVector* vector = g_pGroupFormationManager->GetAVector(index);
			destination.m_xFixed = vector->m_xFixed + p_coordinate.m_xFixed;
			destination.m_yFixed = vector->m_yFixed + p_coordinate.m_yFixed;
			destination.m_zFixed = height;
			object->AddDestination(destination);
			object = GetNextElementInGroup();
			index++;
		}
	}
}

// FUNCTION: LEMBALL 0x0041e3c0
void CGenericGroup::OverideExistingWaypoints(AiCoord p_coordinate)
{
}

// FUNCTION: LEMBALL 0x0041e3d0
void CGenericGroup::ClearExistingWaypoints()
{
	CGameObject* object;

	m_destinationList->m_count = 0;
	object = GetFirstElementInGroup();
	if (object != 0) {
		do {
			object->ResetInstructions();
			object = GetNextElementInGroup();
		} while (object != 0);
	}
}

// FUNCTION: LEMBALL 0x0041e400
void CGenericGroup::SetFormationIndex(int p_formationIndex)
{
	m_formationIndex = p_formationIndex;
}

// FUNCTION: LEMBALL 0x0041e410
int CGenericGroup::GetFormationIndex()
{
	return m_formationIndex;
}

// FUNCTION: LEMBALL 0x0041e420
void CGenericGroup::ReformAlteredGroup(CFormationManager* p_formationManager)
{
	AiCoord coordinate;

	if (m_altered != 0) {
		CGameObject* object = GetFirstElementInGroup();
		if (object != 0) {
			AiCoord destination;
			{
				const AiCoord& returnedDestination = object->GetDestination();
				destination.m_xFixed = returnedDestination.m_xFixed;
				destination.m_yFixed = returnedDestination.m_yFixed;
				destination.m_zFixed = returnedDestination.m_zFixed;
			}
			unsigned int direction = ReturnFacingDirection(object->m_position.m_xFixed >> 12,
														   object->m_position.m_yFixed >> 12,
														   destination.m_xFixed >> 12,
														   destination.m_yFixed >> 12);
			p_formationManager->TransformFormation(m_formationIndex, (direction - 2) << 6);

			int count = GetElementsInGroup();
			for (int i = 0; i < count; i++) {
				CVector* vector = p_formationManager->GetAVector(i);
				coordinate.m_xFixed = vector->m_xFixed + destination.m_xFixed;
				coordinate.m_yFixed = vector->m_yFixed + destination.m_yFixed;
				coordinate.m_zFixed = destination.m_zFixed;
				object->AlterDestination(coordinate);
				object = GetNextElementInGroup();
			}
		}
		m_altered = 0;
	}
}

// FUNCTION: LEMBALL 0x0041e530
bool CGenericGroup::CheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	int groupRight = m_bounds.m_width + m_bounds.m_x;
	int groupBottom = m_bounds.m_height + m_bounds.m_y;
	int rectX = p_rect->m_x;
	int rectRight = p_rect->m_width + rectX;
	int rectY = p_rect->m_y;
	int rectBottom = p_rect->m_height + rectY;

	if (m_bounds.m_x < rectRight && rectX < groupRight && m_bounds.m_y < rectBottom && rectY < groupBottom) {
		CGameObject* object = GetFirstElementInGroup();
		while (object != 0) {
			int x = object->m_position.m_xFixed >> 12;
			int y = object->m_position.m_yFixed >> 12;
			if (x - 24 < rectRight && rectX < x + 24 && y - 24 < rectBottom && rectY < y + 24) {
				p_coordinate->m_xFixed = object->m_position.m_xFixed;
				p_coordinate->m_yFixed = object->m_position.m_yFixed;
				p_coordinate->m_zFixed = object->m_position.m_zFixed;
				return 1;
			}
			object = GetNextElementInGroup();
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e640
int CGenericGroup::GetViewData(CViewData* p_viewData)
{
	int count;
	CGenericGroup* self = this;
	CGameObject* object = self->GetFirstElementInGroup();
	count = 0;
	if (object != 0) {
		do {
			object->GetViewData(*p_viewData);
			p_viewData++;
			count++;
			object = self->GetNextElementInGroup();
		} while (object != 0);
	}
	return count;
}

// GLOBAL: LEMBALL 0x004a781c
CObjectManager* g_pGroupObjectManager;

// GLOBAL: LEMBALL 0x004a7820
CFormationManager* g_pGroupFormationManager;

// GLOBAL: LEMBALL 0x004a7824
CAI* g_pGroupAI;
