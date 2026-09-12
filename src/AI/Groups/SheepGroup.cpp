#include "SheepGroup.h"

#include "../../Visos/Foundation/VsMath.h"
#include "../../Visos/Foundation/VsRect.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../Navigation/Ai.h"

// 68K 0x1061e4d6 __ct__11CSheepGroupFP3CAIP14CObjectManagerP17CFormationManager
// FUNCTION: LEMBALL 0x0041f500
SheepGroup::SheepGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager)
	: GenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// 68K 0x1061e552 RunAway__11CSheepGroupF7AICOORD
// FUNCTION: LEMBALL 0x0041f530
void SheepGroup::RunAway(AiCoord p_threatPosition)
{
	int count;
	GameObject* object;
	AiCoord destination;
	count = 0;
	Vector escape(0x32000, 0);
	object = GetFirstElementInGroup();
	while (object != 0) {
		if (object->DestinationExists() == 1) {
			count++;
		}
		object = GetNextElementInGroup();
	}
	if (count == 0) {
		GameObject* first = GetFirstElementInGroup();
		if (first != 0) {
			first->SetSndEffect((eSoundEffect) 4);
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
			Fixed sine(sineValue);
			int cosineAngle = angle + 128;
			if (cosineAngle < 0) {
				cosineValue = -g_pVSTrig->m_sine[(-128 - angle) % 512].m_value;
			}
			else {
				cosineValue = g_pVSTrig->m_sine[cosineAngle % 512].m_value;
			}
			Fixed cosine(cosineValue);
			Vector rotated = trig->Rotate(escape, sine, cosine);
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

// 68K 0x1061e744 CheckAgainstLemmings__11CSheepGroupFv
// FUNCTION: LEMBALL 0x0041f730
void SheepGroup::CheckAgainstLemmings()
{
	AiCoord coordinate;
	VsRect bounds;
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

// 68K 0x1061e870 Process__11CSheepGroupFv
// FUNCTION: LEMBALL 0x0041f820
bool SheepGroup::Process()
{
	CalculateBoundingBox(24);
	for (int i = 0; i < m_elementCount; i++) {
		m_elements[i]->Process();
	}
	CheckAgainstLemmings();
	CheckAgainstCatapults();
	return 0;
}

// 68K 0x1061e8e8 CheckAgainstCatapults__11CSheepGroupFv
// FUNCTION: LEMBALL 0x0041f870
bool SheepGroup::CheckAgainstCatapults()
{
	return 0;
}

// 68K 0x1011b3b6 __dt__11CSheepGroupFv
SheepGroup::~SheepGroup()
{
}
