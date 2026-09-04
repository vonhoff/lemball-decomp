#include "PlayerLemmingGroup.h"

// 68K 0x1060e81a __ct__19CPlayerLemmingGroupFP3CAIP14CObjectManagerP17CFormationManager
// STUB: LEMBALL 0x00414010
PlayerLemmingGroup::PlayerLemmingGroup(Ai* p_arg0, ObjectManager* p_arg1, FormationManager* p_arg2)
{
}

// 68K 0x1060e89e Restart__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414040
void PlayerLemmingGroup::Restart()
{
	GenericGroup::Restart();
	m_formationIndex = 0;
	m_altered = 0;
	m_playerControlled = 0;
	m_useObject = 0;
}

// 68K 0x1060e946 GetViewData__19CPlayerLemmingGroupFP9CViewData
// STUB: LEMBALL 0x00414080
void PlayerLemmingGroup::GetViewData(ViewData* p_viewData)
{
}

// 68K 0x1060ea70 Delete__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414130
void PlayerLemmingGroup::Delete()
{
	if (m_useObject != 0) {
		m_useObject->m_unk0x8c = 0;
	}
	m_useObject = 0;
}

// 68K 0x1060eab2 Process__19CPlayerLemmingGroupFv
// STUB: LEMBALL 0x00414150
bool PlayerLemmingGroup::Process()
{
	return 0;
}

// 68K 0x1060f0f2 AddLemmingToGroup__19CPlayerLemmingGroupFP14CPlayerLemming
// STUB: LEMBALL 0x00414600
bool PlayerLemmingGroup::AddLemmingToGroup(PlayerLemming* p_lemming)
{
	return 0;
}

// 68K 0x1060f19e AddUseObject__19CPlayerLemmingGroupFi
// STUB: LEMBALL 0x00414660
void PlayerLemmingGroup::AddUseObject(int p_objectId)
{
}

// 68K 0x1060f2a6 AddUseObject__19CPlayerLemmingGroupFP11CGameObjecti
// STUB: LEMBALL 0x00414730
void PlayerLemmingGroup::AddUseObject(GameObject* p_object, int p_objectId)
{
}

// 68K 0x1060f38c RemoveLemmingFromGroup__19CPlayerLemmingGroupFP14CPlayerLemming
// STUB: LEMBALL 0x004147d0
bool PlayerLemmingGroup::RemoveLemmingFromGroup(PlayerLemming* p_lemming)
{
	return 0;
}

// 68K 0x1060f420 SetPlayerControlled__19CPlayerLemmingGroupFUcP14CPlayerLemming
// STUB: LEMBALL 0x00414810
void PlayerLemmingGroup::SetPlayerControlled(unsigned char p_playerControlled, PlayerLemming* p_leader)
{
}

// 68K 0x1060f4e4 CheckPlayerControlled__19CPlayerLemmingGroupFv
// FUNCTION: LEMBALL 0x00414880
bool PlayerLemmingGroup::CheckPlayerControlled()
{
	return m_playerControlled;
}

// 68K 0x1060f526 GetFirstDeadLemming__19CPlayerLemmingGroupFv
// STUB: LEMBALL 0x00414890
PlayerLemming* PlayerLemmingGroup::GetFirstDeadLemming()
{
	return 0;
}

// 68K 0x1060f596 ClearExistingWaypoints__19CPlayerLemmingGroupFv
// STUB: LEMBALL 0x004148f0
void PlayerLemmingGroup::ClearExistingWaypoints()
{
}

// 68K 0x1060f66a HasSFXChanged__19CPlayerLemmingGroupFv
// STUB: LEMBALL 0x004149a0
bool PlayerLemmingGroup::HasSfxChanged()
{
	return 0;
}

// 68K 0x1060e8ec __dt__19CPlayerLemmingGroupFv
PlayerLemmingGroup::~PlayerLemmingGroup()
{
}
