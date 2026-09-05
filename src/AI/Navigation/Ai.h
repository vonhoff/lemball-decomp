#ifndef LEMBALL_AI_NAVIGATION_AI_H
#define LEMBALL_AI_NAVIGATION_AI_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseProcess.h"      // complete type
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Messaging/NetworkMessage.h"    // complete type
#include "../Base/AiCoord.h"                         // complete type
#include "../Base/Pt3.h"                             // complete type

// SIZE 0x1f0
// VTABLE: LEMBALL 0x00493a50 BaseQueueHandler
// VTABLE: LEMBALL 0x00493a40 BaseProcess
// VTABLE: LEMBALL 0x00493a20 NetworkMessage
class Ai : public BaseQueueHandler, public BaseProcess, public NetworkMessage {
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
	void SetObjectRequired(eObjectType p_objectType, unsigned int p_required);
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
	void GetOrigin(AiCoord& p_origin, unsigned int& p_player);
	void GetPlayerPos(int p_id, AiCoord& p_position);
	void HitTrampoline(const AiCoord& p_position, GameObject* p_object);
	void LoadFlagInfo(unsigned char* p_data, int p_size);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void NLemmings(int p_count);
	virtual void Process(); // vtable+0x04
	void Process(unsigned char p_paused);
	void QuitGame();
	void Restart();
	void Score(int p_score);
	void SendGameState(eGameStates p_state, eGameStateStages p_stage);
	void SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth);
	void SetPlayerIDs();
	void Start();
	void SwitchMessage(swMessage p_message, int p_first, int p_last = 0, int p_arg3 = 0);
	~Ai();

	friend class Ball;
	friend class Bullet;
	friend class ObjectManager;
	friend class PlayerLemming;
	friend class PlayerLemmingGroupManager;
	friend class C2D;
	friend class Main2DDisplay;
	friend class SlinkyManager;
	friend class TrampolineManager;
	friend class TrapDoorManager;
	friend class RocketManager;
	friend class PaintGunManager;
	friend class MineManager;
	friend class LaserManager;
	friend class LiftManager;
	friend class HandManager;
	friend class IceManager;
	friend class BallManager;
	friend class DoorManager;
	friend class LevelLoader;
	friend class MoverManager;
	friend class InvisibleSwitchManager;
	friend class FormationManager;
	friend class SheepGroupManager;
	friend class EnemyGroupManager;
	friend class NodeManager;
	friend class Collectable;
	friend class Enemy;
	friend class PanelPauseButton;
	friend bool GameOver(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);

private:
	undefined4 m_unk0x48;                      // 0x48
	int m_initialised;                         // 0x4c
	unsigned int* m_objectRequired;            // 0x50
	unsigned short m_levelVersion;             // 0x54
	undefined2 m_pad0x56;                      // 0x56
	PlayerLemmingGroupManager* m_playerGroups; // 0x58
	undefined4 m_unk0x5c;                      // 0x5c
	undefined4 m_unk0x60;                      // 0x60: level map type (LoadLevel 0x00412f4c)
	// Restart (0x00410d98): 32-bit result of skill == 4 (0 or 1).
	undefined4 m_unk0x64;    // 0x64
	undefined4 m_unk0x68[5]; // 0x68
	// Restart (0x00410d82, 0x00410dd2): zero, then the connection's host flag.
	undefined4 m_unk0x7c;                             // 0x7c
	undefined4 m_unk0x80[4];                          // 0x80
	int m_networkStartsX[4];                          // 0x90
	int m_networkStartsY[4];                          // 0xa0
	int m_networkStartsZ[4];                          // 0xb0
	int m_networkTrapDoors[4];                        // 0xc0
	int m_networkTrapDoorCount;                       // 0xd0
	int m_unk0xd4;                                    // 0xd4
	int m_paused;                                     // 0xd8
	int m_unk0xdc;                                    // 0xdc
	int m_unk0xe0;                                    // 0xe0
	int m_unk0xe4;                                    // 0xe4
	int m_gameTime;                                   // 0xe8
	int m_unk0xec;                                    // 0xec
	int m_score;                                      // 0xf0
	undefined4 m_unk0xf4;                             // 0xf4
	int m_lemmingCount;                               // 0xf8
	int m_flagCounts[2];                              // 0xfc
	undefined4 m_unk0x104;                            // 0x104
	int m_gameStatus;                                 // 0x108
	int m_isSinglePlayer;                             // 0x10c
	Map* m_map;                                       // 0x110
	Maze* m_maze;                                     // 0x114
	int m_objectCount;                                // 0x118
	int m_objectCapacity;                             // 0x11c
	GameObject** m_objects;                           // 0x120
	undefined4 m_unk0x124[12];                        // 0x124
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
	undefined4 m_pad0x19c;                            // 0x19c
	LaserManager* m_laserManager;                     // 0x1a0
	HandManager* m_handManager;                       // 0x1a4
	BalloonPost* m_balloonPost;                       // 0x1a8
	IceManager* m_iceManager;                         // 0x1ac
	PaintGunManager* m_paintGunManager;               // 0x1b0
	TrampolineManager* m_trampolineManager;           // 0x1b4
	undefined4 m_unk0x1b8[2];                         // 0x1b8
	MoverManager* m_moverManager;                     // 0x1c0
	TrapDoorManager* m_trapDoorManager;               // 0x1c4
	SlinkyManager* m_slinkyManager;                   // 0x1c8
	InvisibleSwitchManager* m_invisibleSwitchManager; // 0x1cc
	PlayerLemming* m_networkLemmings[8];              // 0x1d0
};

extern Ai* g_pGenericGroupAI;
extern Ai* g_pAI;
extern Ai* g_pActiveAI;
extern int g_nGameOver;

// SYNTHETIC: LEMBALL 0x00413e30
// Ai::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00413e60
// ??_EAi@@WBA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00413e70
// ??_EAi@@WBM@AEPAXI@Z

#endif
