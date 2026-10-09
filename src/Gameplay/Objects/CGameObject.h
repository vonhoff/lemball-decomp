#ifndef LEMBALL_AI_BASE_CGAMEOBJECT_H
#define LEMBALL_AI_BASE_CGAMEOBJECT_H

#include "Application/SoundEffects.h"
#include "CViewData.h"
#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/CRect3.h"
#include "Gameplay/Movement/CVMovePos.h"
#include "Gameplay/Simulation/GameTime.h"
#include "ObjectActions.h"
#include "ObjectIds.h"
#include "ObjectTypes.h"

class CVSRect;

enum {
	GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS = 4000,
	GAME_OBJECT_BOREDOM_RANDOM_DELAY_RANGE_MS = 5000,
	GAME_OBJECT_FIRE_WINDUP_TICKS = 10,
	GAME_OBJECT_FLIGHT_START_DELAY_TICKS = 10,
	GAME_OBJECT_LANDING_TRANSITION_DELAY_TICKS = 8
};

class CAI;
class CAiDestinationList;
class CBaseObjectManager;
class CBullet;
class CIce;
class tInfo;
struct tCoord3d;
struct CPt3;
struct tStateEntry;

#define GAME_OBJECT_COLLISION_AFFECT_ROUTE_ON_BLOCK 0x01
#define GAME_OBJECT_COLLISION_ALLOW_JUMP 0x02
#define GAME_OBJECT_COLLISION_ALLOW_FALL 0x04
#define GAME_OBJECT_COLLISION_STEP_ON_MINE 0x08
#define GAME_OBJECT_COLLISION_STEP_ON_SPECIAL_OBJECTS 0x10
#define GAME_OBJECT_COLLISION_OPEN_DOORS 0x20
#define GAME_OBJECT_COLLISION_TRIGGER_HAZARDS 0x40
#define GAME_OBJECT_COLLISION_STEP_ON_INVISIBLE_SWITCHES 0x100
#define GAME_OBJECT_COLLISION_ENEMY 0x118
#define GAME_OBJECT_COLLISION_SHEEP 0x108
#define GAME_OBJECT_COLLISION_PLAYER_LEMMING 0x17f // SIZE 0x124
#define GAME_OBJECT_COLLISION_XY_MIN_INSET 8
#define GAME_OBJECT_COLLISION_XY_MAX_OFFSET 7
#define GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET 15
// VTABLE: LEMBALL 0x00493c40
class CGameObject {
public:
	AICOORD Position();
	AICOORD GetDestination();
	AICOORD GetNextDestination();
	CGameObject(eObjectType p_objectType, unsigned short p_collisionFlags, unsigned short p_destinationCapacity);
	bool DestinationExists();
	bool FacingDestination();
	bool Fall();
	void Jump();
	void ResetInstructions();
	static short NextId();
	static short NextLoadingId();
	short GetId();
	void Blocked();
	unsigned short MapCheck(int p_x, int p_y);
	virtual ~CGameObject();                                                          // vtable+0x00
	virtual void Action(eAction p_action);                                           // vtable+0x08
	virtual void Action(eAction p_action, int p_actionArgument);                     // vtable+0x04
	virtual void GetViewData(CViewData& p_viewData);                                 // vtable+0x0c
	virtual void ForgetObjectLink(unsigned short p_arg0);                            // vtable+0x10
	virtual bool Process();                                                          // vtable+0x14
	virtual bool Activate(class CGameObject* p_object);                              // vtable+0x18
	virtual bool IsFlying();                                                         // vtable+0x1c
	virtual void StartFly(C3DVector& p_velocity, C3DVector* p_origin);               // vtable+0x20
	virtual void Fly();                                                              // vtable+0x24
	virtual int Usage();                                                             // vtable+0x28
	virtual AICOORD ActivatePosition();                                              // vtable+0x2c
	virtual void StartStanding();                                                    // vtable+0x30
	virtual void SetSndEffect(eSoundEffect p_soundEffect);                           // vtable+0x34
	virtual eSoundEffect GetSndEffect();                                             // vtable+0x38
	virtual bool StartRoute();                                                       // vtable+0x3c
	virtual bool SearchRoute();                                                      // vtable+0x40
	virtual bool Move();                                                             // vtable+0x44
	virtual void GetBoundingBox(CVSRect& p_rect);                                    // vtable+0x48
	virtual bool Collision(const CPt3& p_point);                                     // vtable+0x50
	virtual bool Collision(const CRect3& p_bounds);                                  // vtable+0x4c
	virtual void HitBullet(CBullet* p_bullet);                                       // vtable+0x54
	virtual void HitBall();                                                          // vtable+0x58
	virtual bool IsHit();                                                            // vtable+0x5c
	virtual void GetHit();                                                           // vtable+0x60
	virtual void HitMine();                                                          // vtable+0x64
	virtual void Die();                                                              // vtable+0x68
	virtual void Land();                                                             // vtable+0x6c
	virtual bool IsSelectable();                                                     // vtable+0x70
	virtual bool FacingCursor();                                                     // vtable+0x74
	virtual void TurnToFaceCursor();                                                 // vtable+0x78
	virtual bool IsRequestingFire();                                                 // vtable+0x7c
	virtual int Bored();                                                             // vtable+0x80
	virtual void SetBored(unsigned long p_arg0);                                     // vtable+0x84
	virtual void Fire();                                                             // vtable+0x88
	virtual void StartFiring();                                                      // vtable+0x8c
	virtual void EndFiring();                                                        // vtable+0x90
	virtual void RandomAction();                                                     // vtable+0x94
	virtual bool FacingTarget();                                                     // vtable+0x98
	virtual void TurnToFaceTarget();                                                 // vtable+0x9c
	virtual bool OnLift(tCoord3d& p_liftPosition);                                   // vtable+0xa4
	virtual bool OnLift(tCoord3d& p_liftMin, tCoord3d& p_liftMax);                   // vtable+0xa0
	virtual void OffLift(tCoord3d& p_liftPosition);                                  // vtable+0xac
	virtual void OffLift(tCoord3d& p_liftMin, tCoord3d& p_liftMax);                  // vtable+0xa8
	virtual bool PossiblyOnLift();                                                   // vtable+0xb0
	virtual bool HasObject(eObjectType p_objectType);                                // vtable+0xb4
	virtual bool AddObject(eObjectType p_objectType, class CGameObject* p_object);   // vtable+0xb8
	virtual void ReNumberNode(int p_arg0, int p_arg1);                               // vtable+0xbc
	virtual bool NeedsNode(int p_arg0);                                              // vtable+0xc0
	virtual void ConvertVer0ToVer1();                                                // vtable+0xc4
	virtual void Delete();                                                           // vtable+0xc8
	virtual void PickUpAmmo(unsigned short p_amount);                                // vtable+0xcc
	virtual void ExternalControlEnd();                                               // vtable+0xd0
	virtual void RequestBalloon();                                                   // vtable+0xd4
	virtual void StartBalloon();                                                     // vtable+0xd8
	virtual void OnBalloon();                                                        // vtable+0xdc
	virtual int QOnBalloon();                                                        // vtable+0xe0
	virtual void OnConveyor(unsigned int p_arg0, CIce* p_arg1, unsigned int p_arg2); // vtable+0xe8
	virtual int OnConveyor();                                                        // vtable+0xe4
	virtual CIce* Conveyor();                                                        // vtable+0xec
	virtual void StartLand();                                                        // vtable+0xf0
	virtual void SendRemove();                                                       // vtable+0xf4
	virtual void SendCancel();                                                       // vtable+0xf8
	virtual int UsableState();                                                       // vtable+0xfc
	virtual bool IsUsable(eAction p_action);                                         // vtable+0x100
	virtual void Restart();                                                          // vtable+0x104
	void AddDestination(const AICOORD& p_destination);
	void AlterDestination(const AICOORD& p_destination);
	void DeleteFirstEntryFromDestinationList();
	void EmptyDestinationList();
	static void Init(CAI* p_ai);
	void Initialise();
	void ReSetId();
	void RegisterId();
	void RotateAnticlockwise();
	void RotateClockwise();
	void SetId(unsigned short p_id);
	void StartMoving();
	void StartSommersault();
	void StopMoving();
	void TurnToFaceDestination();
	void UpdateCollision();
	CGameObject();

	friend class CGameObjectMess;
	friend class CLift;
	friend class CBalloonPost;
	friend class CBullet;
	friend class CBulletManager;
	friend class CEnemy;
	friend class CObjectManager;
	friend class CPlayerLemming;
	friend class CGlobalGameObject;
	friend class CObjectChangeStateMess;
	friend class CObjectDiesMess;
	friend class CObjectHitMess;
	friend class CObjectPosMess;
	friend class CRemoveObjectMess;
	friend class CRequestActionMess;
	friend class CRequestCancelMess;
	friend class CRequestReplyMess;
	friend class CGenericGroup;
	friend class CSheepGroup;
	friend class CPlayerLemmingGroup;
	friend void StateMachine(tStateEntry** p_stateTables, CAI* p_ai, CGameObject* p_object);

	friend bool AtDestination(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool IsStuck(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool RequestDeath(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool IsJumping(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool IsFalling(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool NotTimeUp(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool PlayerWaitingToFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend bool EnemyWaitingToFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info);
	friend class C2D;
	friend class CPanelButton;
	friend class CTower;
	friend class CCatapult;
	friend class CAmmo;
	friend class CDuplicator;
	friend class CHand;
	friend class CHandManager;
	friend class CKey;
	friend class CRocket;
	friend class CBalloon;
	friend class CCollectable;
	friend class CCollectableManager;
	friend class CCrate;
	friend class CAI;
	friend class CInvisibleSwitch;
	friend class CLaser;
	friend class CMover;
	friend class CPlayerLemmingGroupManager;
	friend class CTrampoline;
	friend class CIce;
	friend void ReindexAllObjects();

protected:
	unsigned short m_runtimeFlags;         // 0x04
	unsigned int m_routeSearchFailed;      // 0x08
	unsigned int m_routeSearchActive;      // 0x0c
	unsigned int m_transientFlags;         // 0x10
	CRect3 m_collisionBounds;              // 0x14
	unsigned int m_deathRequested;         // 0x2c
	unsigned int m_balloonPostActive;      // 0x30
	unsigned int m_balloonPostId;          // 0x34
	int m_objectActive;                    // 0x38
	unsigned int m_initiallyActive;        // 0x3c
	AICOORD m_spawnPosition;               // 0x40
	C3DVector m_flightVelocity;            // 0x4c
	unsigned int m_unk0x58;                // 0x58
	CGameObject* m_activator;              // 0x5c
	CBaseObjectManager* m_manager;         // 0x60
	eObjectType m_objectType;              // 0x64
	unsigned short m_collisionFlags;       // 0x68
	unsigned short m_objectId;             // 0x6a
	unsigned short m_linkedObjectId;       // 0x6c
	short m_desiredFacingDirection;        // 0x6e
	CAiDestinationList* m_destinationList; // 0x70
	unsigned int m_hasDestination;         // 0x74
	CVMovePos m_movement;                  // 0x78
	int m_moveDurationTicks;               // 0x88
	unsigned int m_activationReserved;     // 0x8c
	unsigned int m_activatorObjectType;    // 0x90
	unsigned int m_stateTimer;             // 0x94
	eSoundEffect m_soundEffect;            // 0x98
	AICOORD m_position;                    // 0x9c
	AICOORD m_destination;                 // 0xa8
	short m_facingDirection;               // 0xb4
	short m_initialFacingDirection;        // 0xb6
	eAction m_action;                      // 0xb8
	short m_actionArgument;                // 0xbc
	unsigned int m_hidden;                 // 0xc0
	short m_unk0xc4;                       // 0xc4
	unsigned long m_lastMovementTick;      // 0xc8
	unsigned long m_actionDeadline;        // 0xcc
	unsigned long m_actionPhase1Deadline;  // 0xd0
	unsigned long m_actionPhase2Deadline;  // 0xd4
	C3DVector m_flightOrigin;              // 0xd8
	C3DVector m_auxiliaryPosition;         // 0xe4
	bool m_isFlying;                       // 0xf0
	AICOORD m_groundPosition;              // 0xf4
	int m_flightZ;                         // 0x100
	bool m_isJumping;                      // 0x104
	bool m_isFalling;                      // 0x108
	bool m_wasHitByMine;                   // 0x10c
	unsigned int m_liftId;                 // 0x110
	unsigned int m_isRemoteObject;         // 0x114
	unsigned int m_unk0x118;               // 0x118
	unsigned int m_onMover;                // 0x11c
	unsigned short m_invisibleSwitchId;    // 0x120
};

extern unsigned char g_abObjectIdBitmap[OBJECT_ID_BITMAP_BYTE_CAPACITY];
extern CGameObject* g_pObjects[OBJECT_REGISTRY_CAPACITY];
extern unsigned short g_wObjectCount;
extern int g_anTurnDelayCursor[16];
extern int g_anTurnDelayTarget[16];
extern unsigned char g_abBitMasks[OBJECT_ID_BITMAP_BITS_PER_BYTE];
extern unsigned short g_wNetworkLemmingIndex;
extern unsigned short g_wLocalLemmingIndex;
extern int g_wLemmingCount;
extern int g_dwSommersaultDirection;

// FUNCTION: LEMBALL 0x0040a6f0
inline void CGameObject::GetViewData(CViewData& p_viewData)
{
	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int argument = (unsigned short) m_actionArgument;
	unsigned int timer = m_stateTimer;
	p_viewData.m_action = m_action;
	p_viewData.m_actionArgument = argument;
	p_viewData.m_stateTimer = timer;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_hidden = m_hidden;
	p_viewData.m_auxiliaryPosition.m_xFixed = m_auxiliaryPosition.m_xFixed;
	p_viewData.m_auxiliaryPosition.m_yFixed = m_auxiliaryPosition.m_yFixed;
	p_viewData.m_auxiliaryPosition.m_zFixed = m_auxiliaryPosition.m_zFixed;
	p_viewData.m_soundEffect = m_soundEffect;
	unsigned long timestamp;
	if (m_isRemoteObject != 0) {
		timestamp = g_dwNetworkSimulationTimestamp;
	}
	else {
		timestamp = g_dwSimulationTimestamp;
	}
	p_viewData.m_animationTime = timestamp;
	SetSndEffect(SFX_NONE);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
}

// SYNTHETIC: LEMBALL 0x00417ad0
// CGameObject::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x0042fc60
// CGameObject::operator=

#endif
