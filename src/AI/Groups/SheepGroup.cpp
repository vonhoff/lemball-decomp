#include "SheepGroup.h"

#include "../../Visos/Foundation/VsRect.h"
#include "../Navigation/Ai.h"

// 68K 0x1061e4d6 __ct__11CSheepGroupFP3CAIP14CObjectManagerP17CFormationManager
// FUNCTION: LEMBALL 0x0041f500
SheepGroup::SheepGroup(Ai* p_ai, ObjectManager* p_objectManager, FormationManager* p_formationManager)
	: GenericGroup(p_ai, p_objectManager, p_formationManager)
{
}

// 68K 0x1061e552 RunAway__11CSheepGroupF7AICOORD
// STUB: LEMBALL 0x0041f530
bool SheepGroup::RunAway(AiCoord p_threatPosition)
{
	return 0;
}

// 68K 0x1061e744 CheckAgainstLemmings__11CSheepGroupFv
// FUNCTION: LEMBALL 0x0041f730
bool SheepGroup::CheckAgainstLemmings()
{
	AiCoord coordinate;
	VsRect bounds;
	GetBoundingBox(bounds);
	if (g_pUnknown0x4a7824->PlayerCheckGroupIntersection(&bounds, &coordinate) == 1) {
		return RunAway(coordinate);
	}
	if (g_pUnknown0x4a7824->EnemyCheckGroupIntersection(&bounds, &coordinate) == 1) {
		return RunAway(coordinate);
	}
	bool result = g_pUnknown0x4a7824->BulletCheckGroupIntersection(&bounds, &coordinate);
	if (result == 1) {
		return RunAway(coordinate);
	}
	return result;
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
