#include "Ai.h"

#include "../../Control/Game/Demo.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameStatus.h"
#include "../../Control/Game/GameTime.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Map/Base/Map.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Network/Game/PbNetworkGame.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Connect.h"
#include "../Base/GameObject.h"
#include "../Groups/EnemyGroupManager.h"
#include "../Groups/FormationManager.h"
#include "../Groups/PlayerLemmingGroupManager.h"
#include "../Groups/SheepGroupManager.h"
#include "../Managers/BallManager.h"
#include "../Managers/BulletManager.h"
#include "../Managers/CollectableManager.h"
#include "../Managers/DoorManager.h"
#include "../Managers/GodManager.h"
#include "../Managers/HandManager.h"
#include "../Managers/IceManager.h"
#include "../Managers/InvisibleSwitchManager.h"
#include "../Managers/LaserManager.h"
#include "../Managers/LiftManager.h"
#include "../Managers/MineManager.h"
#include "../Managers/ObjectManager.h"
#include "../Managers/PaintGunManager.h"
#include "../Managers/RocketManager.h"
#include "../Managers/SlinkyManager.h"
#include "../Managers/TrampolineManager.h"
#include "../Managers/TrapDoorManager.h"
#include "../Messages/GameStateMessage.h"
#include "../Objects/AnimSpecial.h"
#include "../Objects/Ball.h"
#include "../Objects/BalloonPost.h"
#include "../Objects/GroundAnim.h"
#include "../Objects/PlayerLemming.h"
#include "../Objects/ViewData.h"
#include "AiCursor.h"
#include "Maze.h"
#include "MoverManager.h"
#include "NodeManager.h"

extern int g_anDefaultTrapDoorLemmings[4][4];

// 68K 0x1060013a __ct__3CAIFP5CGame
// FUNCTION: LEMBALL 0x00410c10
Ai::Ai(Game* p_arg0)
{
	m_collisionPoint.m_x = 0;
	m_collisionPoint.m_y = 0;
	m_collisionPoint.m_z = 0;
	m_collisionRect.m_x1 = 0;
	m_collisionRect.m_y1 = 0;
	m_collisionRect.m_z1 = 0;
	m_collisionRect.m_x2 = 0xffffffff;
	m_collisionRect.m_y2 = 0xffffffff;
	m_collisionRect.m_z2 = 0xffffffff;
	m_objectCount = 0;
	m_objectCapacity = 100;
	m_objects = new GameObject*[100];
	for (int i = 0; i < m_objectCapacity; i++) {
		m_objects[i] = 0;
	}
	m_game = p_arg0;
	m_initialised = 0;
	g_wObjectCount = 0;
	Restart();
}

// 68K 0x10600286 Restart__3CAIFv
// FUNCTION: LEMBALL 0x00410d00
void Ai::Restart()
{
	int i;
	GameObject::Init(this);
	g_pActiveAI = this;
	m_objectCount = 0;
	for (i = 0; i < m_objectCapacity; i++) {
		m_objects[i] = 0;
	}
	g_wNetworkLemmingIndex = 0;
	g_wLocalLemmingIndex = 0;
	g_pGameStatus->m_skillState = 0;
	m_isSinglePlayer = 0;
	ResetGameTimes();
	m_unk0x88 = 0;
	unsigned int network = g_pGameStatus->m_skill == 4;
	m_unk0x68 = 0;
	m_unk0x6c = 0;
	m_isHost = 0;
	m_unk0x5c = 0;
	m_playerGroups = 0;
	m_unk0x70 = 1;
	m_payloadCapacity += 0x60;
	m_networkMode = network;
	if (m_networkMode != 0) {
		if (m_initialised == 0) {
			m_networkGame = new PbNetworkGame(this);
		}
		m_isHost = g_pActiveConnection->m_isHost;
		if (m_initialised == 0) {
			m_gameStateMessage = new GameStateMessage;
		}
		g_pGameStatus->m_levelState = 0;
	}
	int* networkStart = m_networkStartsZ;
	int networkStartCount = 4;
	do {
		*networkStart = 0;
		networkStart++;
		networkStart[-5] = 0;
		networkStart[-9] = 0;
		networkStart[3] = 0;
		networkStartCount--;
	} while (networkStartCount != 0);
	m_networkTrapDoors[0] = 4;
	m_levelVersion = 0;
	m_networkTrapDoorCount = 1;
	m_unk0xd4 = 4;
	m_flagCounts[1] = 1;
	m_gameTime = 0;
	m_lemmingCount = 4;
	m_flagCounts[0] = 1;
	m_score = g_pGameStatus->m_levelState;
	m_unk0xf4 = m_score;
	m_paused = 0;
	g_wLemmingCount = 0;
	m_unk0xdc = 1;
	m_mapType = 0;
	m_unk0xe4 = 0;
	m_gameStatus = 0;
	m_processState = 0;
	m_timeLimit = 180;
	if (m_initialised == 0) {
		m_map = new Map;
	}
	m_map->Restart();
	g_pMap = m_map;
	Map* map = m_map;
	map->m_ai = this;
	map->m_unk0x18 = this;
	if (m_initialised == 0) {
		m_maze = new Maze(m_map);
	}
	g_pMaze = m_maze;
	if (m_initialised == 0) {
		m_aiQueue = new BaseQueue(10, "AIQueue");
		m_aiQueue->Attach(this, 0);
	}
	if (m_networkMode != 0 && g_pActiveConnection != 0) {
		g_pActiveConnection->ReadSocket::UnUseAllNc();
		g_pActiveConnection->ReadSocket::UnUseAllC();
	}
	if (m_initialised == 0) {
		g_pGodManager = new GodManager(20);
	}
	else {
		g_pGodManager->Restart();
	}
	if (m_initialised == 0) {
		m_liftManager = new LiftManager(this, 60);
		g_pGodManager->Register(m_liftManager);
	}
	if (m_initialised == 0) {
		m_moverManager = new MoverManager(this, 40);
		g_pGodManager->Register(m_moverManager);
	}
	if (m_initialised == 0) {
		m_objectManager = new ObjectManager(this, 60);
		g_pGodManager->Register(m_objectManager);
	}
	if (m_initialised == 0) {
		m_formationManager = new FormationManager();
	}
	m_formationManager->Restart();
	if (m_initialised == 0) {
		m_playerGroupManager = new PlayerLemmingGroupManager(this, m_objectManager, m_formationManager);
		g_pGodManager->Register(m_playerGroupManager);
	}
	if (m_initialised == 0) {
		m_bulletManager = new BulletManager();
		g_pGodManager->Register(m_bulletManager);
	}
	if (m_initialised == 0) {
		m_cursor = new AiCursor(this, 1024, 1024);
	}
	if (m_initialised == 0) {
		m_sheepGroupManager = new SheepGroupManager(this, m_objectManager, m_formationManager);
	}
	m_sheepGroupManager->Restart();
	if (m_initialised == 0) {
		m_enemyGroupManager = new EnemyGroupManager(this, m_objectManager, m_formationManager);
	}
	m_enemyGroupManager->Restart();
	if (m_initialised == 0) {
		m_nodeManager = new NodeManager(300);
	}
	m_nodeManager->Restart();
	if (m_initialised == 0) {
		m_ballManager = new BallManager(this, 20);
	}
	m_ballManager->Restart();
	if (m_initialised == 0) {
		m_collectableManager = new CollectableManager(this, 30);
		g_pGodManager->Register(m_collectableManager);
	}
	if (m_initialised == 0) {
		m_mineManager = new MineManager(this, 40);
		g_pGodManager->Register(m_mineManager);
	}
	if (m_initialised == 0) {
		m_doorManager = new DoorManager(this, 20);
		g_pGodManager->Register(m_doorManager);
	}
	if (m_initialised == 0) {
		m_rocketManager = new RocketManager(this, 20);
		g_pGodManager->Register(m_rocketManager);
	}
	if (m_initialised == 0) {
		m_handManager = new HandManager(this, 20);
		g_pGodManager->Register(m_handManager);
	}
	if (m_initialised == 0) {
		m_laserManager = new LaserManager(this, 20);
		g_pGodManager->Register(m_laserManager);
	}
	if (m_initialised == 0) {
		m_groundAnim = new GroundAnim();
	}
	m_groundAnim->Restart();
	if (m_initialised == 0) {
		m_balloonPost = new BalloonPost(this, m_map);
	}
	m_balloonPost->Restart();
	if (m_initialised == 0) {
		m_trampolineManager = new TrampolineManager(this, 20);
		g_pGodManager->Register(m_trampolineManager);
	}
	if (m_initialised == 0) {
		m_paintGunManager = new PaintGunManager(this, 20);
		g_pGodManager->Register(m_paintGunManager);
	}
	if (m_initialised == 0) {
		m_iceManager = new IceManager(this, 100);
		g_pGodManager->Register(m_iceManager);
	}
	if (m_initialised == 0) {
		m_trapDoorManager = new TrapDoorManager();
		g_pGodManager->Register(m_trapDoorManager);
	}
	if (m_initialised == 0) {
		m_slinkyManager = new SlinkyManager(this, 20);
	}
	m_slinkyManager->Restart();
	if (m_initialised == 0) {
		m_invisibleSwitchManager = new InvisibleSwitchManager(this, 40);
		g_pGodManager->Register(m_invisibleSwitchManager);
	}
	if (m_initialised == 0) {
		m_levelLoader = new LevelLoader(this);
	}

	int level;
	eSkill skill;
	if (g_nDemoMode != 0) {
		unsigned char packet[2];
		unsigned long packetSize;
		g_pDemo->SetDemoMode(1);
		g_pDemo->GetUserPacket(packet, packetSize);
		level = packet[0];
		skill = (eSkill) packet[1];
		*g_pSysOutput << "Starting demo mode for level " << level << " on skill " << (int) skill << "\n";
		*g_pSentinel = 0xad28;
	}
	else {
		level = g_pGameStatus->Level();
		skill = (eSkill) g_pGameStatus->m_skill;
	}
	m_levelLoader->LoadLevel(skill, level, m_initialised);
	m_playerGroupManager->InitialiseNetwork();
	if (m_initialised == 0) {
		SetPlayerIDs();
		m_maze->Initialise();
	}
	else {
		m_maze->ReInitialise();
	}
	if (m_initialised == 0) {
		m_animSpecial = new AnimSpecial;
	}
	m_animSpecial->Initialise(m_map);
	if (m_levelVersion == 0) {
		FixUpLevel();
	}
	m_levelVersion = 10;
	if (m_initialised == 0) {
		m_objectRequired = new unsigned int[55];
	}
	for (i = 0; i < 55; i++) {
		m_objectRequired[i] = 0;
	}
	DecideAnimsRequired();
	m_initialised = 1;
}

// 68K 0x10601170 Start__3CAIFv
// FUNCTION: LEMBALL 0x00411b10
void Ai::Start()
{
	Demo* demo;
	NetworkManager* networkManager;

	if (m_networkMode != 0) {
		m_unk0x6c = 1;
		m_unk0x70 = 0;
		networkManager = g_pNetworkManager;
		networkManager->m_desiredGameState = 3;
		networkManager->m_observedGameState = 0;
		return;
	}

	if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
		demo = g_pDemo;
		demo->m_startTime = CurrentMilliTimer();
		demo->m_duration = 0;
	}

	GameState((eGameStatus) 2);
	m_unk0x68 = 1;
}

// 68K 0x106011ec SendGameState__3CAIF11eGameStates16eGameStateStages
// FUNCTION: LEMBALL 0x00411b70
void Ai::SendGameState(eGameStates p_state, eGameStateStages p_stage)
{
	if (g_pActiveConnection != 0) {
		if (m_gameStateMessage->m_pendingSendCount != 0) {
			unsigned long start = CurrentMilliTimer();
			while (m_gameStateMessage->m_pendingSendCount != 0 && CurrentMilliTimer() - start < 2000) {
				g_pBaseNetwork->WaitProcess();
			}
		}
		if (m_gameStateMessage->m_pendingSendCount == 0) {
			m_unk0x6c = 1;
			m_gameStateMessage->m_state = p_state;
			m_gameStateMessage->m_stage = p_stage;
			m_gameStateMessage->m_levelTime = m_gameTime;
			m_gameStateMessage->m_score = m_score;
			m_gameStateMessage->Send(g_pActiveConnection);
		}
	}
}

// 68K 0x106012d6 RemoteGameState__3CAIFP17CGameStateMessage
// STUB: LEMBALL 0x00411c10
unsigned int Ai::RemoteGameState(GameStateMessage* p_message)
{
	return 0;
}

// 68K 0x1060156a GameState__3CAIF11eGameStatus
// FUNCTION: LEMBALL 0x00411f20
void Ai::GameState(eGameStatus p_status)
{
	if (m_networkMode == 0) {
		switch (p_status) {
		case (eGameStatus) 3:
			g_pGameStatus->m_skillState = 2;
			m_gameStatus = 3;
			return;
		case (eGameStatus) 5:
			if (g_pGameStatus->m_skillState == 0) {
				g_pGameStatus->m_skillState = 3;
			}
			m_gameStatus = 5;
			return;
		case (eGameStatus) 7:
			g_pGameStatus->m_skillState = 4;
			m_gameStatus = 5;
			return;
		default:
			m_gameStatus = p_status;
			return;
		}
	}
	if (m_unk0x6c == 0) {
		switch (p_status) {
		case (eGameStatus) 1:
			m_isSinglePlayer = 0;
			SendGameState((eGameStates) 0, (eGameStateStages) 0);
			return;
		case (eGameStatus) 2:
			SendGameState((eGameStates) 1, (eGameStateStages) 1);
			if (m_gameStatus == 2) {
				m_unk0x6c = 0;
				return;
			}
			break;
		case (eGameStatus) 3:
			SendGameState((eGameStates) 2, (eGameStateStages) 0);
			return;
		case (eGameStatus) 4:
			SendGameState((eGameStates) 3, (eGameStateStages) 0);
			return;
		case (eGameStatus) 5:
			if (g_pGameStatus->m_skillState == 5) {
				SendGameState((eGameStates) 6, (eGameStateStages) 0);
				return;
			}
			SendGameState((eGameStates) 4, (eGameStateStages) 0);
			return;
		case (eGameStatus) 7:
			SendGameState((eGameStates) 7, (eGameStateStages) 0);
			return;
		case (eGameStatus) 8:
			SendGameState((eGameStates) 8, (eGameStateStages) 0);
		}
	}
}

// 68K 0x106016d8 SetPlayerIDs__3CAIFv
// FUNCTION: LEMBALL 0x00412080
void Ai::SetPlayerIDs()
{
	if (g_pActiveConnection != 0) {
		int offsets[2] = {0, 0};
		if (g_pActiveConnection->m_isHost != 0) {
			offsets[0] = 4;
		}
		else {
			offsets[1] = 4;
		}

		int* offset = offsets;
		do {
			PlayerLemming** lemming = m_networkLemmings + *offset;
			int count = 4;
			do {
				(*lemming)->SetId(GameObject::NextLoadingId());
				lemming++;
				count--;
			} while (count != 0);
			offset++;
		} while (offset < offsets + 2);
	}
}

// 68K 0x10601772 DecideAnimsRequired__3CAIFv
// FUNCTION: LEMBALL 0x00412100
void Ai::DecideAnimsRequired()
{
	int count = (unsigned int) g_wObjectCount;
	int i = 0;
	for (;;) {
		if (i >= count) {
			break;
		}
		GameObject* object = g_pObjects[(unsigned short) i];
		if (object != 0 && object->m_objectType != (eObjectType) 0xffff) {
			SetObjectRequired(object->m_objectType, 1);
		}
		i++;
	}
	if (m_ballManager->m_activeCount == 0) {
		SetObjectRequired((eObjectType) 9, 0);
	}
	if (m_doorManager->m_count == 0) {
		SetObjectRequired((eObjectType) 0x1a, 0);
		SetObjectRequired((eObjectType) 0x19, 0);
	}
	if (m_laserManager->m_count == 0) {
		SetObjectRequired((eObjectType) 0x2f, 0);
		SetObjectRequired((eObjectType) 0x1e, 0);
	}
	if (m_mineManager->m_count == 0) {
		SetObjectRequired((eObjectType) 0x13, 0);
	}
	if (m_rocketManager->m_count == 0) {
		SetObjectRequired((eObjectType) 0x20, 0);
	}
	if (m_slinkyManager->m_count == 0) {
		SetObjectRequired((eObjectType) 0x35, 0);
	}
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
// FUNCTION: LEMBALL 0x004124d0
int Ai::GetData(ViewData* p_viewData)
{
	int count = g_pGodManager->GetViewData(p_viewData);
	count += m_enemyGroupManager->GetViewData(p_viewData + count);
	count += m_sheepGroupManager->GetViewData(p_viewData + count);
	int i = 0;
	BallManager* balls = m_ballManager;
	if (balls->m_activeCount > 0) {
		ViewData* data = p_viewData + count;
		do {
			balls->m_balls[i]->GetViewData(*data);
			data++;
			i++;
		} while (i < balls->m_activeCount);
	}
	count += balls->m_activeCount;
	count += m_balloonPost->GetViewData(p_viewData + count);
	count += m_slinkyManager->GetViewData(p_viewData + count);
	return count;
}

// 68K 0x10601c58 HitTrampoline__3CAIFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x004125c0
int Ai::HitTrampoline(const AiCoord& p_position, GameObject* p_object)
{
	return m_trampolineManager->Hit(p_position, p_object);
}

// 68K 0x10601ca6 IsLemmingPlayerControlled__3CAIFP14CPlayerLemming
// FUNCTION: LEMBALL 0x004125e0
bool Ai::IsLemmingPlayerControlled(PlayerLemming* p_lemming)
{
	return m_playerGroupManager->IsLemmingPlayerControlled(p_lemming);
}

// 68K 0x10601cf6 FireBullet__3CAIFUs11eBulletType6eOwneri7AICOORD7AICOORD
// FUNCTION: LEMBALL 0x00412600
void Ai::FireBullet(unsigned short p_id,
					eBulletType p_bulletType,
					eOwner p_owner,
					int p_parameter,
					AiCoord p_start,
					AiCoord p_target)
{
	m_bulletManager->RequestBullet(p_id, p_bulletType, p_owner, p_parameter, p_start, p_target);
}

// 68K 0x10601d94 ProcessMsg__3CAIFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00412660
int Ai::ProcessMsg(Message* p_message)
{
	unsigned int messageType = p_message->type;
	if (messageType != 4) {
		if (m_playerGroups == 0) {
			return 1;
		}
		switch (messageType) {
		case 2:
			m_playerGroupManager->AddNewWaypointToCurrentGroup(p_message->code, (int) p_message->payload);
			return 0;
		case 3:
			m_playerGroupManager->RemoveWaypointsFromCurrentGroup();
			return 0;
		case 5:
			m_playerGroupManager->CreateNewGroup((unsigned short) p_message->code,
												 (unsigned short*) p_message->payload);
			return 0;
		case 6:
			m_playerGroupManager->MakePreviousGroupPlayerControlled();
			return 0;
		case 7:
			m_playerGroupManager->MakeNextGroupPlayerControlled();
			return 0;
		case 8:
			m_playerGroupManager->UseObject(p_message->code);
			return 0;
		default:
			m_processedCount = m_processedCount + 1;
			return 0;
		}
	}
	m_playerGroupManager->PlayerGroupRequestFire(p_message->code, (int) p_message->payload);
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
// FUNCTION: LEMBALL 0x00412890
void Ai::StepOn(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask)
{
	int y;
	int x = p_position.m_xFixed >> 12;
	y = p_position.m_yFixed >> 12;
	int blockX = x / 16;
	int blockY = y / 16;

	if (p_object->m_unk0x11c != 0) {
		return;
	}

	unsigned short groundZ;
	{
		int groundX = x >> 4;
		int groundY = y >> 4;
		Map* map = m_map;
		if (x < 0 || y < 0 || groundX >= map->m_ground.m_width || groundY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			int cellX = x & 0xf;
			int cellY = y & 0xf;
			groundZ = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(cellX, cellY);
		}
	}

	if ((int) groundZ + 4 < (p_object->m_position.m_zFixed >> 12)) {
		return;
	}

	unsigned short collision;
	if (blockX < 0 || blockY < 0) {
		collision = 3;
	}
	else {
		int width = m_map->m_ground.m_width;
		if (width <= blockX || m_map->m_ground.m_height <= blockY) {
			collision = 3;
		}
		else {
			collision = m_map->m_ground.m_ground[blockY * width + blockX].m_collision;
		}
	}

	if (p_object->m_balloonPostId != 0) {
		return;
	}

	if ((collision & 4) != 0 && (p_mask & 0x40) != 0) {
		eObjectType objectType = m_map->m_ground.m_ground[blockY * m_map->m_ground.m_width + blockX].m_objectType;
		p_object->m_actionDeadline = g_dwGameTick + 26;
		p_object->m_action = (eAction) 15;
		if (objectType != (eObjectType) 0x216) {
			p_object->m_actionArgument = 2;
			p_object->m_stateTimer = g_dwGameTick * 50;
			p_object->SetSndEffect((eSoundEffect) 28);
			return;
		}

		p_object->m_actionArgument = 1;
		p_object->m_stateTimer = g_dwGameTick * 50;
		p_object->SetSndEffect((eSoundEffect) 29);
		return;
	}

	if ((collision & 0x8000) == 0) {
		return;
	}

	if ((p_mask & 8) != 0) {
		m_mineManager->StepOn(p_position, p_object);
	}
	if ((p_mask & 0x10) != 0) {
		m_liftManager->StepOn(p_position, p_object);
		m_rocketManager->StepOn(p_position, p_object);
		m_handManager->StepOn(p_position, p_object);
		m_laserManager->StepOn(p_position, p_object);
		m_iceManager->StepOn(p_position, p_object);
	}
	if ((p_mask & 0x100) != 0) {
		m_invisibleSwitchManager->StepOn(p_position, p_object);
	}
}

// 68K 0x10602280 OpenDoor__3CAIFRC7AICOORDP11CGameObjectUs
// FUNCTION: LEMBALL 0x00412ad0
bool Ai::OpenDoor(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask)
{
	int blockX = (p_position.m_xFixed >> 12) / 16;
	int blockY = (p_position.m_yFixed >> 12) / 16;
	unsigned short collision;
	if (blockX < 0 || blockY < 0) {
		collision = 3;
	}
	else {
		Map* map = m_map;
		int width = map->m_ground.m_width;
		if (width <= blockX || map->m_ground.m_height <= blockY) {
			collision = 3;
		}
		else {
			collision = map->m_ground.m_ground[blockY * width + blockX].m_collision;
		}
	}
	if ((collision & 0x8000) != 0 && (p_mask & 0x20) != 0) {
		return m_doorManager->Open(p_position, p_object);
	}
	return false;
}

// 68K 0x10602334 GetNodePosition__3CAIFi
// FUNCTION: LEMBALL 0x00412b60
Pt3 Ai::GetNodePosition(int p_node)
{
	return m_nodeManager->GetNodePosition(p_node);
}

// 68K 0x1060238e AddData__3CAIFv
// FUNCTION: LEMBALL 0x00412b80
void Ai::AddData()
{
	int remaining = 4;
	PlayerLemming** lemming = m_networkLemmings;
	do {
		NetworkMessage* stream = this;
		NetworkMessage* message = (NetworkMessage*) ((unsigned char*) *lemming + sizeof(GlobalGameObject));
		message->CopyDataStream(stream->m_writeCursor, 0);
		stream->m_writeCursor += message->m_writeCursor - message->m_buffer;
		lemming++;
		remaining--;
	} while (remaining != 0);
}

// 68K 0x10602402 GetData__3CAIFv
// FUNCTION: LEMBALL 0x00412be0
void Ai::GetData()
{
	PlayerLemming** lemming = &m_networkLemmings[4];
	for (int i = 0; i < 4; i++) {
		NetworkMessage* stream = this;
		NetworkMessage* message = (NetworkMessage*) ((unsigned char*) *lemming + sizeof(GlobalGameObject));
		if (message->Set(stream->m_readCursor)) {
			stream->m_readCursor = message->m_readCursor;
		}
		lemming++;
	}
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
// FUNCTION: LEMBALL 0x00412dc0
void Ai::GetPlayerPos(int p_id, AiCoord& p_position)
{
	PlayerLemming** lemming = m_networkLemmings;
	int index = 0;
	do {
		if ((*lemming)->m_objectId == p_id) {
			PlayerLemming* player = m_networkLemmings[index];
			p_position.m_xFixed = player->m_position.m_xFixed;
			p_position.m_yFixed = player->m_position.m_yFixed;
			p_position.m_zFixed = player->m_position.m_zFixed;
			return;
		}
		lemming++;
		index++;
	} while (index < 4);
}

// 68K 0x1060264e GetOrigin__3CAIFR7AICOORDRUc
// FUNCTION: LEMBALL 0x00412e20
bool Ai::GetOrigin(AiCoord& p_origin, unsigned int& p_player)
{
	p_player = 0;
	return m_playerGroupManager->GetLeaderPos(p_origin);
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
// FUNCTION: LEMBALL 0x00412f10
void Ai::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	if (p_dataSize > 4) {
		data = (unsigned short*) p_data;
		m_levelVersion = *data++;
	}
	else {
		m_levelVersion = 0;
		data = (unsigned short*) p_data;
	}
	unsigned int mapType = *data++;
	m_mapType = mapType;
	m_timeLimit = *data++;
	Map* map = m_map;
	map->m_reserved = mapType;
	map->m_ground.Clear();
	if (m_levelVersion >= 4) {
		m_lemmingCount = data[0];
		m_flagCounts[0] = data[1];
	}
	else {
		m_lemmingCount = 4;
		m_flagCounts[0] = 1;
	}
	m_unk0xe4 = m_timeLimit;
}

// 68K 0x1060281a FixUpLevel__3CAIFv
// FUNCTION: LEMBALL 0x00412fb0
void Ai::FixUpLevel()
{
	Ai* self = this;
	int count = (unsigned int) g_wObjectCount;
	for (int i = 0; i < count; i++) {
		GameObject* object = g_pObjects[(unsigned short) i];
		if (object->GetId() == (short) 0xffff) {
			object->SetId(GameObject::NextId());
		}
	}
	self->m_objectManager->ConvertVer0ToVer1();
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
// FUNCTION: LEMBALL 0x004131e0
void Ai::SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth)
{
	m_networkTrapDoorCount = p_count;
	if (p_first == -1) {
		m_networkTrapDoors[0] = g_anDefaultTrapDoorLemmings[p_count - 1][0];
		m_networkTrapDoors[1] = g_anDefaultTrapDoorLemmings[p_count - 1][1];
		m_networkTrapDoors[2] = g_anDefaultTrapDoorLemmings[p_count - 1][2];
		m_networkTrapDoors[3] = g_anDefaultTrapDoorLemmings[p_count - 1][3];
	}
	else {
		m_networkTrapDoors[0] = p_first;
		m_networkTrapDoors[1] = p_second;
		m_networkTrapDoors[2] = p_third;
		m_networkTrapDoors[3] = p_fourth;
	}
	for (int i = 0; i < p_count; i++) {
		if (m_networkStartsX[i] > 1024 || m_networkStartsX[i] < 0) {
			m_networkStartsX[i] = i * 16;
		}
		if (m_networkStartsY[i] > 1024 || m_networkStartsY[i] < 0) {
			m_networkStartsY[i] = i * 16;
		}
	}
}

// 68K 0x10602b0a LoadFlagInfo__3CAIFPUci
// FUNCTION: LEMBALL 0x00413300
void Ai::LoadFlagInfo(unsigned char* p_data, int p_size)
{
	unsigned short* data = (unsigned short*) p_data;
	if (m_networkMode == 1) {
		if (m_isHost == 1) {
			m_flagCounts[0] = data[0];
			m_flagCounts[1] = data[1];
		}
		else {
			m_flagCounts[1] = data[0];
			m_flagCounts[0] = data[1];
		}
	}
	else {
		m_flagCounts[0] = data[0];
		m_flagCounts[1] = data[1];
	}
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
