#include "CAI.h"

#include "../../Control/Game/CDemo.h"
#include "../../Control/Game/CGame.h"
#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Game/GameTime.h"
#include "../../Control/Level/CLevelLoader.h"
#include "../../Map/Base/CMap.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Game/CPBNetworkGame.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CVSOStream.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CGameObject.h"
#include "../Groups/CEnemyGroupManager.h"
#include "../Groups/CFormationManager.h"
#include "../Groups/CPlayerLemmingGroupManager.h"
#include "../Groups/CSheepGroupManager.h"
#include "../Managers/CBallManager.h"
#include "../Managers/CBulletManager.h"
#include "../Managers/CCollectableManager.h"
#include "../Managers/CDoorManager.h"
#include "../Managers/CGodManager.h"
#include "../Managers/CHandManager.h"
#include "../Managers/CIceManager.h"
#include "../Managers/CInvisibleSwitchManager.h"
#include "../Managers/CLaserManager.h"
#include "../Managers/CLiftManager.h"
#include "../Managers/CMineManager.h"
#include "../Managers/CObjectManager.h"
#include "../Managers/CPaintGunManager.h"
#include "../Managers/CRocketManager.h"
#include "../Managers/CSlinkyManager.h"
#include "../Managers/CTrampolineManager.h"
#include "../Managers/CTrapDoorManager.h"
#include "../Messages/CGameStateMessage.h"
#include "../Objects/CAnimSpecial.h"
#include "../Objects/CBall.h"
#include "../Objects/CBalloonPost.h"
#include "../Objects/CGroundAnim.h"
#include "../Objects/CPlayerLemming.h"
#include "../Objects/CViewData.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/CPt3.h"
#include "AI/Base/CRect3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "AI/Objects/CBullet.h"
#include "AI/Objects/SwitchEntry.h"
#include "CAICursor.h"
#include "CMaze.h"
#include "CMoverManager.h"
#include "CNodeManager.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Messaging/CNetworkMessage.h"
#include "Visos/Network/CReadSocket.h"

extern int g_anDefaultTrapDoorLemmings[4][4];

// FUNCTION: LEMBALL 0x00410c10
CAI::CAI(CGame* p_game)
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
	m_objects = new CGameObject*[100];
	for (int i = 0; i < m_objectCapacity; i++) {
		m_objects[i] = 0;
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
	CGameObject::Init(this);
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
	m_clockSourceReady = 0;
	unsigned int network = g_pGameStatus->m_skill == 4;
	m_started = 0;
	m_gameStatePending = 0;
	m_isHost = 0;
	m_gameplayStartDelay = 0;
	m_gameplayEnabled = 0;
	m_networkStartReady = 1;
	m_payloadCapacity += 0x60;
	m_networkMode = network;
	if (m_networkMode != 0) {
		if (m_initialised == 0) {
			m_networkGame = new CPBNetworkGame(this);
		}
		m_isHost = g_pActiveConnection->m_isHost;
		if (m_initialised == 0) {
			m_gameStateMessage = new CGameStateMessage;
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
	m_clockStartPending = 1;
	m_mapType = 0;
	m_levelTimeRemaining = 0;
	m_gameStatus = GAME_STATUS_0;
	m_processState = 0;
	m_timeLimit = 180;
	if (m_initialised == 0) {
		m_map = new CMap;
	}
	m_map->Restart();
	g_pMap = m_map;
	CMap* map = m_map;
	map->m_ai = this;
	map->m_unk0x18 = this;
	if (m_initialised == 0) {
		m_maze = new CMaze(m_map);
	}
	g_pMaze = m_maze;
	if (m_initialised == 0) {
		m_aiQueue = new CBaseQueue(10, "AIQueue");
		m_aiQueue->Attach(this, 0);
	}
	if (m_networkMode != 0 && g_pActiveConnection != 0) {
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
		m_cursor = new CAICursor(this, 1024, 1024);
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
		m_nodeManager = new CNodeManager(300);
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
		m_animSpecial = new CAnimSpecial;
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

// FUNCTION: LEMBALL 0x004117a0
CAI::~CAI()
{
	if (g_nDemoMode != 0) {
		g_pDemo->SetDemoMode(0);
	}
	CGameObject::Init(0);
	g_pActiveAI = 0;
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
	if (m_networkMode != 0) {
		delete m_networkGame;
		delete m_gameStateMessage;
	}
	operator delete(m_objectRequired);
	if (g_pDemo != 0 && g_pDemo->m_demoMode != 0) {
		g_pGameStatus->m_levelState = m_score;
	}
	operator delete(m_objects);
}

// FUNCTION: LEMBALL 0x00411b10
void CAI::Start()
{
	CDemo* demo;
	CNetworkManager* networkManager;

	if (m_networkMode != 0) {
		m_gameStatePending = 1;
		m_networkStartReady = 0;
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

	GameState(GAME_STATUS_RUNNING);
	m_started = 1;
}

// FUNCTION: LEMBALL 0x00411b70
void CAI::SendGameState(eGameStates p_state, eGameStateStages p_stage)
{
	if (g_pActiveConnection != 0) {
		if (m_gameStateMessage->m_pendingSendCount != 0) {
			unsigned long start = CurrentMilliTimer();
			while (m_gameStateMessage->m_pendingSendCount != 0 && CurrentMilliTimer() - start < 2000) {
				g_pBaseNetwork->WaitProcess();
			}
		}
		if (m_gameStateMessage->m_pendingSendCount == 0) {
			m_gameStatePending = 1;
			m_gameStateMessage->m_state = p_state;
			m_gameStateMessage->m_stage = p_stage;
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

	apply = 0;
	state = p_message->m_state;
	stage = p_message->m_stage;
	*g_pSysOutput << "Received Game State " << (int) state << ", stage " << (int) stage << "\n";
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
		case GAME_STATE_0:
			if (m_gameStatus == GAME_STATUS_PAUSED) {
				SendGameState(state, GAME_STATE_STAGE_REJECT);
				m_gameStatePending = 0;
				return;
			}
			apply = 1;
			m_isSinglePlayer = 1;
			break;
		case GAME_STATE_2:
			g_pGameStatus->m_skillState = 2;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_3:
			if (m_gameStatus == GAME_STATUS_4 || m_gameStatus == GAME_STATUS_SUCCESS) {
				SendGameState(state, GAME_STATE_STAGE_REJECT);
				m_gameStatePending = 0;
				return;
			}
			m_gameStatus = GAME_STATUS_6;
			g_nGameOver = 1;
			break;
		case GAME_STATE_4:
			g_pGameStatus->m_skillState = 3;
			m_gameStatus = GAME_STATUS_SUCCESS;
			break;
		case GAME_STATE_6:
			g_pGameStatus->m_skillState = 5;
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
		case GAME_STATE_0:
			m_gameStatus = GAME_STATUS_PAUSED;
			break;
		case GAME_STATE_1:
			if (m_gameStatus != GAME_STATUS_RESTART) {
				m_started = 1;
				m_gameStatus = GAME_STATUS_RUNNING;
			}
			break;
		case GAME_STATE_2:
			g_pGameStatus->m_skillState = 2;
			m_gameStatus = GAME_STATUS_SUCCESS;
			break;
		case GAME_STATE_3:
			m_gameStatus = GAME_STATUS_4;
			break;
		case GAME_STATE_4:
			g_pGameStatus->m_skillState = 3;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_6:
			g_pGameStatus->m_skillState = 5;
			m_gameStatus = GAME_STATUS_FAILURE;
			break;
		case GAME_STATE_7:
			if ((unsigned int) m_gameTime > p_message->m_levelTime) {
				g_pGameStatus->m_skillState = 4;
				m_gameStatus = GAME_STATUS_SUCCESS;
			}
			else if ((unsigned int) m_gameTime != p_message->m_levelTime) {
				g_pGameStatus->m_skillState = 4;
				m_gameStatus = GAME_STATUS_FAILURE;
			}
			else if ((unsigned int) m_score > p_message->m_score) {
				g_pGameStatus->m_skillState = 1;
				m_gameStatus = GAME_STATUS_SUCCESS;
			}
			else if ((unsigned int) m_score < p_message->m_score) {
				g_pGameStatus->m_skillState = 1;
				m_gameStatus = GAME_STATUS_FAILURE;
			}
			else {
				g_pGameStatus->m_skillState = 4;
				m_gameStatus = GAME_STATUS_FAILURE;
			}
			break;
		case GAME_STATE_8:
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
	if (m_networkMode == 0) {
		switch (p_status) {
		case GAME_STATUS_SUCCESS:
			g_pGameStatus->m_skillState = 2;
			m_gameStatus = GAME_STATUS_SUCCESS;
			return;
		case GAME_STATUS_FAILURE:
			if (g_pGameStatus->m_skillState == 0) {
				g_pGameStatus->m_skillState = 3;
			}
			m_gameStatus = GAME_STATUS_FAILURE;
			return;
		case GAME_STATUS_7:
			g_pGameStatus->m_skillState = 4;
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
			SendGameState(GAME_STATE_0, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_RUNNING:
			SendGameState(GAME_STATE_1, GAME_STATE_STAGE_CONFIRM);
			if (m_gameStatus == GAME_STATUS_RUNNING) {
				m_gameStatePending = 0;
				return;
			}
			break;
		case GAME_STATUS_SUCCESS:
			SendGameState(GAME_STATE_2, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_4:
			SendGameState(GAME_STATE_3, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_FAILURE:
			if (g_pGameStatus->m_skillState == 5) {
				SendGameState(GAME_STATE_6, GAME_STATE_STAGE_REQUEST);
				return;
			}
			SendGameState(GAME_STATE_4, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_7:
			SendGameState(GAME_STATE_7, GAME_STATE_STAGE_REQUEST);
			return;
		case GAME_STATUS_RESTART:
			SendGameState(GAME_STATE_8, GAME_STATE_STAGE_REQUEST);
		}
	}
}

// FUNCTION: LEMBALL 0x00412080
void CAI::SetPlayerIDs()
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
			CPlayerLemming** lemming = m_networkLemmings + *offset;
			int count = 4;
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
	int count = (unsigned int) g_wObjectCount;
	int i = 0;
	for (;;) {
		if (i >= count) {
			break;
		}
		CGameObject* object = g_pObjects[(unsigned short) i];
		if (object != 0 && object->m_objectType != OBJECT_INVALID) {
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
	if (m_networkMode != 0 && g_pActiveConnection == 0) {
		return;
	}
	m_aiQueue->ProcessNMsgs(m_aiQueue->GetMessageCount());
	if (m_networkMode != 0 && m_networkStartReady == 0 &&
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
	case 0:
	case 2:
	case 4:
	case 6:
		break;
	default:
		SetGameTime();
		return;
	}
	if (m_started == 0 || m_gameStatePending != 0) {
		return;
	}
	SetGameTime();
	if (m_networkMode != 0) {
		if (g_pActiveConnection != 0 && g_pActiveConnection->IsChanged(*m_networkGame)) {
			g_pActiveConnection->GetLatest(*m_networkGame);
			m_clockSourceReady = 1;
		}
	}
	unsigned int time;
	if (m_networkMode != 0 && g_pActiveConnection != 0 && g_pActiveConnection->m_isHost != 0) {
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
		if (g_nGameOver == 0 && m_levelTimeRemaining < 600) {
			m_levelTimeRemaining = m_timeLimit - (time - m_levelStartTick) / 20;
		}
		int remaining = m_levelTimeRemaining;
		remaining += m_gameTime;
		if (remaining < 0) {
			GameState(GAME_STATUS_7);
			m_levelTimeRemaining = -1 - m_gameTime;
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
			GameState(GAME_STATUS_4);
			g_nGameOver = 1;
			m_gameOverDeadline = g_dwGameTick + 0x3c;
		}
		if (m_gameStatus == 4 && m_gameOverDeadline < g_dwGameTick) {
			GameState(GAME_STATUS_SUCCESS);
		}
	}
	if (g_pActiveConnection != 0 &&
		(LemmingsSFXChanged() || g_dwSimulationTimestamp - m_lastNetworkSendCheckTick > 0x42)) {
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
int CAI::HitTrampoline(const AiCoord& p_position, CGameObject* p_object)
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
					 AiCoord p_start,
					 AiCoord p_target)
{
	m_bulletManager->RequestBullet(p_id, p_bulletType, p_owner, p_parameter, p_start, p_target);
}

// FUNCTION: LEMBALL 0x00412660
int CAI::ProcessMsg(Message* p_message)
{
	unsigned int messageType = p_message->type;
	if (messageType != 4) {
		if (m_gameplayEnabled == 0) {
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

// FUNCTION: LEMBALL 0x00412780
bool CAI::PlayerCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	return m_playerGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127a0
bool CAI::EnemyCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	return m_enemyGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127c0
bool CAI::SheepCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	return m_sheepGroupManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x004127e0
bool CAI::BulletCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate)
{
	return m_bulletManager->CheckGroupIntersection(p_rect, p_coordinate);
}

// FUNCTION: LEMBALL 0x00412890
void CAI::StepOn(const AiCoord& p_position, CGameObject* p_object, unsigned short p_mask)
{
	int y;
	int x = p_position.m_xFixed >> 12;
	y = p_position.m_yFixed >> 12;
	int blockX = x / 16;
	int blockY = y / 16;

	if (p_object->m_onMover != 0) {
		return;
	}

	unsigned short groundZ;
	{
		CMap* map = m_map;
		int groundX = x >> 4;
		int groundY = y >> 4;
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
		p_object->m_action = ACTION_15;
		if (objectType != TERRAIN_ELECTRIC) {
			p_object->m_actionArgument = 2;
			p_object->m_stateTimer = g_dwGameTick * 50;
			p_object->SetSndEffect(SFX_AAAAH1);
			return;
		}

		p_object->m_actionArgument = 1;
		p_object->m_stateTimer = g_dwGameTick * 50;
		p_object->SetSndEffect(SFX_AAAAH2);
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

// FUNCTION: LEMBALL 0x00412ad0
bool CAI::OpenDoor(const AiCoord& p_position, CGameObject* p_object, unsigned short p_mask)
{
	int blockX = (p_position.m_xFixed >> 12) / 16;
	int blockY = (p_position.m_yFixed >> 12) / 16;
	unsigned short collision;
	if (blockX < 0 || blockY < 0) {
		collision = 3;
	}
	else {
		CMap* map = m_map;
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

// FUNCTION: LEMBALL 0x00412b60
CPt3 CAI::GetNodePosition(int p_node)
{
	return m_nodeManager->GetNodePosition(p_node);
}

// FUNCTION: LEMBALL 0x00412b80
void CAI::AddData()
{
	int remaining = 4;
	CPlayerLemming** lemming = m_networkLemmings;
	do {
		CNetworkMessage* stream = this;
		CNetworkMessage* message = (CNetworkMessage*) ((unsigned char*) *lemming + sizeof(CGlobalGameObject));
		message->CopyDataStream(stream->m_writeCursor, 0);
		stream->m_writeCursor += message->m_writeCursor - message->m_buffer;
		lemming++;
		remaining--;
	} while (remaining != 0);
}

// FUNCTION: LEMBALL 0x00412be0
void CAI::GetData()
{
	CPlayerLemming** lemming = &m_networkLemmings[4];
	for (int i = 0; i < 4; i++) {
		CNetworkMessage* stream = this;
		CNetworkMessage* message = (CNetworkMessage*) ((unsigned char*) *lemming + sizeof(CGlobalGameObject));
		if (message->Set(stream->m_readCursor)) {
			stream->m_readCursor = message->m_readCursor;
		}
		lemming++;
	}
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
	g_pGameStatus->m_skillState = 5;
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

// FUNCTION: LEMBALL 0x00412dc0
void CAI::GetPlayerPos(int p_id, AiCoord& p_position)
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
	} while (index < 4);
}

// FUNCTION: LEMBALL 0x00412e20
bool CAI::GetOrigin(AiCoord& p_origin, unsigned int& p_player)
{
	p_player = 0;
	return m_playerGroupManager->GetLeaderPos(p_origin);
}

// FUNCTION: LEMBALL 0x00412eb0
void CAI::AddNewTrapDoor(int p_x, int p_y, int p_z, unsigned long p_time)
{
	short id = CGameObject::NextLoadingId();
	AiCoord position(p_x << 12, p_y << 12, p_z << 12);
	m_trapDoorManager->AddNewDoor(id, position, 0, p_time);
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
		m_levelVersion = 0;
		data = (unsigned short*) p_data;
	}
	unsigned int mapType = *data++;
	m_mapType = mapType;
	m_timeLimit = *data++;
	CMap* map = m_map;
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
	m_levelTimeRemaining = m_timeLimit;
}

// FUNCTION: LEMBALL 0x00412fb0
void CAI::FixUpLevel()
{
	int count = (unsigned int) g_wObjectCount;
	for (int i = 0; i < count; i++) {
		CGameObject* object = g_pObjects[(unsigned short) i];
		if (object->GetId() == (short) 0xffff) {
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
	return 1;
}

// FUNCTION: LEMBALL 0x004130a0
void CAI::SetObjectRequired(eObjectType p_objectType, unsigned int p_required)
{
	if (p_objectType < (eObjectType) 0x211 || p_objectType > TERRAIN_LIFT) {
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

// FUNCTION: LEMBALL 0x00413180
void CAI::AddANetworkStart(int p_x, int p_y, int p_z, int p_index)
{
	m_networkStartsX[p_index] = p_x;
	m_networkStartsY[p_index] = p_y;
	m_networkStartsZ[p_index] = p_z;
}

// FUNCTION: LEMBALL 0x004131e0
void CAI::SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth)
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

// FUNCTION: LEMBALL 0x00413300
void CAI::LoadFlagInfo(unsigned char* p_data, int p_size)
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

// FUNCTION: LEMBALL 0x00413370
int CAI::nDead()
{
	return m_playerGroupManager->m_deadCount;
}

// FUNCTION: LEMBALL 0x00413390
void CAI::Score(int p_score)
{
	m_score += p_score;
	if (m_score > 9999999) {
		m_score = 9999999;
	}
}

// FUNCTION: LEMBALL 0x00413e20
void CAI::Process()
{
	Process(0);
}

// GLOBAL: LEMBALL 0x004a782c
CAI* g_pGenericGroupAI;

// GLOBAL: LEMBALL 0x004a74b0
CAI* g_pAI;

// GLOBAL: LEMBALL 0x004a74b8
int g_nGameOver = 0;

// GLOBAL: LEMBALL 0x0049cf34
CAI* g_pActiveAI = 0;
