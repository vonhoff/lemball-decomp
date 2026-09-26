#include "CSheepGroup.h"

#include "../../Control/Game/CGame.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/VSTrig.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Groups/CGenericGroup.h"
#include "AI/Objects/CViewData.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CFixed.h"
#include "Visos/Foundation/CVector.h"

// FUNCTION: LEMBALL 0x0041f500
CSheepGroup::CSheepGroup(CAI* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager)
	: CGenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// FUNCTION: LEMBALL 0x0041f530
void CSheepGroup::RunAway(AiCoord p_threatPosition)
{
	CGameObject* groupMember;
	AiCoord escapeDestination;
	CVector escapeVector(0x32000, 0);
	int membersWithDestination = 0;
	groupMember = GetFirstElementInGroup();
	while (groupMember != 0) {
		if (groupMember->DestinationExists() == 1) {
			membersWithDestination++;
		}
		groupMember = GetNextElementInGroup();
	}
	if (membersWithDestination == 0) {
		CGameObject* firstMember = GetFirstElementInGroup();
		if (firstMember != 0) {
			firstMember->SetSndEffect(SFX_SHEEP);
			int positionY = firstMember->m_position.m_yFixed;
			int positionZ = firstMember->m_position.m_zFixed;
			escapeDestination.m_xFixed = firstMember->m_position.m_xFixed;
			escapeDestination.m_yFixed = positionY;
			escapeDestination.m_zFixed = positionZ;
			int escapeAngle = ((ReturnFacingDirection(escapeDestination.m_xFixed >> 12,
													  escapeDestination.m_yFixed >> 12,
													  p_threatPosition.m_xFixed >> 12,
													  p_threatPosition.m_yFixed >> 12) +
								1) &
							   7) *
							  64;
			VSTrig* trigTable = g_pVSTrig;
			int sineValue;
			int cosineValue;
			if (escapeAngle < 0) {
				sineValue = -trigTable->m_sine[(-escapeAngle) % 512].m_value;
			}
			else {
				sineValue = g_pVSTrig->m_sine[escapeAngle % 512].m_value;
			}
			CFixed sine(sineValue);
			int cosineAngle = escapeAngle + 128;
			if (cosineAngle < 0) {
				cosineValue = -g_pVSTrig->m_sine[(-128 - escapeAngle) % 512].m_value;
			}
			else {
				cosineValue = g_pVSTrig->m_sine[cosineAngle % 512].m_value;
			}
			CFixed cosine(cosineValue);
			CVector rotatedEscapeVector = trigTable->Rotate(escapeVector, sine, cosine);
			escapeDestination.m_xFixed += rotatedEscapeVector.m_xFixed;
			escapeDestination.m_yFixed += rotatedEscapeVector.m_yFixed;
			if ((escapeDestination.m_xFixed >> 12) < 0) {
				escapeDestination.m_xFixed = 0;
			}
			if ((escapeDestination.m_yFixed >> 12) < 0) {
				escapeDestination.m_yFixed = 0;
			}
			int formationRandomValue = (*g_pSentinel * 41 + 31) & 0x7fffff;
			*g_pSentinel = formationRandomValue;
			SetFormationIndex(formationRandomValue % 3);
			SendNewWaypoint(escapeDestination);
			m_runAwayActive = 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0041f730
void CSheepGroup::CheckAgainstLemmings()
{
	AiCoord coordinate;
	CVsRect bounds;
	GetBoundingBox(bounds);
	if (g_pGroupAI->PlayerCheckGroupIntersection(&bounds, &coordinate) == 1) {
		RunAway(coordinate);
		return;
	}
	if (g_pGroupAI->EnemyCheckGroupIntersection(&bounds, &coordinate) == 1) {
		RunAway(coordinate);
		return;
	}
	bool result = g_pGroupAI->BulletCheckGroupIntersection(&bounds, &coordinate);
	if (result == 1) {
		RunAway(coordinate);
		return;
	}
}

// FUNCTION: LEMBALL 0x0041f820
bool CSheepGroup::Process()
{
	CalculateBoundingBox(24);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	CheckAgainstLemmings();
	CheckAgainstCatapults();
	return 0;
}

// FUNCTION: LEMBALL 0x0041f870
bool CSheepGroup::CheckAgainstCatapults()
{
	return 0;
}
