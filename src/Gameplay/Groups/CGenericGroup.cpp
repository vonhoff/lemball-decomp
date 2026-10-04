#include "CGenericGroup.h"

#include "Gameplay/Geometry/Facing.h"
#include "Engine/Math/VSTrig.h"
#include "Gameplay/Navigation/CAiDestinationList.h"
#include "CFormationManager.h"

#include <string.h>
#define GENERIC_GROUP_ELEMENT_CAPACITY 10
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

enum {
	GROUP_DESTINATION_CAPACITY = 20,
	GROUP_INITIAL_BOUNDS_VALUE = 9999,
	GROUP_MIN_BOUND_INITIAL_VALUE = 99999,
	GROUP_MAX_BOUND_INITIAL_VALUE = -1,
	GROUP_ELEMENT_INDEX_NOT_FOUND = -1
};

// FUNCTION: LEMBALL 0x0041dda0
CGenericGroup::CGenericGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager)
	: CGameObject(OBJECT_GROUP, 0, GROUP_DESTINATION_CAPACITY)
{
	g_pGroupAI = p_ai;
	g_pGroupFormationManager = p_formationManager;
	g_pGroupObjectManager = p_objectManager;
	m_currentElement = 0;
	m_elementCount = 0;
	m_groupState = GROUP_STATE_IDLE;
	memset(m_elements, 0, sizeof(m_elements));
	m_bounds.m_height = GROUP_INITIAL_BOUNDS_VALUE;
	m_bounds.m_width = GROUP_INITIAL_BOUNDS_VALUE;
	m_bounds.m_y = GROUP_INITIAL_BOUNDS_VALUE;
	m_bounds.m_x = GROUP_INITIAL_BOUNDS_VALUE;
}

// FUNCTION: LEMBALL 0x0041de40
CGenericGroup::~CGenericGroup()
{
	int remaining = GENERIC_GROUP_ELEMENT_CAPACITY;
	CGameObject** element = m_elements;
	do {
		delete *element;
		element++;
	} while (--remaining != 0);
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
	CalculateBoundingBox(GROUP_BOUNDING_BOX_RADIUS_PIXELS);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	return false;
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
		return NULL;
	}
	return m_elements[index];
}

// FUNCTION: LEMBALL 0x0041df40
CGameObject* CGenericGroup::GetCurrentElementInGroup()
{
	if (m_elementCount <= m_currentElement) {
		return NULL;
	}
	return m_elements[m_currentElement];
}

// FUNCTION: LEMBALL 0x0041df60
CGameObject* CGenericGroup::GetNthElementInGroup(int p_index)
{
	m_currentElement = p_index;
	if (m_elementCount <= p_index) {
		return NULL;
	}
	return m_elements[p_index];
}

// FUNCTION: LEMBALL 0x0041df90
void CGenericGroup::SwapElementIndices(int p_firstIndex, int p_secondIndex)
{
	CGameObject* element = m_elements[p_secondIndex];
	m_elements[p_secondIndex] = m_elements[p_firstIndex];
	m_elements[p_firstIndex] = element;
}

// FUNCTION: LEMBALL 0x0041dfc0
void CGenericGroup::SwapElements(CGameObject* p_first, CGameObject* p_second)
{
	int firstIndex;
	int secondIndex = GROUP_ELEMENT_INDEX_NOT_FOUND;
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
			if (index < GENERIC_GROUP_ELEMENT_CAPACITY - 1) {
				element = &m_elements[index];
				int remaining = GENERIC_GROUP_ELEMENT_CAPACITY - 1 - index;
				index += remaining;
				do {
					CGameObject* copy = element[1];
					element++;
					remaining--;
					element[-1] = copy;
				} while (remaining != 0);
			}
			m_elements[index] = NULL;
			m_elementCount--;
			m_altered = 1;
			return;
		}
		element++;
		index++;
	} while (index < GENERIC_GROUP_ELEMENT_CAPACITY);
}

// FUNCTION: LEMBALL 0x0041e0c0
bool CGenericGroup::ConfirmElementIsInGroup(CGameObject* p_object)
{
	for (int i = 0; i < m_elementCount; i++) {
		if (m_elements[i] == p_object) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041e100
bool CGenericGroup::ConfirmElementIsInGroup(unsigned short p_objectId)
{
	for (int i = 0; i < m_elementCount; i++) {
		CGameObject* object = m_elements[i];
		if (object != NULL && object->m_objectId == p_objectId) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041e140
CVSRect CGenericGroup::GetBoundingBox()
{
	return m_bounds;
}

// FUNCTION: LEMBALL 0x0041e180
void CGenericGroup::GetBoundingBox(CVSRect& p_rect)
{
	p_rect.m_width = m_bounds.m_width;
	p_rect.m_height = m_bounds.m_height;
	const CVSPoint* position = &m_bounds;
	p_rect.m_x = position->m_x;
	p_rect.m_y = position->m_y;
}

// FUNCTION: LEMBALL 0x0041e1c0
void CGenericGroup::CalculateBoundingBox(int p_radius)
{
	int minY = GROUP_MIN_BOUND_INITIAL_VALUE;
	int minX = GROUP_MIN_BOUND_INITIAL_VALUE;
	int maxY = GROUP_MAX_BOUND_INITIAL_VALUE;
	int maxX = GROUP_MAX_BOUND_INITIAL_VALUE;
	if (m_elementCount > 0) {
		int radius = p_radius;
		CGameObject** element = m_elements;
		int count = m_elementCount;
		do {
			CGameObject* object = *element;
			if (object != NULL) {
				int x = object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
				int y = object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
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

#include "Gameplay/Navigation/CAiDestinationEntry.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVector.h"

// FUNCTION: LEMBALL 0x0041e290
void CGenericGroup::AddNewWaypoint(AICOORD p_coordinate, CFormationManager* p_formationManager)
{
	g_pGroupFormationManager = p_formationManager;
	CAiDestinationList* list = m_destinationList;
	unsigned short count = list->m_count;
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
void CGenericGroup::SendNewWaypoint(AICOORD p_coordinate)
{
	AICOORD destination;
	CGameObject* object = GetFirstElementInGroup();
	if (object != NULL) {
		unsigned int direction = ReturnFacingDirection(object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													   object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
													   p_coordinate.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													   p_coordinate.m_yFixed >> FIXED_POINT_FRACTION_BITS);
		g_pGroupFormationManager->TransformFormation(m_formationIndex, (direction - 2) * TRIG_ANGLE_EIGHTH_TURN);
		int count = GetElementsInGroup();
		int height = p_coordinate.m_zFixed;
		int index = 0;
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
void CGenericGroup::OverideExistingWaypoints(AICOORD p_coordinate)
{
}

// FUNCTION: LEMBALL 0x0041e3d0
void CGenericGroup::ClearExistingWaypoints()
{
	CGameObject* object;

	m_destinationList->m_count = 0;
	object = GetFirstElementInGroup();
	if (object != NULL) {
		do {
			object->ResetInstructions();
			object = GetNextElementInGroup();
		} while (object != NULL);
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
	AICOORD coordinate;

	if (m_altered != 0) {
		CGameObject* object = GetFirstElementInGroup();
		if (object != NULL) {
			AICOORD destination;
			{
				const AICOORD& returnedDestination = object->GetDestination();
				destination.m_xFixed = returnedDestination.m_xFixed;
				destination.m_yFixed = returnedDestination.m_yFixed;
				destination.m_zFixed = returnedDestination.m_zFixed;
			}
			unsigned int direction = ReturnFacingDirection(object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
														   object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
														   destination.m_xFixed >> FIXED_POINT_FRACTION_BITS,
														   destination.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			p_formationManager->TransformFormation(m_formationIndex,
												   (direction - 2) * (TRIG_ANGLE_FULL_TURN / FACING_DIRECTION_COUNT));

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
bool CGenericGroup::CheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	int groupRight = m_bounds.m_width + m_bounds.m_x;
	int groupBottom = m_bounds.m_height + m_bounds.m_y;
	int rectX = p_rect->m_x;
	int rectRight = p_rect->m_width + rectX;
	int rectY = p_rect->m_y;
	int rectBottom = p_rect->m_height + rectY;

	if (m_bounds.m_x < rectRight && rectX < groupRight && m_bounds.m_y < rectBottom && rectY < groupBottom) {
		CGameObject* object = GetFirstElementInGroup();
		while (object != NULL) {
			int x = object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			int y = object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			if (x - 24 < rectRight && rectX < x + 24 && y - 24 < rectBottom && rectY < y + 24) {
				p_coordinate->m_xFixed = object->m_position.m_xFixed;
				p_coordinate->m_yFixed = object->m_position.m_yFixed;
				p_coordinate->m_zFixed = object->m_position.m_zFixed;
				return true;
			}
			object = GetNextElementInGroup();
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041e640
int CGenericGroup::GetViewData(CViewData* p_viewData)
{
	int count;
	CGenericGroup* self = this;
	CGameObject* object = self->GetFirstElementInGroup();
	count = 0;
	if (object != NULL) {
		do {
			object->GetViewData(*p_viewData);
			p_viewData++;
			count++;
			object = self->GetNextElementInGroup();
		} while (object != NULL);
	}
	return count;
}

// GLOBAL: LEMBALL 0x004a781c
CObjectManager* g_pGroupObjectManager;

// GLOBAL: LEMBALL 0x004a7820
CFormationManager* g_pGroupFormationManager;

// GLOBAL: LEMBALL 0x004a7824
CAI* g_pGroupAI;
