#include "CSheepGroup.h"

#include "../../Control/Game/CGame.h"
#include "../../Visos/Foundation/CVsMath.h"
#include "../../Visos/Foundation/CVsRect.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../Navigation/CAi.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Groups/CGenericGroup.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/CFixed.h"
#include "Visos/Foundation/CVector.h"

// FUNCTION: LEMBALL 0x0041f500
CSheepGroup::CSheepGroup(CAi* p_ai, CObjectManager* p_objectManager, CFormationManager* p_formationManager)
	: CGenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// FUNCTION: LEMBALL 0x0041f530
void CSheepGroup::RunAway(AiCoord p_threatPosition)
{
	int count;
	CGameObject* object;
	AiCoord destination;
	count = 0;
	CVector escape(0x32000, 0);
	object = GetFirstElementInGroup();
	while (object != 0) {
		if (object->DestinationExists() == 1) {
			count++;
		}
		object = GetNextElementInGroup();
	}
	if (count == 0) {
		CGameObject* first = GetFirstElementInGroup();
		if (first != 0) {
			first->SetSndEffect(SFX_SHEEP);
			int positionY = first->m_position.m_yFixed;
			int positionZ = first->m_position.m_zFixed;
			destination.m_xFixed = first->m_position.m_xFixed;
			destination.m_yFixed = positionY;
			destination.m_zFixed = positionZ;
			int angle = ((ReturnFacingDirection(destination.m_xFixed >> 12,
												destination.m_yFixed >> 12,
												p_threatPosition.m_xFixed >> 12,
												p_threatPosition.m_yFixed >> 12) +
						  1) &
						 7) *
						64;
			VsTrig* trig = g_pVSTrig;
			int sineValue;
			int cosineValue;
			if (angle < 0) {
				sineValue = -trig->m_sine[(-angle) % 512].m_value;
			}
			else {
				sineValue = g_pVSTrig->m_sine[angle % 512].m_value;
			}
			CFixed sine(sineValue);
			int cosineAngle = angle + 128;
			if (cosineAngle < 0) {
				cosineValue = -g_pVSTrig->m_sine[(-128 - angle) % 512].m_value;
			}
			else {
				cosineValue = g_pVSTrig->m_sine[cosineAngle % 512].m_value;
			}
			CFixed cosine(cosineValue);
			CVector rotated = trig->Rotate(escape, sine, cosine);
			destination.m_xFixed += rotated.m_xFixed;
			destination.m_yFixed += rotated.m_yFixed;
			if ((destination.m_xFixed >> 12) < 0) {
				destination.m_xFixed = 0;
			}
			if ((destination.m_yFixed >> 12) < 0) {
				destination.m_yFixed = 0;
			}
			int random = (*g_pSentinel * 41 + 31) & 0x7fffff;
			*g_pSentinel = random;
			SetFormationIndex(random % 3);
			SendNewWaypoint(destination);
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
	if (g_pUnknown0x4a7824->PlayerCheckGroupIntersection(&bounds, &coordinate) == 1) {
		RunAway(coordinate);
		return;
	}
	if (g_pUnknown0x4a7824->EnemyCheckGroupIntersection(&bounds, &coordinate) == 1) {
		RunAway(coordinate);
		return;
	}
	bool result = g_pUnknown0x4a7824->BulletCheckGroupIntersection(&bounds, &coordinate);
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
