#include "CSheepGroup.h"

#include "Application/GameMain.h"
#include "Application/SoundEffects.h"
#include "CGenericGroup.h"
#include "Engine/Math/CVSMath.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVector.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/RandomConstants.h"
#include "Engine/Math/VSTrig.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Simulation/CAI.h"

#include <stddef.h>

enum {
	SHEEP_ESCAPE_VECTOR_DISTANCE_FIXED = 50 * FIXED_POINT_ONE
};
// FUNCTION: LEMBALL 0x0041f500
CSheepGroup::CSheepGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager)
	: CGenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// FUNCTION: LEMBALL 0x0041f530
void CSheepGroup::RunAway(AICOORD p_threatPosition)
{
	int membersWithDestination;
	CGameObject* groupMember;
	AICOORD escapeDestination;
	CVector escapeVector(SHEEP_ESCAPE_VECTOR_DISTANCE_FIXED, 0);
	membersWithDestination = 0;
	groupMember = GetFirstElementInGroup();
	while (groupMember != NULL) {
		if (groupMember->DestinationExists() == true) {
			membersWithDestination++;
		}
		groupMember = GetNextElementInGroup();
	}
	if (membersWithDestination == 0) {
		CGameObject* firstMember = GetFirstElementInGroup();
		if (firstMember != NULL) {
			firstMember->SetSndEffect(SFX_SHEEP);
			int positionY = firstMember->m_position.m_yFixed;
			int positionZ = firstMember->m_position.m_zFixed;
			escapeDestination.m_xFixed = firstMember->m_position.m_xFixed;
			escapeDestination.m_yFixed = positionY;
			escapeDestination.m_zFixed = positionZ;
			int escapeAngle = ((ReturnFacingDirection(escapeDestination.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													  escapeDestination.m_yFixed >> FIXED_POINT_FRACTION_BITS,
													  p_threatPosition.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													  p_threatPosition.m_yFixed >> FIXED_POINT_FRACTION_BITS) +
								1) &
							   FACING_DIRECTION_MASK) *
							  TRIG_ANGLE_EIGHTH_TURN;
			CVector rotatedEscapeVector = g_pVSTrig->Rotate(escapeVector, escapeAngle);
			escapeDestination.m_xFixed = (unsigned int) escapeDestination.m_xFixed + rotatedEscapeVector.m_xFixed;
			escapeDestination.m_yFixed = (unsigned int) escapeDestination.m_yFixed + rotatedEscapeVector.m_yFixed;
			if ((escapeDestination.m_xFixed >> FIXED_POINT_FRACTION_BITS) < 0) {
				escapeDestination.m_xFixed = 0;
			}
			if ((escapeDestination.m_yFixed >> FIXED_POINT_FRACTION_BITS) < 0) {
				escapeDestination.m_yFixed = 0;
			}
			int formationRandomValue =
				((unsigned int) g_pRandomSeed->m_randomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) &
				RANDOM_SEED_MASK;
			g_pRandomSeed->m_randomSeed = formationRandomValue;
			SetFormationIndex(formationRandomValue % 3);
			SendNewWaypoint(escapeDestination);
			m_runAwayActive = 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0041f730
void CSheepGroup::CheckAgainstLemmings()
{
	AICOORD coordinate;
	CVSRect bounds;
	GetBoundingBox(bounds);
	if (g_pGroupAI->PlayerCheckGroupIntersection(&bounds, &coordinate) == true) {
		RunAway(coordinate);
		return;
	}
	if (g_pGroupAI->EnemyCheckGroupIntersection(&bounds, &coordinate) == true) {
		RunAway(coordinate);
		return;
	}
	bool result = g_pGroupAI->BulletCheckGroupIntersection(&bounds, &coordinate);
	if (result == true) {
		RunAway(coordinate);
		return;
	}
}

// FUNCTION: LEMBALL 0x0041f820
bool CSheepGroup::Process()
{
	CalculateBoundingBox(GROUP_BOUNDING_BOX_RADIUS_PIXELS);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	CheckAgainstLemmings();
	CheckAgainstCatapults();
	return false;
}

// FUNCTION: LEMBALL 0x0041f870
bool CSheepGroup::CheckAgainstCatapults()
{
	return false;
}
