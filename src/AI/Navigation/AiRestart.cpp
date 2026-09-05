#include "../../Control/Game/Demo.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameStatus.h"
#include "../../Control/Game/GameTime.h"
#include "../../Control/Level/LevelLoader.h"
#include "../../Map/Base/Map.h"
#include "../../Network/Game/PbNetworkGame.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsOStream.h"
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
#include "../Objects/BalloonPost.h"
#include "../Objects/GroundAnim.h"
#include "Ai.h"
#include "AiCursor.h"
#include "Maze.h"
#include "MoverManager.h"
#include "NodeManager.h"

// 68K 0x10600286 Restart__3CAIFv
// FUNCTION: LEMBALL 0x00410d00
void Ai::Restart()
{
	GameObject::Init(this);
	g_pActiveAI = this;
	m_objectCount = 0;
	for (int i = 0; i < m_objectCapacity; i++) {
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
	for (i = 0; i < 4; i++) {
		m_networkStartsZ[i] = 0;
		m_networkStartsY[i] = 0;
		m_networkStartsX[i] = 0;
		m_networkTrapDoors[i] = 0;
	}
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
		skill = (eSkill) packet[1];
		level = packet[0];
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
