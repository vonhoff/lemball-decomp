#include "CSheepGroup.h"

#include "Game/CGame.h"
#include "Gameplay/Geometry/Facing.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/VSTrig.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "CGenericGroup.h"
#include "Gameplay/Objects/CViewData.h"
#include "Game/SoundEffects.h"
#include "Visos/Math/CFixed.h"
#include "Visos/Math/CVector.h"
#include "Visos/Math/RandomConstants.h"

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
			VSTrig* trigTable = g_pVSTrig;
			int sineValue;
			int cosineValue;
			if (escapeAngle < 0) {
				sineValue = -trigTable->m_sine[(-escapeAngle) % TRIG_ANGLE_FULL_TURN].m_value;
			}
			else {
				sineValue = g_pVSTrig->m_sine[escapeAngle % TRIG_ANGLE_FULL_TURN].m_value;
			}
			CFixed sine(sineValue);
			int cosineAngle = escapeAngle + TRIG_ANGLE_QUARTER_TURN;
			if (cosineAngle < 0) {
				cosineValue =
					-g_pVSTrig->m_sine[(-TRIG_ANGLE_QUARTER_TURN - escapeAngle) % TRIG_ANGLE_FULL_TURN].m_value;
			}
			else {
				cosineValue = g_pVSTrig->m_sine[cosineAngle % TRIG_ANGLE_FULL_TURN].m_value;
			}
			CFixed cosine(cosineValue);
			CVector rotatedEscapeVector = trigTable->Rotate(escapeVector, sine, cosine);
			escapeDestination.m_xFixed += rotatedEscapeVector.m_xFixed;
			escapeDestination.m_yFixed += rotatedEscapeVector.m_yFixed;
			if ((escapeDestination.m_xFixed >> FIXED_POINT_FRACTION_BITS) < 0) {
				escapeDestination.m_xFixed = 0;
			}
			if ((escapeDestination.m_yFixed >> FIXED_POINT_FRACTION_BITS) < 0) {
				escapeDestination.m_yFixed = 0;
			}
			int formationRandomValue =
				(*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
			*g_pRandomSeed = formationRandomValue;
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
