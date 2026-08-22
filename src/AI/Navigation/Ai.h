#ifndef LEMBALL_AI_NAVIGATION_AI_H
#define LEMBALL_AI_NAVIGATION_AI_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/Process.h"          // complete type
#include "../../Visos/Messaging/NetworkMessage.h"    // complete type
#include "../Base/AiCoord.h"                         // complete type
#include "../Base/Pt3.h"                             // complete type

// SIZE 0x1f0
// VTABLE: LEMBALL 0x00493a50 primary BaseQueueHandler view
// VTABLE: LEMBALL 0x00493a40 secondary Process view at +0x10
// VTABLE: LEMBALL 0x00493a20 secondary NetworkMessage view at +0x1c
class Ai : public BaseQueueHandler, public Process {
public:
	Ai(Game* p_arg0);
	Game* LevelName();
	Mover* FindMoverHeight(int p_x, int p_y, int& p_height);
	PlayerLemming* GetDead();
	Pt3 GetNodePosition(int p_node);
	bool BulletCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);
	bool EnemyCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);
	bool GetObjectRequired(eObjectType p_objectType);
	bool IsLemmingPlayerControlled(PlayerLemming* p_lemming);
	bool LemmingsSfxChanged();
	bool OpenDoor(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask);
	bool PlayerCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);
	bool SheepCheckGroupIntersection(VsRect* p_rect, AiCoord* p_coordinate);
	int GetData(ViewData* p_viewData);
	int NDead();
	unsigned int RemoteGameState(GameStateMessage* p_message);
	unsigned int SetObjectRequired(eObjectType p_objectType, unsigned char p_required);
	unsigned int StepOn(const AiCoord& p_position, GameObject* p_object, unsigned short p_mask);
	unsigned short DoorId(int p_index);
	unsigned short LiftId(int p_index);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	void AddANetworkStart(int p_x, int p_y, int p_z, int p_index);
	void AddData();
	void AddNewTrapDoor(int p_x, int p_y, int p_z, unsigned long p_time);
	void AddTime(int p_time);
	void DecideAnimsRequired();
	void FireBullet(unsigned short p_id,
					eBulletType p_bulletType,
					eOwner p_owner,
					int p_parameter,
					AiCoord p_start,
					AiCoord p_target);
	void FixUpLevel();
	void GameState(eGameStatus p_status);
	void GetData();
	void GetOrigin(AiCoord& p_origin, unsigned char& p_player);
	void GetPlayerPos(int p_id, AiCoord& p_position);
	void HitTrampoline(const AiCoord& p_position, GameObject* p_object);
	void LoadFlagInfo(unsigned char* p_data, int p_size);
	void LoadLevel(unsigned char* p_data, int p_version, unsigned char p_skip);
	void NLemmings(int p_count);
	void Process();
	void Process(unsigned char p_paused);
	void QuitGame();
	void Restart();
	void Score(int p_score);
	void SendGameState(eGameStates p_state, eGameStateStages p_stage);
	void SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth);
	void SetPlayerIDs();
	void Start();
	void SwitchMessage(swMessage p_message, int p_first, int p_last);
	~Ai();

private:
	NetworkMessage m_networkMessage;                  // 0x1c
	int m_initialised;                                // 0x4c
	unsigned int* m_objectRequired;                   // 0x50
	unsigned short m_levelVersion;                    // 0x54
	int m_networkMode;                                // 0x64
	GameStateMessage* m_gameStateMessage;             // 0x74
	PbNetworkGame* m_networkGame;                     // 0x78
	AnimSpecial* m_animSpecial;                       // 0x8c
	int m_networkStartsX[4];                          // 0x90
	int m_networkStartsY[4];                          // 0xa0
	int m_networkStartsZ[4];                          // 0xb0
	int m_networkTrapDoors[4];                        // 0xc0
	int m_networkTrapDoorCount;                       // 0xd0
	int m_score;                                      // 0xf0
	int m_lemmingCount;                               // 0xf8
	int m_flagCounts[2];                              // 0xfc
	Map* m_map;                                       // 0x110
	Maze* m_maze;                                     // 0x114
	int m_objectCount;                                // 0x118
	int m_objectCapacity;                             // 0x11c
	GameObject** m_objects;                           // 0x120
	Game* m_game;                                     // 0x154
	ObjectManager* m_objectManager;                   // 0x158
	PlayerLemmingGroupManager* m_playerGroupManager;  // 0x15c
	AiCursor* m_cursor;                               // 0x160
	BaseQueue* m_aiQueue;                             // 0x164
	BulletManager* m_bulletManager;                   // 0x168
	FormationManager* m_formationManager;             // 0x16c
	SheepGroupManager* m_sheepGroupManager;           // 0x170
	EnemyGroupManager* m_enemyGroupManager;           // 0x174
	LevelLoader* m_levelLoader;                       // 0x178
	NodeManager* m_nodeManager;                       // 0x17c
	BallManager* m_ballManager;                       // 0x180
	CollectableManager* m_collectableManager;         // 0x184
	MineManager* m_mineManager;                       // 0x188
	LiftManager* m_liftManager;                       // 0x18c
	DoorManager* m_doorManager;                       // 0x190
	GroundAnim* m_groundAnim;                         // 0x194
	RocketManager* m_rocketManager;                   // 0x198
	LaserManager* m_laserManager;                     // 0x1a0
	HandManager* m_handManager;                       // 0x1a4
	BalloonPost* m_balloonPost;                       // 0x1a8
	IceManager* m_iceManager;                         // 0x1ac
	PaintGunManager* m_paintGunManager;               // 0x1b0
	TrampolineManager* m_trampolineManager;           // 0x1b4
	MoverManager* m_moverManager;                     // 0x1c0
	TrapDoorManager* m_trapDoorManager;               // 0x1c4
	SlinkyManager* m_slinkyManager;                   // 0x1c8
	InvisibleSwitchManager* m_invisibleSwitchManager; // 0x1cc
	PlayerLemming* m_networkLemmings[8];              // 0x1d0
};

// Confirmed class-scoped globals.
extern Ai* g_pGenericGroupAI;
extern Ai* g_pAI;
extern Ai* g_pActiveAI;
#endif
