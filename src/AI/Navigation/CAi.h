#ifndef LEMBALL_AI_NAVIGATION_CAI_H
#define LEMBALL_AI_NAVIGATION_CAI_H

#include "../../Visos/Foundation/CBaseProcess.h"      // complete type
#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Messaging/CNetworkMessage.h"    // complete type
#include "../Base/AiCoord.h"                          // complete type
#include "../Base/CPt3.h"                             // complete type
#include "../Base/CRect3.h"                           // complete type
#include "../Messages/CGameStateMessage.h"
#include "../Objects/CBullet.h"
#include "../Objects/SwitchEntry.h"
#include "AI/Base/ObjectTypes.h"

class CAiCursor;
class CAnimSpecial;
class CBallManager;
class CBalloonPost;
class CBaseQueue;
class CBulletManager;
class CCollectableManager;
class CDoorManager;
class CEnemyGroupManager;
class CFormationManager;
class CGame;
class CGameObject;
class CGroundAnim;
class CHandManager;
class CIceManager;
class Info;
class CInvisibleSwitchManager;
class CLaserManager;
class CLevelLoader;
class CLiftManager;
class CMap;
class CMaze;
class CMineManager;
class CMover;
class CMoverManager;
class CNodeManager;
class CObjectManager;
class CPaintGunManager;
class CPbNetworkGame;
class CPlayerLemming;
class CPlayerLemmingGroupManager;
class CRocketManager;
class CSheepGroupManager;
class CSlinkyManager;
class CTrampolineManager;
class CTrapDoorManager;
class CViewData;
class CVsRect;
enum eGameStatus {
	GAME_STATUS_0 = 0,
	GAME_STATUS_1 = 1,
	GAME_STATUS_2 = 2,
	GAME_STATUS_3 = 3,
	GAME_STATUS_4 = 4,
	GAME_STATUS_5 = 5,
	GAME_STATUS_6 = 6,
	GAME_STATUS_7 = 7,
	GAME_STATUS_8 = 8
};

// SIZE 0x1f0
// VTABLE: LEMBALL 0x00493a50 CBaseQueueHandler
// VTABLE: LEMBALL 0x00493a40 CBaseProcess
// VTABLE: LEMBALL 0x00493a20 CNetworkMessage
class CAi : public CBaseQueueHandler, public CBaseProcess, public CNetworkMessage {
public:
	CAi(CGame* p_game);
	CGame* LevelName();
	CMover* FindMoverHeight(int p_x, int p_y, int& p_height);
	CPlayerLemming* GetDead();
	CPt3 GetNodePosition(int p_node);
	bool BulletCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);
	bool EnemyCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);
	bool GetObjectRequired(eObjectType p_objectType);
	bool IsLemmingPlayerControlled(CPlayerLemming* p_lemming);
	bool LemmingsSfxChanged();
	bool OpenDoor(const AiCoord& p_position, CGameObject* p_object, unsigned short p_mask);
	bool PlayerCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);
	bool SheepCheckGroupIntersection(CVsRect* p_rect, AiCoord* p_coordinate);
	int GetData(CViewData* p_viewData);
	int NDead();
	void RemoteGameState(CGameStateMessage* p_message);
	void SetObjectRequired(eObjectType p_objectType, unsigned int p_required);
	void StepOn(const AiCoord& p_position, CGameObject* p_object, unsigned short p_mask);
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
	bool GetOrigin(AiCoord& p_origin, unsigned int& p_player);
	void GetPlayerPos(int p_id, AiCoord& p_position);
	int HitTrampoline(const AiCoord& p_position, CGameObject* p_object);
	void LoadFlagInfo(unsigned char* p_data, int p_size);
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void NLemmings(int p_count);
	virtual void Process(); // vtable+0x04
	void Process(int p_paused);
	void QuitGame();
	void Restart();
	void Score(int p_score);
	void SendGameState(eGameStates p_state, eGameStateStages p_stage);
	void SetNetworkTrapDoors(int p_count, int p_first, int p_second, int p_third, int p_fourth);
	void SetPlayerIDs();
	void Start();
	void SwitchMessage(swMessage p_message, int p_first, int p_last = 0, int p_arg3 = 0);
	~CAi();

	friend class CBall;
	friend class CBullet;
	friend class CAiCursor;
	friend class CObjectManager;
	friend class CPlayerLemming;
	friend class CPlayerLemmingGroupManager;
	friend class CPbNetworkGame;
	friend class C2D;
	friend class CMain2DDisplay;
	friend class CSlinkyManager;
	friend class CSlinky;
	friend class CCursorMotion;
	friend class CTrampolineManager;
	friend class CTrapDoorManager;
	friend class CRocketManager;
	friend class CPaintGunManager;
	friend class CMineManager;
	friend class CLaserManager;
	friend class CLiftManager;
	friend class CHandManager;
	friend class CIceManager;
	friend class CBallManager;
	friend class CDoorManager;
	friend class CGodManager;
	friend class CLevelLoader;
	friend class CMoverManager;
	friend class CInvisibleSwitchManager;
	friend class CInvisibleSwitch;
	friend class CFormationManager;
	friend class CFlag;
	friend class CSheep;
	friend class CSheepGroupManager;
	friend class CEnemyGroupManager;
	friend class CNodeManager;
	friend class CCollectableManager;
	friend class CCollectable;
	friend class CEnemy;
	friend class CPanel;
	friend class CPanelLemming;
	friend class CPanelPauseButton;
	friend class CPanelButton;
	friend bool GameOver(CAi* p_ai, CGameObject* p_object, Info* p_info);

private:
	unsigned int m_unk0x48;         // 0x48
	int m_initialised;              // 0x4c
	unsigned int* m_objectRequired; // 0x50
	unsigned short m_levelVersion;  // 0x54
	unsigned short m_pad0x56;       // 0x56
	unsigned int m_playerGroups;    // 0x58: gameplay enable flag, set to 1 at 0x004123bb
	unsigned int m_unk0x5c;         // 0x5c
	unsigned int m_mapType;         // 0x60: level map type (LoadLevel 0x00412f4c)
	// Restart (0x00410d98): 32-bit result of skill == 4 (0 or 1).
	unsigned int m_networkMode;            // 0x64
	unsigned int m_unk0x68;                // 0x68
	unsigned int m_unk0x6c;                // 0x6c
	unsigned int m_unk0x70;                // 0x70
	CGameStateMessage* m_gameStateMessage; // 0x74: constructor result at 0x00410dec
	CPbNetworkGame* m_networkGame;         // 0x78: constructor result at 0x00410dba
	// Restart (0x00410d82, 0x00410dd2): zero, then the connection's host flag.
	unsigned int m_isHost;                             // 0x7c
	unsigned int m_unk0x80[2];                         // 0x80
	unsigned int m_unk0x88;                            // 0x88
	CAnimSpecial* m_animSpecial;                       // 0x8c: allocation/Initialise at 0x0041171a
	int m_networkStartsX[4];                           // 0x90
	int m_networkStartsY[4];                           // 0xa0
	int m_networkStartsZ[4];                           // 0xb0
	int m_networkTrapDoors[4];                         // 0xc0
	int m_networkTrapDoorCount;                        // 0xd0
	int m_unk0xd4;                                     // 0xd4
	int m_paused;                                      // 0xd8
	int m_clockStartPending;                           // 0xdc
	int m_levelStartTick;                              // 0xe0
	int m_levelTimeRemaining;                          // 0xe4
	int m_gameTime;                                    // 0xe8
	int m_timeLimit;                                   // 0xec
	int m_score;                                       // 0xf0
	unsigned int m_unk0xf4;                            // 0xf4
	int m_lemmingCount;                                // 0xf8
	int m_flagCounts[2];                               // 0xfc
	unsigned int m_gameOverDeadline;                   // 0x104
	eGameStatus m_gameStatus;                          // 0x108
	int m_isSinglePlayer;                              // 0x10c
	CMap* m_map;                                       // 0x110
	CMaze* m_maze;                                     // 0x114
	int m_objectCount;                                 // 0x118
	int m_objectCapacity;                              // 0x11c
	CGameObject** m_objects;                           // 0x120
	CPt3 m_collisionPoint;                             // 0x124
	int m_collisionIndex;                              // 0x130
	CRect3 m_collisionRect;                            // 0x134
	int m_rectCollisionIndex;                          // 0x14c
	CGameObject* m_collisionExclude;                   // 0x150
	CGame* m_game;                                     // 0x154
	CObjectManager* m_objectManager;                   // 0x158
	CPlayerLemmingGroupManager* m_playerGroupManager;  // 0x15c
	CAiCursor* m_cursor;                               // 0x160
	CBaseQueue* m_aiQueue;                             // 0x164
	CBulletManager* m_bulletManager;                   // 0x168
	CFormationManager* m_formationManager;             // 0x16c
	CSheepGroupManager* m_sheepGroupManager;           // 0x170
	CEnemyGroupManager* m_enemyGroupManager;           // 0x174
	CLevelLoader* m_levelLoader;                       // 0x178
	CNodeManager* m_nodeManager;                       // 0x17c
	CBallManager* m_ballManager;                       // 0x180
	CCollectableManager* m_collectableManager;         // 0x184
	CMineManager* m_mineManager;                       // 0x188
	CLiftManager* m_liftManager;                       // 0x18c
	CDoorManager* m_doorManager;                       // 0x190
	CGroundAnim* m_groundAnim;                         // 0x194
	CRocketManager* m_rocketManager;                   // 0x198
	unsigned int m_pad0x19c;                           // 0x19c
	CLaserManager* m_laserManager;                     // 0x1a0
	CHandManager* m_handManager;                       // 0x1a4
	CBalloonPost* m_balloonPost;                       // 0x1a8
	CIceManager* m_iceManager;                         // 0x1ac
	CPaintGunManager* m_paintGunManager;               // 0x1b0
	CTrampolineManager* m_trampolineManager;           // 0x1b4
	unsigned int m_unk0x1b8[2];                        // 0x1b8
	CMoverManager* m_moverManager;                     // 0x1c0
	CTrapDoorManager* m_trapDoorManager;               // 0x1c4
	CSlinkyManager* m_slinkyManager;                   // 0x1c8
	CInvisibleSwitchManager* m_invisibleSwitchManager; // 0x1cc
	CPlayerLemming* m_networkLemmings[8];              // 0x1d0
};

extern CAi* g_pGenericGroupAI;
extern CAi* g_pAI;
extern CAi* g_pActiveAI;
extern int g_nGameOver;

// SYNTHETIC: LEMBALL 0x00413e30
// CAi::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00413e60
// ??_ECAi@@WBA@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00413e70
// ??_ECAi@@WBM@AEPAXI@Z

#endif
