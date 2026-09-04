#include "Ai.h"

#include "../../Control/Game/GameStatus.h"
#include "../Base/GameObject.h"
#include "../Groups/EnemyGroupManager.h"
#include "../Groups/PlayerLemmingGroupManager.h"
#include "../Groups/SheepGroupManager.h"
#include "../Managers/BulletManager.h"
#include "../Managers/DoorManager.h"
#include "../Managers/LiftManager.h"
#include "../Managers/TrampolineManager.h"
#include "../Managers/TrapDoorManager.h"
#include "MoverManager.h"
#include "NodeManager.h"

// 68K 0x1060013a __ct__3CAIFP5CGame
// STUB: LEMBALL 0x00410c10
Ai::Ai(Game* p_arg0)
{
}

// 68K 0x10600286 Restart__3CAIFv
// STUB: LEMBALL 0x00410d00
void Ai::Restart()
{
}

// 68K 0x10601170 Start__3CAIFv
// STUB: LEMBALL 0x00411b10
void Ai::Start()
{
}

// 68K 0x106011ec SendGameState__3CAIF11eGameStates16eGameStateStages
// STUB: LEMBALL 0x00411b70
void Ai::SendGameState(eGameStates p_state, eGameStateStages p_stage)
{
}

// 68K 0x106012d6 RemoteGameState__3CAIFP17CGameStateMessage
// STUB: LEMBALL 0x00411c10
unsigned int Ai::RemoteGameState(GameStateMessage* p_message)
{
	return 0;
}

// 68K 0x1060156a GameState__3CAIF11eGameStatus
// STUB: LEMBALL 0x00411f20
void Ai::GameState(eGameStatus p_status)
{
}

// 68K 0x106016d8 SetPlayerIDs__3CAIFv
// STUB: LEMBALL 0x00412080
void Ai::SetPlayerIDs()
{
}

// 68K 0x10601772 DecideAnimsRequired__3CAIFv
// STUB: LEMBALL 0x00412100
void Ai::DecideAnimsRequired()
{
}

// 68K 0x106018b4 AddTime__3CAIFi
// FUNCTION: LEMBALL 0x004121e0
void Ai::AddTime(int p_time)
{
	m_gameTime += p_time;
}

// 68K 0x106018da Process__3CAIFUc
// STUB: LEMBALL 0x004121f0
void Ai::Process(unsigned char p_paused)
{
}

// 68K 0x10601b88 GetData__3CAIFP9CViewData
// STUB: LEMBALL 0x004124d0
int Ai::GetData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10601c58 HitTrampoline__3CAIFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x004125c0
void Ai::HitTrampoline(const AiCoord& p_position, GameObject* p_object)
{
	m_trampolineManager->Hit(p_position, p_object);
}

// 68K 0x10601ca6 IsLemmingPlayerControlled__3CAIFP14CPlayerLemming
// FUNCTION: LEMBALL 0x004125e0
bool Ai::IsLemmingPlayerControlled(PlayerLemming* p_lemming)
{
	return m_playerGroupManager->IsLemmingPlayerControlled(p_lemming);
}

// 68K 0x10601cf6 FireBullet__3CAIFUs11eBulletType6eOwneri7AICOORD7AICOORD
// STUB: LEMBALL 0x00412600
void Ai::FireBullet(unsigned short p_id,
					eBulletType p_bulletType,
					eOwner p_owner,
					int p_parameter,
					AiCoord p_start,
					AiCoord p_target)
{
}

// 68K 0x10601d94 ProcessMsg__3CAIFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00412660
int Ai::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10601eac PlayerCheckGroupIntersection__3CAIFP7CVSRectP7AICOORD
// FUNCTION: LEMBALL 0x00412780
bool Ai::PlayerCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return m_playerGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// 68K 0x10601f0a EnemyCheckGroupIntersection__3CAIFP7CVSRectP7AICOORD
// FUNCTION: LEMBALL 0x004127a0
bool Ai::EnemyCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return m_enemyGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// 68K 0x10601f66 SheepCheckGroupIntersection__3CAIFP7CVSRectP7AICOORD
// FUNCTION: LEMBALL 0x004127c0
bool Ai::SheepCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return m_sheepGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// 68K 0x10601fc2 BulletCheckGroupIntersection__3CAIFP7CVSRectP7AICOORD
// FUNCTION: LEMBALL 0x004127e0
bool Ai::BulletCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate)
{
	return m_bulletManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// 68K 0x1060201a StepOn__3CAIFRC7AICOORDP11CGameObjectUs
// STUB: LEMBALL 0x00412890
unsigned int Ai::StepOn(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask)
{
	return 0;
}

// 68K 0x10602280 OpenDoor__3CAIFRC7AICOORDP11CGameObjectUs
// STUB: LEMBALL 0x00412ad0
bool Ai::OpenDoor(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask)
{
	return 0;
}

// 68K 0x10602334 GetNodePosition__3CAIFi
// FUNCTION: LEMBALL 0x00412b60
Pt3 Ai::GetNodePosition(int p_node)
{
	return m_nodeManager->GetNodePosition(p_node);
}

// 68K 0x1060238e AddData__3CAIFv
// STUB: LEMBALL 0x00412b80
void Ai::AddData()
{
}

// 68K 0x10602402 GetData__3CAIFv
// STUB: LEMBALL 0x00412be0
void Ai::GetData()
{
}

// 68K 0x10602470 LemmingsSFXChanged__3CAIFv
// FUNCTION: LEMBALL 0x00412c50
bool Ai::LemmingsSfxChanged()
{
	return m_playerGroupManager->HasSfxChanged();
}

// 68K 0x106024a4 QuitGame__3CAIFv
// FUNCTION: LEMBALL 0x00412c60
void Ai::QuitGame()
{
	m_paused = 0;
	g_pGameStatus->m_skillState = 5;
	GameState((eGameStatus) 5);
}

// 68K 0x106024e8 SwitchMessage__3CAIF9swMessageiii
// STUB: LEMBALL 0x00412c80
void Ai::SwitchMessage(swMessage p_message, int p_first, int p_last, int p_arg3)
{
}

// 68K 0x106025ce GetPlayerPos__3CAIFiR7AICOORD
// STUB: LEMBALL 0x00412dc0
void Ai::GetPlayerPos(int p_id, AiCoord& p_position)
{
}

// 68K 0x1060264e GetOrigin__3CAIFR7AICOORDRUc
// FUNCTION: LEMBALL 0x00412e20
void Ai::GetOrigin(AiCoord& p_origin, unsigned int& p_player)
{
	p_player = 0;
	m_playerGroupManager->GetLeaderPos(p_origin);
}

// 68K 0x1060268e AddNewTrapDoor__3CAIFiiiUl
// FUNCTION: LEMBALL 0x00412eb0
void Ai::AddNewTrapDoor(int p_x, int p_y, int p_z, unsigned long p_time)
{
	short id = GameObject::NextLoadingId();
	AiCoord position(p_x << 12, p_y << 12, p_z << 12);
	m_trapDoorManager->AddNewDoor(id, position, 0, p_time);
}

// 68K 0x1060272a LevelName__3CAIFv
// FUNCTION: LEMBALL 0x00412f00
Game* Ai::LevelName()
{
	return m_game;
}

// 68K 0x10602750 LoadLevel__3CAIFPUciUc
// STUB: LEMBALL 0x00412f10
void Ai::LoadLevel(unsigned char* p_data, int p_version, unsigned char p_skip)
{
}

// 68K 0x1060281a FixUpLevel__3CAIFv
// STUB: LEMBALL 0x00412fb0
void Ai::FixUpLevel()
{
}

// 68K 0x10602894 DoorId__3CAIFi
// FUNCTION: LEMBALL 0x00413000
unsigned short Ai::DoorId(int p_index)
{
	return m_doorManager->Id(p_index);
}

// 68K 0x106028c0 LiftId__3CAIFi
// FUNCTION: LEMBALL 0x00413020
unsigned short Ai::LiftId(int p_index)
{
	return m_liftManager->Id(p_index);
}

// 68K 0x106028ec GetDead__3CAIFv
// FUNCTION: LEMBALL 0x00413040
PlayerLemming* Ai::GetDead()
{
	return m_playerGroupManager->GetDead();
}

// 68K 0x10602914 GetObjectRequired__3CAIF11eObjectType
// FUNCTION: LEMBALL 0x00413090
bool Ai::GetObjectRequired(eObjectType p_objectType)
{
	return 1;
}

// 68K 0x10602948 SetObjectRequired__3CAIF11eObjectTypeUc
// FUNCTION: LEMBALL 0x004130a0
void Ai::SetObjectRequired(eObjectType p_objectType, unsigned int p_required)
{
	if (p_objectType < (eObjectType) 0x211 || p_objectType > (eObjectType) 0x212) {
		m_objectRequired[p_objectType] = p_required;
	}
}

// 68K 0x106029a6 FindMoverHeight__3CAIFiiRi
// FUNCTION: LEMBALL 0x004130d0
Mover* Ai::FindMoverHeight(int p_x, int p_y, int& p_height)
{
	return m_moverManager->Find(p_x, p_y, p_height);
}

// 68K 0x106029e6 NLemmings__3CAIFi
// FUNCTION: LEMBALL 0x004130f0
void Ai::NLemmings(int p_count)
{
	m_lemmingCount = p_count;
}

// 68K 0x10602a0c AddANetworkStart__3CAIFiiii
// FUNCTION: LEMBALL 0x00413180
void Ai::AddANetworkStart(int p_x, int p_y, int p_z, int p_index)
{
	m_networkStartsX[p_index] = p_x;
	m_networkStartsY[p_index] = p_y;
	m_networkStartsZ[p_index] = p_z;
}

// 68K 0x10602a54 SetNetworkTrapDoors__3CAIFiiiii
// STUB: LEMBALL 0x004131e0
void Ai::SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth)
{
}

// 68K 0x10602b0a LoadFlagInfo__3CAIFPUci
// STUB: LEMBALL 0x00413300
void Ai::LoadFlagInfo(unsigned char* p_data, int p_size)
{
}

// 68K 0x10602bc2 nDead__3CAIFv
// FUNCTION: LEMBALL 0x00413370
int Ai::NDead()
{
	return m_playerGroupManager->m_deadCount;
}

// 68K 0x10602be6 Score__3CAIFi
// FUNCTION: LEMBALL 0x00413390
void Ai::Score(int p_score)
{
	m_score += p_score;
	if (m_score > 9999999) {
		m_score = 9999999;
	}
}

// 68K 0x101192f0 Process__3CAIFv
// FUNCTION: LEMBALL 0x00413e20
void Ai::Process()
{
	Process(0);
}

// 68K 0x10600d02 __dt__3CAIFv
Ai::~Ai()
{
}

// GLOBAL: LEMBALL 0x004a782c
Ai* g_pGenericGroupAI;

// GLOBAL: LEMBALL 0x004a74b0
Ai* g_pAI;

// GLOBAL: LEMBALL 0x004a74b8
int g_nGameOver = 0;

// GLOBAL: LEMBALL 0x0049cf34
Ai* g_pActiveAI = 0;
