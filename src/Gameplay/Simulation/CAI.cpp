#include "CAI.h"

#include "Application/GameMain.h"

#include "Application/CDemo.h"
#include "Application/CGame.h"
#include "Application/CGameStatus.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Level/CLevelLoader.h"
#include "Map/CMap.h"
#include "Multiplayer/CNetworkManager.h"
#include "Multiplayer/CPBNetworkGame.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Time/VsTime.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Geometry/Rect.h"
#include "Gameplay/Groups/CEnemyGroupManager.h"
#include "Gameplay/Groups/CFormationManager.h"
#include "Gameplay/Groups/CPlayerLemmingGroupManager.h"
#include "Gameplay/Groups/CSheepGroupManager.h"
#include "Gameplay/Projectiles/CBallManager.h"
#include "Gameplay/Projectiles/CBulletManager.h"
#include "Gameplay/Collectables/CCollectableManager.h"
#include "Gameplay/Mechanisms/CDoorManager.h"
#include "CGodManager.h"
#include "Gameplay/Hazards/CHandManager.h"
#include "Gameplay/Mechanisms/CIceManager.h"
#include "Gameplay/Mechanisms/CInvisibleSwitchManager.h"
#include "Gameplay/Hazards/CLaserManager.h"
#include "Gameplay/Mechanisms/CLiftManager.h"
#include "Gameplay/Hazards/CMineManager.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Hazards/CPaintGunManager.h"
#include "Gameplay/Hazards/CRocketManager.h"
#include "Gameplay/Hazards/CSlinkyManager.h"
#include "Gameplay/Mechanisms/CTrampolineManager.h"
#include "Gameplay/Mechanisms/CTrapDoorManager.h"
#include "Gameplay/Messages/CGameStateMessage.h"
#include "GameView/Animation/CAnimSpecial.h"
#include "Gameplay/Projectiles/CBall.h"
#include "Gameplay/Mechanisms/CBalloonPost.h"
#include "Gameplay/Animation/CGroundAnim.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Mechanisms/CTrapDoor.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Geometry/CRect3.h"
#include "Level/LevelVersions.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "Gameplay/Projectiles/CBullet.h"
#include "Gameplay/Mechanisms/SwitchEntry.h"
#include "Gameplay/Simulation/CAICursor.h"
#include "Gameplay/Navigation/CMaze.h"
#include "Gameplay/Mechanisms/CMoverManager.h"
#include "Gameplay/Navigation/CNodeManager.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"
#include "Engine/Queues/Message.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "Multiplayer/Transport/CReadSocket.h"

extern int g_anDefaultTrapDoorLemmings[4][4];

enum {
	OBJECT_REQUIREMENT_SLOT_COUNT = OBJECT_INVISIBLE_SWITCH + 1,
	OBJECT_REQUIREMENT_TRACKING_EXCLUSION_START = 0x211
};

enum {
	AI_NAVIGATION_NODE_CAPACITY = 300
};

enum {
	AI_GAME_OBJECT_CAPACITY = 100,
	AI_DEFAULT_LEVEL_TIME_LIMIT_SECONDS = 180,
	AI_LEVEL_TIME_COUNTDOWN_THRESHOLD_SECONDS = 600,
	AI_GAME_TICKS_PER_SECOND = 1000 / GAME_TICK_MILLISECONDS,
	AI_GAME_OVER_SUCCESS_DELAY_TICKS = 60,
	AI_NETWORK_SEND_INTERVAL_MILLISECONDS = 66,
	AI_LEVEL_TIME_EXPIRED_ADJUSTED_VALUE = -1,
	AI_DEMO_RANDOM_SEED = 0xad28,
	NETWORK_START_TRAP_DOOR_DEFAULT = -1
};

#define NETWORK_START_POSITION_COUNT 4
#define NETWORK_LEMMINGS_PER_TEAM 4

enum {
	CAI_NETWORK_STATE_BUFFER_RESERVE_BYTES = 0x60
};

enum {
	AI_EMPTY_COLLISION_RECT_MAX_COORDINATE = -1
};

// GLOBAL: LEMBALL 0x004a782c
CAI* g_pGenericGroupAI;

// GLOBAL: LEMBALL 0x004a74b0
CAI* g_pAI;

// GLOBAL: LEMBALL 0x004a74b8
int g_nGameOver = 0;

// GLOBAL: LEMBALL 0x0049cf34
CAI* g_pActiveAI = NULL;

// GLOBAL: LEMBALL 0x0049cf60
int g_anDefaultTrapDoorLemmings[4][4] = {{4, 0, 0, 0}, {3, 1, 0, 0}, {2, 1, 1, 0}, {1, 1, 1, 1}};

// FUNCTION: LEMBALL 0x00410c10
CAI::CAI(CGame* p_game)
{
	m_collisionPoint.m_x = 0;
	m_collisionPoint.m_y = 0;
	m_collisionPoint.m_z = 0;
	m_collisionRect.m_x1 = 0;
	m_collisionRect.m_y1 = 0;
	m_collisionRect.m_z1 = 0;
	m_collisionRect.m_x2 = AI_EMPTY_COLLISION_RECT_MAX_COORDINATE;
	m_collisionRect.m_y2 = AI_EMPTY_COLLISION_RECT_MAX_COORDINATE;
	m_collisionRect.m_z2 = AI_EMPTY_COLLISION_RECT_MAX_COORDINATE;
	m_objectCount = 0;
	m_objectCapacity = AI_GAME_OBJECT_CAPACITY;
	m_objects = new CGameObject*[AI_GAME_OBJECT_CAPACITY];
	CGameObject** objects = m_objects;
	for (int i = 0; i < m_objectCapacity; i++) {
		objects[i] = NULL;
	}
	m_game = p_game;
	m_initialised = 0;
	g_wObjectCount = 0;
	Restart();
}

// FUNCTION: LEMBALL 0x00410d00
void CAI::Restart()
{
	int i;
	unsigned int network;
	CMap* map;
	int level;
	eSkill skill;
	CGameObject::Init(this);
	g_pActiveAI = this;
	m_objectCount = 0;
	for (i = 0; i < m_objectCapacity; i++) {
		m_objects[i] = NULL;
	}
	g_wNetworkLemmingIndex = 0;
	g_wLocalLemmingIndex = 0;
	g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_UNSET;
	m_isSinglePlayer = 0;
	ResetGameTimes();
	m_clockSourceReady = 0;
	network = g_pGameStatus->m_skill == SKILL_NETWORK;
	m_started = 0;
	m_gameStatePending = 0;
	m_isHost = 0;
	m_gameplayStartDelay = 0;
	m_gameplayEnabled = 0;
	m_networkStartReady = 1;
	m_payloadCapacity += CAI_NETWORK_STATE_BUFFER_RESERVE_BYTES;
	m_networkMode = network;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		if (m_initialised == 0) {
			m_networkGame = new CPBNetworkGame(this);
		}
		m_isHost = g_pActiveConnection->m_isHost;
		if (m_initialised == 0) {
			m_gameStateMessage = new CGameStateMessage;
		}
		g_pGameStatus->m_levelState = 0;
	}
	for (i = 0; i < NETWORK_START_POSITION_COUNT; i++) {
		m_networkStartsZ[i] = 0;
		m_networkStartsY[i] = 0;
		m_networkStartsX[i] = 0;
		m_networkTrapDoors[i] = 0;
	}
	m_networkTrapDoors[0] = 4;
	m_networkTrapDoorCount = 1;
	m_levelVersion = LEVEL_VERSION_UNVERSIONED;
	m_unk0xd4 = 4;
	m_flagCounts[1] = 1;
	m_gameTime = 0;
	m_lemmingCount = 4;
	m_flagCounts[0] = 1;
	m_score = g_pGameStatus->m_levelState;
	m_levelStartScore = m_score;
	m_paused = 0;
	g_wLemmingCount = 0;
	m_clockStartPending = 1;
	m_mapType = 0;
	m_levelTimeRemaining = 0;
	m_gameStatus = GAME_STATUS_NOT_STARTED;
	m_processState = PROCESS_RESULT_CONTINUE;
	m_timeLimit = AI_DEFAULT_LEVEL_TIME_LIMIT_SECONDS;
	if (m_initialised == 0) {
		m_map = new CMap;
	}
	m_map->Restart();
	g_pMap = m_map;
	map = m_map;
	map->m_ai = this;
	map->m_ownerAI = this;
	if (m_initialised == 0) {
		m_maze = new CMaze(m_map);
	}
	g_pMaze = m_maze;
	if (m_initialised == 0) {
		m_aiQueue = new CBaseQueue(10, "AIQueue");
		m_aiQueue->Attach(this, 0);
	}
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && g_pActiveConnection != NULL) {
		g_pActiveConnection->CReadSocket::UnUseAllNC();
		g_pActiveConnection->CReadSocket::UnUseAllC();
	}
	if (m_initialised == 0) {
		g_pGodManager = new CGodManager(20);
	}
	else {
		g_pGodManager->Restart();
	}
	if (m_initialised == 0) {
		m_liftManager = new CLiftManager(this, 60);
		g_pGodManager->Register(m_liftManager);
	}
	if (m_initialised == 0) {
		m_moverManager = new CMoverManager(this, 40);
		g_pGodManager->Register(m_moverManager);
	}
	if (m_initialised == 0) {
		m_objectManager = new CObjectManager(this, 60);
		g_pGodManager->Register(m_objectManager);
	}
	if (m_initialised == 0) {
		m_formationManager = new CFormationManager();
	}
	m_formationManager->Restart();
	if (m_initialised == 0) {
		m_playerGroupManager = new CPlayerLemmingGroupManager(this, m_objectManager, m_formationManager);
		g_pGodManager->Register(m_playerGroupManager);
	}
	if (m_initialised == 0) {
		m_bulletManager = new CBulletManager();
		g_pGodManager->Register(m_bulletManager);
	}
	if (m_initialised == 0) {
		m_cursor = new CAICursor(this, MAP_COORDINATE_MAX, MAP_COORDINATE_MAX);
	}
	if (m_initialised == 0) {
		m_sheepGroupManager = new CSheepGroupManager(this, m_objectManager, m_formationManager);
	}
	m_sheepGroupManager->Restart();
	if (m_initialised == 0) {
		m_enemyGroupManager = new CEnemyGroupManager(this, m_objectManager, m_formationManager);
	}
	m_enemyGroupManager->Restart();
	if (m_initialised == 0) {
		m_nodeManager = new CNodeManager(AI_NAVIGATION_NODE_CAPACITY);
	}
	m_nodeManager->Restart();
	if (m_initialised == 0) {
		m_ballManager = new CBallManager(this, 20);
	}
	m_ballManager->Restart();
	if (m_initialised == 0) {
		m_collectableManager = new CCollectableManager(this, 30);
		g_pGodManager->Register(m_collectableManager);
	}
	if (m_initialised == 0) {
		m_mineManager = new CMineManager(this, 40);
		g_pGodManager->Register(m_mineManager);
	}
	if (m_initialised == 0) {
		m_doorManager = new CDoorManager(this, 20);
		g_pGodManager->Register(m_doorManager);
	}
	if (m_initialised == 0) {
		m_rocketManager = new CRocketManager(this, 20);
		g_pGodManager->Register(m_rocketManager);
	}
	if (m_initialised == 0) {
		m_handManager = new CHandManager(this, 20);
		g_pGodManager->Register(m_handManager);
	}
	if (m_initialised == 0) {
		m_laserManager = new CLaserManager(this, 20);
		g_pGodManager->Register(m_laserManager);
	}
	if (m_initialised == 0) {
		m_groundAnim = new CGroundAnim();
	}
	m_groundAnim->Restart();
	if (m_initialised == 0) {
		m_balloonPost = new CBalloonPost(this, m_map);
	}
	m_balloonPost->Restart();
	if (m_initialised == 0) {
		m_trampolineManager = new CTrampolineManager(this, 20);
		g_pGodManager->Register(m_trampolineManager);
	}
	if (m_initialised == 0) {
		m_paintGunManager = new CPaintGunManager(this, 20);
		g_pGodManager->Register(m_paintGunManager);
	}
	if (m_initialised == 0) {
		m_iceManager = new CIceManager(this, 100);
		g_pGodManager->Register(m_iceManager);
	}
	if (m_initialised == 0) {
		m_trapDoorManager = new CTrapDoorManager();
		g_pGodManager->Register(m_trapDoorManager);
	}
	if (m_initialised == 0) {
		m_slinkyManager = new CSlinkyManager(this, 20);
	}
	m_slinkyManager->Restart();
	if (m_initialised == 0) {
		m_invisibleSwitchManager = new CInvisibleSwitchManager(this, 40);
		g_pGodManager->Register(m_invisibleSwitchManager);
	}
	if (m_initialised == 0) {
		m_levelLoader = new CLevelLoader(this);
	}

	if (g_nDemoMode != 0) {
		unsigned char packet[2];
		unsigned long packetSize;
		g_pDemo->SetDemoMode(1);
		g_pDemo->GetUserPacket(packet, packetSize);
		level = packet[0];
		skill = (eSkill) packet[1];
		*g_pSysOutput << "Starting demo mode for level " << level << " on skill " << (int) skill << "\r\n";
		*g_pRandomSeed = AI_DEMO_RANDOM_SEED;
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
		m_animSpecial = new CAnimSpecial;
	}
	m_animSpecial->Initialise(m_map);
	if (m_levelVersion == LEVEL_VERSION_UNVERSIONED) {
		FixUpLevel();
	}
	m_levelVersion = LEVEL_VERSION_CURRENT;
	if (m_initialised == 0) {
		m_objectRequired = new unsigned int[OBJECT_REQUIREMENT_SLOT_COUNT];
	}
	for (i = 0; i < OBJECT_REQUIREMENT_SLOT_COUNT; i++) {
		m_objectRequired[i] = 0;
	}
	DecideAnimsRequired();
	m_initialised = 1;
}

// FUNCTION: LEMBALL 0x004117a0
CAI::~CAI()
{
	if (g_nDemoMode != 0) {
		g_pDemo->SetDemoMode(0);
	}
	CGameObject::Init(NULL);
	g_pActiveAI = NULL;
	m_aiQueue->Detach(this, 0);
	delete m_cursor;
	delete m_playerGroupManager;
	delete m_bulletManager;
	delete m_objectManager;
	delete m_formationManager;
	delete m_sheepGroupManager;
	delete m_enemyGroupManager;
	delete m_maze;
	delete m_map;
	delete m_nodeManager;
	delete m_ballManager;
	delete m_collectableManager;
	delete m_mineManager;
	delete m_liftManager;
	delete m_doorManager;
	delete m_rocketManager;
	delete m_laserManager;
	delete m_handManager;
	delete m_groundAnim;
	delete m_balloonPost;
	delete m_trampolineManager;
	delete m_paintGunManager;
	delete m_iceManager;
	delete m_moverManager;
	delete m_slinkyManager;
	delete m_trapDoorManager;
	delete m_invisibleSwitchManager;
	delete m_levelLoader;
	delete m_animSpecial;
	delete g_pGodManager;
	delete m_aiQueue;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		delete m_networkGame;
		delete m_gameStateMessage;
	}
	operator delete(m_objectRequired);
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		g_pGameStatus->m_levelState = m_score;
	}
	operator delete(m_objects);
}

// FUNCTION: LEMBALL 0x00411b10
void CAI::Start()
{
	CDemo* demo;
	CNetworkManager* networkManager;

	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		m_gameStatePending = 1;
		m_networkStartReady = 0;
		networkManager = g_pNetworkManager;
		networkManager->m_desiredGameState = 3;
		networkManager->m_observedGameState = 0;
		return;
	}

	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		demo = g_pDemo;
		demo->m_startTime = CurrentMilliTimer();
		demo->m_duration = 0;
	}

	GameState(GAME_STATUS_RUNNING);
	m_started = 1;
}

// FUNCTION: LEMBALL 0x00411b70
void CAI::SendGameState(eGameStates p_state, eGameStateStages p_stage)
{
	if (g_pActiveConnection != NULL) {
		if (m_gameStateMessage->m_pendingSendCount != 0) {
			unsigned long start = CurrentMilliTimer();
			while (m_gameStateMessage->m_pendingSendCount != 0 &&
				   CurrentMilliTimer() - start < NETWORK_PENDING_SEND_TIMEOUT_MS) {
				g_pBaseNetwork->WaitProcess();
			}
		}
		if (m_gameStateMessage->m_pendingSendCount == 0) {
			CGameStateMessage& message = *m_gameStateMessage;
			m_gameStatePending = 1;
			message.m_state = p_state;
			message.m_stage = p_stage;
			m_gameStateMessage->m_levelTime = m_gameTime;
			m_gameStateMessage->m_score = m_score;
			m_gameStateMessage->Send(g_pActiveConnection);
		}
	}
}

// FUNCTION: LEMBALL 0x00411c10
void CAI::RemoteGameState(CGameStateMessage* p_message)
{
	eGameStates state;
	int apply;
	eGameStateStages stage;

	const CGameStateMessage& message = *p_message;
	apply = 0;
	state = message.m_state;
	stage = message.m_stage;
	*g_pSysOutput << "Received Game State " << (int) state << ", stage " << (int) stage << "\r\n";
	switch (stage) {
	case GAME_STATE_STAGE_REQUEST:
		if (m_gameStatePending != 0) {
			if (m_isHost != 0) {
				SendGameState(state, GAME_STATE_STAGE_REJECT);
				return;
			}
			m_gameStatePending = 0;
		}
		switch (state) {
		case GAME_STATE_PAUSED:
			if (m_gameStatus == GAME_STATUS_PAUSED) {
				SendGameState(state, GAME_STATE_STAGE_REJECT);
				m_gameStatePending = 0;
				return;
			}
			apply = 1;
			m_isSinglePlayer = 1;
			break;
		case GAME_STATE_SUCCESS:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_FLAGS_COLLECTED;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_COMPLETING:
			if (m_gameStatus == GAME_STATUS_COMPLETING || m_gameStatus == GAME_STATUS_SUCCESS) {
				SendGameState(state, GAME_STATE_STAGE_REJECT);
				m_gameStatePending = 0;
				return;
			}
			m_gameStatus = GAME_STATUS_GAME_OVER;
			g_nGameOver = 1;
			break;
		case GAME_STATE_FAILURE:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_LEMMING_ELIMINATION;
			m_gameStatus = GAME_STATUS_SUCCESS;
			break;
		case GAME_STATE_QUIT:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_GAVE_UP;
			m_gameStatus = GAME_STATUS_SUCCESS;
			break;
		default:
			apply = 1;
			break;
		}
		SendGameState(state, GAME_STATE_STAGE_CONFIRM);
		m_gameStatePending = 0;
		if (apply == 0) {
			return;
		}
	case GAME_STATE_STAGE_CONFIRM:
		switch (state) {
		case GAME_STATE_PAUSED:
			m_gameStatus = GAME_STATUS_PAUSED;
			break;
		case GAME_STATE_RUNNING:
			if (m_gameStatus != GAME_STATUS_RESTART) {
				m_started = 1;
				m_gameStatus = GAME_STATUS_RUNNING;
			}
			break;
		case GAME_STATE_SUCCESS:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_FLAGS_COLLECTED;
			m_gameStatus = GAME_STATUS_SUCCESS;
			break;
		case GAME_STATE_COMPLETING:
			m_gameStatus = GAME_STATUS_COMPLETING;
			break;
		case GAME_STATE_FAILURE:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_LEMMING_ELIMINATION;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_QUIT:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_GAVE_UP;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_TIME_EXPIRED:
			if ((unsigned int) m_gameTime > message.m_levelTime) {
				g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_TIME_EXPIRED;
				m_gameStatus = GAME_STATUS_SUCCESS;
			}
			else if ((unsigned int) m_gameTime != message.m_levelTime) {
				g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_TIME_EXPIRED;
				m_gameStatus = GAME_STATUS_FAILURE;
			}
			else {
				unsigned int score = m_score;
				if (score > message.m_score) {
					g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_BEST_SCORE;
					m_gameStatus = GAME_STATUS_SUCCESS;
				}
				else if (score < message.m_score) {
					g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_BEST_SCORE;
					m_gameStatus = GAME_STATUS_FAILURE;
				}
				else {
					g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_TIME_EXPIRED;
					m_gameStatus = GAME_STATUS_FAILURE;
				}
			}
			break;
		case GAME_STATE_RESTART:
			m_gameStatus = GAME_STATUS_RESTART;
			return;
		}
		m_gameStatePending = 0;
		break;
	case GAME_STATE_STAGE_REJECT:
		m_gameStatePending = 0;
		return;
	}
}

// FUNCTION: LEMBALL 0x00411f20
void CAI::GameState(eGameStatus p_status)
{
	if (m_networkMode == NETWORK_MODE_SINGLE_PLAYER) {
		switch (p_status) {
		case GAME_STATUS_SUCCESS:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_FLAGS_COLLECTED;
			m_gameStatus = GAME_STATUS_SUCCESS;
			return;
		case GAME_STATUS_FAILURE:
			if (g_pGameStatus->m_skillState == GAME_RESULT_MESSAGE_UNSET) {
				g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_LEMMING_ELIMINATION;
			}
			m_gameStatus = GAME_STATUS_FAILURE;
			return;
		case GAME_STATUS_TIME_EXPIRED:
			g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_TIME_EXPIRED;
			m_gameStatus = GAME_STATUS_FAILURE;
			return;
		default:
			m_gameStatus = p_status;
			return;
		}
	}
	if (m_gameStatePending == 0) {
		switch (p_status) {
		case GAME_STATUS_PAUSED:
			m_isSinglePlayer = 0;
			SendGameState(GAME_STATE_PAUSED, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_RUNNING:
			SendGameState(GAME_STATE_RUNNING, GAME_STATE_STAGE_CONFIRM);
			if (m_gameStatus == GAME_STATUS_RUNNING) {
				m_gameStatePending = 0;
				return;
			}
			break;
		case GAME_STATUS_SUCCESS:
			SendGameState(GAME_STATE_SUCCESS, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_COMPLETING:
			SendGameState(GAME_STATE_COMPLETING, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_FAILURE:
			if (g_pGameStatus->m_skillState == GAME_RESULT_MESSAGE_GAVE_UP) {
				SendGameState(GAME_STATE_QUIT, GAME_STATE_STAGE_REQUEST);
				return;
			}
			SendGameState(GAME_STATE_FAILURE, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_TIME_EXPIRED:
			SendGameState(GAME_STATE_TIME_EXPIRED, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_RESTART:
			SendGameState(GAME_STATE_RESTART, GAME_STATE_STAGE_REQUEST);
		}
	}
}

// FUNCTION: LEMBALL 0x00412080
void CAI::SetPlayerIDs()
{
	if (g_pActiveConnection != NULL) {
		int offsets[2] = {0, 0};
		if (g_pActiveConnection->m_isHost != 0) {
			offsets[0] = 4;
		}
		else {
			offsets[1] = 4;
		}

		int* offset = offsets;
		do {
			int count;
			CPlayerLemming** lemming = m_networkLemmings + *offset;
			count = 4;
			do {
				(*lemming)->SetId(CGameObject::NextLoadingId());
				lemming++;
				count--;
			} while (count != 0);
			offset++;
		} while (offset < offsets + 2);
	}
}

// FUNCTION: LEMBALL 0x00412100
void CAI::DecideAnimsRequired()
{
	int count = g_wObjectCount;
	int i = 0;
	for (;;) {
		if (i >= count) {
			break;
		}
		CGameObject* object = g_pObjects[(unsigned short) i];
		if (object != NULL && object->m_objectType != OBJECT_INVALID) {
			SetObjectRequired(object->m_objectType, 1);
		}
		i++;
	}
	if (m_ballManager->m_activeCount == 0) {
		SetObjectRequired(OBJECT_BALL, 0);
	}
	if (m_doorManager->m_count == 0) {
		SetObjectRequired(OBJECT_DOOR_2, 0);
		SetObjectRequired(OBJECT_DOOR_1, 0);
	}
	if (m_laserManager->m_count == 0) {
		SetObjectRequired(OBJECT_LASER_VERTICAL, 0);
		SetObjectRequired(OBJECT_LASER_HORIZONTAL, 0);
	}
	if (m_mineManager->m_count == 0) {
		SetObjectRequired(OBJECT_MINE, 0);
	}
	if (m_rocketManager->m_count == 0) {
		SetObjectRequired(OBJECT_ROCKET, 0);
	}
	if (m_slinkyManager->m_count == 0) {
		SetObjectRequired(OBJECT_SLINKY, 0);
	}
}

// FUNCTION: LEMBALL 0x004121e0
void CAI::AddTime(int p_time)
{
	m_gameTime += p_time;
}

// FUNCTION: LEMBALL 0x004121f0
void CAI::Process(int p_paused)
{
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && g_pActiveConnection == NULL) {
		return;
	}
	m_aiQueue->ProcessNMsgs(m_aiQueue->GetMessageCount());
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && m_networkStartReady == 0 &&
		g_pNetworkManager->m_desiredGameState == g_pNetworkManager->m_observedGameState) {
		m_gameStatePending = 0;
		m_networkStartReady = 1;
		GameState(GAME_STATUS_RUNNING);
	}
	if (p_paused == 0 && m_paused != 0) {
		SetGameTime();
		return;
	}
	switch (m_gameStatus) {
	case GAME_STATUS_NOT_STARTED:
	case GAME_STATUS_RUNNING:
	case GAME_STATUS_COMPLETING:
	case GAME_STATUS_GAME_OVER:
		break;
	default:
		SetGameTime();
		return;
	}
	if (m_started == 0 || m_gameStatePending != 0) {
		return;
	}
	SetGameTime();
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		if (g_pActiveConnection != NULL && g_pActiveConnection->IsChanged(*m_networkGame)) {
			g_pActiveConnection->GetLatest(*m_networkGame);
			m_clockSourceReady = 1;
		}
	}
	unsigned int time;
	if (m_networkMode != NETWORK_MODE_SINGLE_PLAYER && g_pActiveConnection != NULL &&
		g_pActiveConnection->m_isHost != 0) {
		time = g_dwRemoteGameTick;
	}
	else {
		time = g_dwGameTick;
		m_clockSourceReady = 1;
	}
	if (m_clockStartPending != 0) {
		if (m_clockSourceReady != 0) {
			m_levelStartTick = time;
			m_clockStartPending = 0;
			m_levelTimeRemaining = m_timeLimit;
		}
	}
	else {
		if (g_nGameOver == 0 && m_levelTimeRemaining < AI_LEVEL_TIME_COUNTDOWN_THRESHOLD_SECONDS) {
			m_levelTimeRemaining = m_timeLimit - (time - m_levelStartTick) / AI_GAME_TICKS_PER_SECOND;
		}
		int remaining = m_levelTimeRemaining;
		remaining += m_gameTime;
		if (remaining < 0) {
			GameState(GAME_STATUS_TIME_EXPIRED);
			m_levelTimeRemaining = AI_LEVEL_TIME_EXPIRED_ADJUSTED_VALUE - m_gameTime;
		}
		if (m_gameplayEnabled == 0 && m_gameplayStartDelay < time - m_levelStartTick) {
			m_gameplayEnabled = 1;
		}
	}
	m_enemyGroupManager->Process();
	m_sheepGroupManager->Process();
	m_ballManager->Process();
	m_balloonPost->Process();
	m_slinkyManager->Process();
	m_groundAnim->Process();
	g_pGodManager->Process();
	if (m_flagCounts[0] <= 0) {
		if (g_nGameOver == 0) {
			GameState(GAME_STATUS_COMPLETING);
			g_nGameOver = 1;
			m_gameOverDeadline = g_dwGameTick + AI_GAME_OVER_SUCCESS_DELAY_TICKS;
		}
		if (m_gameStatus == GAME_STATUS_COMPLETING && m_gameOverDeadline < g_dwGameTick) {
			GameState(GAME_STATUS_SUCCESS);
		}
	}
	if (g_pActiveConnection != NULL && (LemmingsSFXChanged() || g_dwSimulationTimestamp - m_lastNetworkSendCheckTick >
																	AI_NETWORK_SEND_INTERVAL_MILLISECONDS)) {
		CConnect* connection = g_pActiveConnection;
		if (m_networkGame->m_pendingSendCount == 0) {
			m_networkGame->Send(connection);
		}
		m_lastNetworkSendCheckTick = g_dwSimulationTimestamp;
	}
}

// FUNCTION: LEMBALL 0x004124d0
int CAI::GetData(CViewData* p_viewData)
{
	int count = g_pGodManager->GetViewData(p_viewData);
	count += m_enemyGroupManager->GetViewData(p_viewData + count);
	count += m_sheepGroupManager->GetViewData(p_viewData + count);
	int i = 0;
	CBallManager* balls = m_ballManager;
	if (balls->m_activeCount > 0) {
		CViewData* data = p_viewData + count;
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

// FUNCTION: LEMBALL 0x004125c0
int CAI::HitTrampoline(const AICOORD& p_position, CGameObject* p_object)
{
	return m_trampolineManager->Hit(p_position, p_object);
}

// FUNCTION: LEMBALL 0x004125e0
bool CAI::IsLemmingPlayerControlled(CPlayerLemming* p_lemming)
{
	return m_playerGroupManager->IsLemmingPlayerControlled(p_lemming);
}

// FUNCTION: LEMBALL 0x00412600
void CAI::FireBullet(unsigned short p_id,
					 eBulletType p_bulletType,
					 eOwner p_owner,
					 int p_parameter,
					 AICOORD p_start,
					 AICOORD p_target)
{
	m_bulletManager->RequestBullet(p_id, p_bulletType, p_owner, p_parameter, p_start, p_target);
}

// FUNCTION: LEMBALL 0x00412660
int CAI::ProcessMsg(Message* p_message)
{
	unsigned int messageType = p_message->m_type;
	if (messageType != AI_MESSAGE_REQUEST_FIRE) {
		if (m_gameplayEnabled == 0) {
			return 1;
		}
		switch (messageType) {
		case AI_MESSAGE_MOVE_GROUP:
			m_playerGroupManager->AddNewWaypointToCurrentGroup(p_message->m_code, (int) p_message->m_payload);
			return 0;
		case AI_MESSAGE_CANCEL_MOVES:
			m_playerGroupManager->RemoveWaypointsFromCurrentGroup();
			return 0;
		case AI_MESSAGE_FORM_GROUP:
			m_playerGroupManager->CreateNewGroup((unsigned short) p_message->m_code,
												 (unsigned short*) p_message->m_payload);
			return 0;
		case AI_MESSAGE_PREVIOUS_GROUP:
			m_playerGroupManager->MakePreviousGroupPlayerControlled();
			return 0;
		case AI_MESSAGE_NEXT_GROUP:
			m_playerGroupManager->MakeNextGroupPlayerControlled();
			return 0;
		case AI_MESSAGE_USE_OBJECT:
			m_playerGroupManager->UseObject(p_message->m_code);
			return 0;
		default:
			m_processedCount = m_processedCount + 1;
			return 0;
		}
	}
	m_playerGroupManager->PlayerGroupRequestFire(p_message->m_code, (int) p_message->m_payload);
	return 0;
}

// FUNCTION: LEMBALL 0x00412740
void CAI::CollectNetworkGroupData(int* p_output)
{
	*p_output = 0;
	int sheepCount = m_sheepGroupManager->GetAllBoundingBoxes(reinterpret_cast<Rect*>(p_output + 1));
	*p_output = sheepCount;
	int playerCount = m_playerGroupManager->GetAllBoundingBoxes(
		reinterpret_cast<Rect*>(p_output + sheepCount * (sizeof(Rect) / sizeof(int)) + 1));
	*p_output = sheepCount + playerCount;
}

// FUNCTION: LEMBALL 0x00412780
bool CAI::PlayerCheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	return m_playerGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127a0
bool CAI::EnemyCheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	return m_enemyGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127c0
bool CAI::SheepCheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	return m_sheepGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127e0
bool CAI::BulletCheckGroupIntersection(CVSRect* p_rect, AICOORD* p_coordinate)
{
	return m_bulletManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x00412800
CGlobalGameObject* CAI::FindNearbyObject(AICOORD p_position)
{
	return m_objectManager->FindNearbyObject(p_position);
}

// FUNCTION: LEMBALL 0x00412830
CGlobalGameObject* CAI::FindNearbyObject(AICOORD p_position, eObjectType p_objectType)
{
	return m_objectManager->FindNearbyObject(p_position, p_objectType);
}

// FUNCTION: LEMBALL 0x00412870
CGlobalGameObject* CAI::FindObjectInBounds(CVSRect* p_bounds, eObjectType p_objectType)
{
	return m_objectManager->FindObjectInBounds(p_bounds, p_objectType);
}

// FUNCTION: LEMBALL 0x00412890
void CAI::StepOn(const AICOORD& p_position, CGameObject* p_object, unsigned short p_collisionFlags)
{
	int y;
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX = x / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = y / GROUND_BLOCK_PIXEL_SIZE;

	if (p_object->m_onMover != 0) {
		return;
	}

	unsigned short groundZ;
	{
		CMap* map = m_map;
		int groundX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int groundY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		if (x < 0 || y < 0 || groundX >= map->m_ground.m_width || groundY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			int cellX = x & GROUND_BLOCK_PIXEL_MASK;
			int cellY = y & GROUND_BLOCK_PIXEL_MASK;
			groundZ = map->m_ground.m_ground[groundY * map->m_ground.m_width + groundX].GetZ(cellX, cellY);
		}
	}

	int groundThreshold = (int) groundZ + 4;
	if (groundThreshold < (p_object->m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS)) {
		return;
	}

	unsigned short collision;
	if (blockX < 0 || blockY < 0) {
		collision = GROUND_COLLISION_OUT_OF_BOUNDS;
	}
	else {
		CMap* map = m_map;
		if (blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
			collision = GROUND_COLLISION_OUT_OF_BOUNDS;
		}
		else {
			collision = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].m_collision;
		}
	}

	if (p_object->m_balloonPostId != 0) {
		return;
	}

	if ((collision & GROUND_COLLISION_HAZARD) != 0 && (p_collisionFlags & GAME_OBJECT_COLLISION_TRIGGER_HAZARDS) != 0) {
		eObjectType objectType = m_map->m_ground.m_ground[blockY * m_map->m_ground.m_width + blockX].m_objectType;
		p_object->m_actionDeadline = g_dwGameTick + HAZARD_DEATH_DELAY_TICKS;
		p_object->m_action = ACTION_EXTERNAL_CONTROL;
		switch (objectType) {
		default:
			p_object->m_actionArgument = EXTERNAL_CONTROL_ON_FIRE;
			p_object->m_stateTimer = g_dwGameTick * GAME_TICK_MILLISECONDS;
			p_object->SetSndEffect(SFX_AAAAH1);
			return;

		case TERRAIN_ELECTRIC:
			p_object->m_actionArgument = EXTERNAL_CONTROL_ELECTROCUTED;
			p_object->m_stateTimer = g_dwGameTick * GAME_TICK_MILLISECONDS;
			p_object->SetSndEffect(SFX_AAAAH2);
			return;
		}
	}

	if ((collision & GROUND_COLLISION_OBJECT_INTERACTION) == 0) {
		return;
	}

	if ((p_collisionFlags & GAME_OBJECT_COLLISION_STEP_ON_MINE) != 0) {
		m_mineManager->StepOn(p_position, p_object);
	}
	if ((p_collisionFlags & GAME_OBJECT_COLLISION_STEP_ON_SPECIAL_OBJECTS) != 0) {
		m_liftManager->StepOn(p_position, p_object);
		m_rocketManager->StepOn(p_position, p_object);
		m_handManager->StepOn(p_position, p_object);
		m_laserManager->StepOn(p_position, p_object);
		m_iceManager->StepOn(p_position, p_object);
	}
	if ((p_collisionFlags & GAME_OBJECT_COLLISION_STEP_ON_INVISIBLE_SWITCHES) != 0) {
		m_invisibleSwitchManager->StepOn(p_position, p_object);
	}
}

// FUNCTION: LEMBALL 0x00412ad0
bool CAI::OpenDoor(const AICOORD& p_position, CGameObject* p_object, unsigned short p_collisionFlags)
{
	int blockX = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	unsigned short collision;
	if (blockX < 0 || blockY < 0) {
		collision = GROUND_COLLISION_OUT_OF_BOUNDS;
	}
	else {
		CMap* map = m_map;
		int width = map->m_ground.m_width;
		if (width <= blockX || map->m_ground.m_height <= blockY) {
			collision = GROUND_COLLISION_OUT_OF_BOUNDS;
		}
		else {
			collision = map->m_ground.m_ground[blockY * width + blockX].m_collision;
		}
	}
	if ((collision & GROUND_COLLISION_OBJECT_INTERACTION) != 0 &&
		(p_collisionFlags & GAME_OBJECT_COLLISION_OPEN_DOORS) != 0) {
		return m_doorManager->Open(p_position, p_object);
	}
	return false;
}

// FUNCTION: LEMBALL 0x00412b60
CPt3 CAI::GetNodePosition(int p_node)
{
	return m_nodeManager->GetNodePosition(p_node);
}

// FUNCTION: LEMBALL 0x00412b80
void CAI::AddData()
{
	int remaining = NETWORK_LEMMINGS_PER_TEAM;
	CPlayerLemming** lemming = m_networkLemmings;
	do {
		CNetworkMessage* stream = this;
		CPlayerLemming& message = **lemming;
		message.CopyDataStream(stream->m_writeCursor, 0);
		stream->m_writeCursor += message.m_writeCursor - message.m_buffer;
		lemming++;
		remaining--;
	} while (remaining != 0);
}

// FUNCTION: LEMBALL 0x00412be0
void CAI::GetData()
{
	CPlayerLemming** lemming = &m_networkLemmings[NETWORK_LEMMINGS_PER_TEAM];
	for (int i = 0; i < NETWORK_LEMMINGS_PER_TEAM; i++) {
		CNetworkMessage* stream = this;
		CPlayerLemming& message = **lemming;
		if (message.Set(stream->m_readCursor)) {
			stream->m_readCursor = message.m_readCursor;
		}
		lemming++;
	}
}

// FUNCTION: LEMBALL 0x00412c40
bool CAI::CheckNetworkStateChanged()
{
	return m_playerGroupManager->CheckNetworkStateChanged();
}

// FUNCTION: LEMBALL 0x00412c50
bool CAI::LemmingsSFXChanged()
{
	return m_playerGroupManager->HasSFXChanged();
}

// FUNCTION: LEMBALL 0x00412c60
void CAI::QuitGame()
{
	m_paused = 0;
	g_pGameStatus->m_skillState = GAME_RESULT_MESSAGE_GAVE_UP;
	GameState(GAME_STATUS_FAILURE);
}

// FUNCTION: LEMBALL 0x00412c80
void CAI::SwitchMessage(swMessage p_message, int p_first, int p_last, int p_arg3)
{
	switch (p_message) {
	case SW_LIFT:
		m_liftManager->Switch(p_message, p_first, p_last, p_arg3);
		return;
	case SW_LIFTS: {
		int index = p_first;
		if (index < p_last) {
			do {
				m_liftManager->Switch(SW_LIFT, index, 0, 0);
				index++;
			} while (index < p_last);
			return;
		}
		break;
	}
	case SW_DOOR:
		m_doorManager->Switch(SW_DOOR, p_first);
		return;
	case SW_MOVER:
		m_moverManager->Switch(SW_MOVER, p_first);
		return;
	case SW_ICE:
		m_iceManager->Switch(SW_ICE, p_first);
	}
}

// FUNCTION: LEMBALL 0x00412d50
void CAI::GetPlayerStartCoordinates(int& p_x, int& p_y, int& p_z)
{
	CPlayerLemmingGroupManager* manager = m_playerGroupManager;
	p_x = manager->m_startX[0];
	p_y = manager->m_startY[0];
	p_z = manager->m_startZ[0];
}

// FUNCTION: LEMBALL 0x00412d80
void CAI::GetPlayerStartCoordinates(int& p_x, int& p_y, int& p_z, int p_index)
{
	CPlayerLemmingGroupManager* manager = m_playerGroupManager;
	p_x = manager->m_startX[p_index];
	p_y = manager->m_startY[p_index];
	p_z = manager->m_startZ[p_index];
}

// FUNCTION: LEMBALL 0x00412dc0
void CAI::GetPlayerPos(int p_id, AICOORD& p_position)
{
	CPlayerLemming** lemming = m_networkLemmings;
	int index = 0;
	do {
		if ((*lemming)->m_objectId == p_id) {
			CPlayerLemming* player = m_networkLemmings[index];
			p_position.m_xFixed = player->m_position.m_xFixed;
			p_position.m_yFixed = player->m_position.m_yFixed;
			p_position.m_zFixed = player->m_position.m_zFixed;
			return;
		}
		lemming++;
		index++;
	} while (index < NETWORK_LEMMINGS_PER_TEAM);
}

// FUNCTION: LEMBALL 0x00412e20
bool CAI::GetOrigin(AICOORD& p_origin, unsigned int& p_player)
{
	p_player = 0;
	return m_playerGroupManager->GetLeaderPos(p_origin);
}

// FUNCTION: LEMBALL 0x00412e40
int CAI::ExportGroundAnimRecords(tCoord3d* p_records)
{
	return m_groundAnim->ExportCoordinates(p_records);
}

// FUNCTION: LEMBALL 0x00412e60
int CAI::ExportLiftEndpointRecords(LiftEndpointRecord* p_records)
{
	return m_liftManager->ExportEndpoints(p_records);
}

// FUNCTION: LEMBALL 0x00412e80
void CAI::AddNewTrapDoor(const AICOORD& p_position, unsigned long p_time)
{
	short id = CGameObject::NextLoadingId();
	m_trapDoorManager->AddNewDoor(id, p_position, TRAPDOOR_MODE_LOCAL_AUTOMATIC, p_time);
}

// FUNCTION: LEMBALL 0x00412eb0
void CAI::AddNewTrapDoor(int p_x, int p_y, int p_z, unsigned long p_time)
{
	short id = CGameObject::NextLoadingId();
	AICOORD position(p_x << FIXED_POINT_FRACTION_BITS,
					 p_y << FIXED_POINT_FRACTION_BITS,
					 p_z << FIXED_POINT_FRACTION_BITS);
	m_trapDoorManager->AddNewDoor(id, position, TRAPDOOR_MODE_LOCAL_AUTOMATIC, p_time);
}

// FUNCTION: LEMBALL 0x00412f00
CGame* CAI::LevelName()
{
	return m_game;
}

// FUNCTION: LEMBALL 0x00412f10
void CAI::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	if (p_dataSize > 4) {
		data = (unsigned short*) p_data;
		m_levelVersion = *data++;
	}
	else {
		m_levelVersion = LEVEL_VERSION_UNVERSIONED;
		data = (unsigned short*) p_data;
	}
	unsigned int mapType = *data++;
	m_mapType = mapType;
	m_timeLimit = *data++;
	CMap* map = m_map;
	map->m_mapType = mapType;
	map->m_ground.Clear();
	if (m_levelVersion >= LEVEL_VERSION_WITH_PLAYER_COUNTS) {
		m_lemmingCount = data[0];
		m_flagCounts[0] = data[1];
	}
	else {
		m_lemmingCount = 4;
		m_flagCounts[0] = 1;
	}
	m_levelTimeRemaining = m_timeLimit;
}

// FUNCTION: LEMBALL 0x00412fb0
void CAI::FixUpLevel()
{
	int count = g_wObjectCount;
	for (int i = 0; i < count; i++) {
		CGameObject* object = g_pObjects[(unsigned short) i];
		if (object->GetId() == (short) INVALID_OBJECT_ID) {
			object->SetId(CGameObject::NextId());
		}
	}
	m_objectManager->ConvertVer0ToVer1();
}

// FUNCTION: LEMBALL 0x00413000
unsigned short CAI::DoorId(int p_index)
{
	return m_doorManager->Id(p_index);
}

// FUNCTION: LEMBALL 0x00413020
unsigned short CAI::LiftId(int p_index)
{
	return m_liftManager->Id(p_index);
}

// FUNCTION: LEMBALL 0x00413040
CPlayerLemming* CAI::GetDead()
{
	return m_playerGroupManager->GetDead();
}

// FUNCTION: LEMBALL 0x00413090
bool CAI::GetObjectRequired(eObjectType p_objectType)
{
	return true;
}

// FUNCTION: LEMBALL 0x004130a0
void CAI::SetObjectRequired(eObjectType p_objectType, unsigned int p_required)
{
	if (p_objectType < (eObjectType) OBJECT_REQUIREMENT_TRACKING_EXCLUSION_START || p_objectType > TERRAIN_LIFT) {
		m_objectRequired[p_objectType] = p_required;
	}
}

// FUNCTION: LEMBALL 0x004130d0
CMover* CAI::FindMoverHeight(int p_x, int p_y, int& p_height)
{
	return m_moverManager->Find(p_x, p_y, p_height);
}

// FUNCTION: LEMBALL 0x004130f0
void CAI::NLemmings(int p_count)
{
	m_lemmingCount = p_count;
}

// FUNCTION: LEMBALL 0x00413100
void CAI::GetPlayerStartPosition(AICOORD& p_position, int p_index)
{
	m_playerGroupManager->GetPlayerStartPosition(p_position, p_index);
}

// FUNCTION: LEMBALL 0x00413120
int CAI::GetStartPositionCount()
{
	return m_playerGroupManager->m_startPositionCount;
}

// FUNCTION: LEMBALL 0x00413130
void CAI::ConfigurePlayerLemmingCounts(int p_playerCount, int p_count0, int p_count1, int p_count2, int p_count3)
{
	m_playerGroupManager->SetLemmingCounts(p_playerCount, p_count0, p_count1, p_count2, p_count3);
}

// FUNCTION: LEMBALL 0x00413160
int CAI::GetLemmingCountForPlayer(int p_playerIndex)
{
	return m_playerGroupManager->GetLemmingCountForPlayer(p_playerIndex);
}

// FUNCTION: LEMBALL 0x00413180
void CAI::AddANetworkStart(int p_x, int p_y, int p_z, int p_index)
{
	m_networkStartsX[p_index] = p_x;
	m_networkStartsY[p_index] = p_y;
	m_networkStartsZ[p_index] = p_z;
}

// FUNCTION: LEMBALL 0x004131b0
void CAI::SetNetworkTrapDoorCount(int p_count)
{
	m_networkTrapDoorCount = p_count;
	SetNetworkTrapDoors(p_count,
						m_networkTrapDoors[0],
						m_networkTrapDoors[1],
						m_networkTrapDoors[2],
						m_networkTrapDoors[3]);
}

// FUNCTION: LEMBALL 0x004131e0
void CAI::SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth)
{
	m_networkTrapDoorCount = p_count;
	if (p_first == NETWORK_START_TRAP_DOOR_DEFAULT) {
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
		if (m_networkStartsX[i] > MAP_COORDINATE_MAX || m_networkStartsX[i] < 0) {
			m_networkStartsX[i] = i * GROUND_BLOCK_PIXEL_SIZE;
		}
		if (m_networkStartsY[i] > MAP_COORDINATE_MAX || m_networkStartsY[i] < 0) {
			m_networkStartsY[i] = i * GROUND_BLOCK_PIXEL_SIZE;
		}
	}
}

// FUNCTION: LEMBALL 0x00413290
int CAI::GetNetworkTrapDoor(int p_index)
{
	return m_networkTrapDoors[p_index];
}

// FUNCTION: LEMBALL 0x004132a0
void CAI::SetNetworkTrapDoor(int p_value, int p_index)
{
	m_networkTrapDoors[p_index] = p_value;
}

// FUNCTION: LEMBALL 0x004132c0
void CAI::GetNetworkStartPosition(AICOORD& p_position, int p_index)
{
	p_position.m_xFixed = m_networkStartsX[p_index] << FIXED_POINT_FRACTION_BITS;
	p_position.m_yFixed = m_networkStartsY[p_index] << FIXED_POINT_FRACTION_BITS;
	p_position.m_zFixed = m_networkStartsZ[p_index] << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x00413300
void CAI::LoadFlagInfo(unsigned char* p_data, int p_size)
{
	unsigned short* data = (unsigned short*) p_data;
	if (m_networkMode == NETWORK_MODE_MULTIPLAYER) {
		if (m_isHost == NETWORK_ROLE_HOST) {
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

// FUNCTION: LEMBALL 0x00413370
int CAI::nDead()
{
	return m_playerGroupManager->m_deadCount;
}

// FUNCTION: LEMBALL 0x00413380
void CAI::ProcessLiftCliffs()
{
	m_liftManager->CalculateAllLiftCliffs();
}

// FUNCTION: LEMBALL 0x00413390
void CAI::Score(int p_score)
{
	m_score += p_score;
	if (m_score > GAME_SCORE_MAX_DISPLAY_VALUE) {
		m_score = GAME_SCORE_MAX_DISPLAY_VALUE;
	}
}

// FUNCTION: LEMBALL 0x004133c0
void CAI::ClearAllTrapDoors()
{
	m_trapDoorManager->ClearAllTrapDoors();
}

// FUNCTION: LEMBALL 0x00413e20
void CAI::Process()
{
	Process(0);
}
