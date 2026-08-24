#ifndef LEMBALL_AI_BASE_GAMEOBJECT_H
#define LEMBALL_AI_BASE_GAMEOBJECT_H

#include "../../Common.h"
#include "AiCoord.h"   // complete type
#include "C3DVector.h" // complete type

// SIZE 0x124
// VTABLE: LEMBALL 0x00493c40
class GameObject {
public:
	AiCoord Position();
	AiCoord* GetDestination();
	GameObject(eObjectType p_arg0, unsigned short p_arg1, unsigned short p_arg2);
	bool DestinationExists();
	bool FacingDestination();
	bool Fall();
	bool Jump();
	bool ResetInstructions();
	int NextId();
	int NextLoadingId();
	short GetId();
	unsigned short Blocked();
	unsigned short MapCheck(int p_arg0, int p_arg1);
	virtual ~GameObject();                                                            // vtable+0x00
	virtual void Action(eAction p_arg0);                                              // vtable+0x08
	virtual void Action(eAction p_arg0, int p_arg1);                                  // vtable+0x04
	virtual void GetViewData(ViewData& p_viewData);                                   // vtable+0x0c
	virtual void ForgetObjectLink(unsigned short p_arg0);                             // vtable+0x10
	virtual bool Process();                                                           // vtable+0x14
	virtual bool Activate(class GameObject* p_object);                                // vtable+0x18
	virtual bool IsFlying();                                                          // vtable+0x1c
	virtual void StartFly(C3DVector& p_arg0, C3DVector* p_arg1);                      // vtable+0x20
	virtual void Fly();                                                               // vtable+0x24
	virtual int Usage();                                                              // vtable+0x28
	virtual AiCoord ActivatePosition();                                               // vtable+0x2c
	virtual unsigned int StartStanding();                                             // vtable+0x30
	virtual void SetSndEffect(eSoundEffect p_arg0);                                   // vtable+0x34
	virtual unsigned char GetSndEffect();                                             // vtable+0x38
	virtual bool StartRoute();                                                        // vtable+0x3c
	virtual bool SearchRoute();                                                       // vtable+0x40
	virtual bool Move();                                                              // vtable+0x44
	virtual void GetBoundingBox(VsRect& p_rect);                                      // vtable+0x48
	virtual bool Collision(const Rect3& p_arg0);                                      // vtable+0x4c
	virtual bool Collision(const Pt3& p_arg0);                                        // vtable+0x50
	virtual int HitBullet(Bullet* p_bullet);                                          // vtable+0x54
	virtual void HitBall();                                                           // vtable+0x58
	virtual bool IsHit();                                                             // vtable+0x5c
	virtual void GetHit();                                                            // vtable+0x60
	virtual void HitMine();                                                           // vtable+0x64
	virtual void Die();                                                               // vtable+0x68
	virtual void Land();                                                              // vtable+0x6c
	virtual bool IsSelectable();                                                      // vtable+0x70
	virtual bool FacingCursor();                                                      // vtable+0x74
	virtual void TurnToFaceCursor();                                                  // vtable+0x78
	virtual bool IsRequestingFire();                                                  // vtable+0x7c
	virtual bool Bored();                                                             // vtable+0x80
	virtual void SetBored(unsigned long p_arg0);                                      // vtable+0x84
	virtual bool Fire();                                                              // vtable+0x88
	virtual void StartFiring();                                                       // vtable+0x8c
	virtual void EndFiring();                                                         // vtable+0x90
	virtual int RandomAction();                                                       // vtable+0x94
	virtual bool FacingTarget();                                                      // vtable+0x98
	virtual void TurnToFaceTarget();                                                  // vtable+0x9c
	virtual bool OnLift(Coord3d& p_arg0, Coord3d& p_arg1);                            // vtable+0xa0
	virtual bool OnLift(Coord3d& p_arg0);                                             // vtable+0xa4
	virtual void OffLift(Coord3d& p_arg0, Coord3d& p_arg1);                           // vtable+0xa8
	virtual void OffLift(Coord3d& p_arg0);                                            // vtable+0xac
	virtual bool PossiblyOnLift();                                                    // vtable+0xb0
	virtual bool HasObject(eObjectType p_arg0);                                       // vtable+0xb4
	virtual bool AddObject(eObjectType p_arg0, class GameObject* p_arg1);             // vtable+0xb8
	virtual void ReNumberNode(int p_arg0, int p_arg1);                                // vtable+0xbc
	virtual bool NeedsNode(int p_arg0);                                               // vtable+0xc0
	virtual unsigned int ConvertVer0ToVer1();                                         // vtable+0xc4
	virtual void Delete();                                                            // vtable+0xc8
	virtual void PickUpAmmo(unsigned short p_arg0);                                   // vtable+0xcc
	virtual void ExternalControlEnd();                                                // vtable+0xd0
	virtual int RequestBalloon();                                                     // vtable+0xd4
	virtual void StartBalloon();                                                      // vtable+0xd8
	virtual void OnBalloon();                                                         // vtable+0xdc
	virtual bool QOnBalloon();                                                        // vtable+0xe0
	virtual bool OnConveyor();                                                        // vtable+0xe4
	virtual void OnConveyor(unsigned char p_arg0, Ice* p_arg1, unsigned char p_arg2); // vtable+0xe8
	virtual Ice* Conveyor();                                                          // vtable+0xec
	virtual void StartLand();                                                         // vtable+0xf0
	virtual void SendRemove();                                                        // vtable+0xf4
	virtual void SendCancel();                                                        // vtable+0xf8
	virtual int UsableState();                                                        // vtable+0xfc
	virtual bool IsUsable(eAction p_action);                                          // vtable+0x100
	virtual void Restart();                                                           // vtable+0x104
	void AddDestination(const AiCoord& p_arg0);
	void AlterDestination(const AiCoord& p_arg0);
	void DeleteFirstEntryFromDestinationList();
	void EmptyDestinationList();
	void Init(Ai* p_arg0);
	void Initialise();
	void ReSetId();
	void RegisterId();
	void RotateAnticlockwise();
	void RotateClockwise();
	void SetId(unsigned short p_arg0);
	void StartMoving();
	void StartSommersault();
	void StopMoving();
	void TurnToFaceDestination();
	void UpdateCollision();
	GameObject();

	friend class GameObjectMess;
	friend class ObjectChangeStateMess;
	friend class ObjectDiesMess;
	friend class ObjectHitMess;
	friend class ObjectPosMess;
	friend class RemoveObjectMess;
	friend class RequestActionMess;
	friend class RequestCancelMess;
	friend class RequestReplyMess;

private:
	unsigned short m_runtimeFlags;        // 0x04
	undefined2 m_unk0x06;                 // 0x06
	unsigned int m_routeSearchFailed;     // 0x08
	unsigned int m_routeSearchActive;     // 0x0c
	undefined4 m_unk0x10;                 // 0x10
	int m_collisionMinX;                  // 0x14
	int m_collisionMinY;                  // 0x18
	int m_collisionMinZ;                  // 0x1c
	int m_collisionMaxX;                  // 0x20
	int m_collisionMaxY;                  // 0x24
	int m_collisionMaxZ;                  // 0x28
	undefined4 m_unk0x2c[3];              // 0x2c
	int m_heading;                        // 0x38
	undefined4 m_unk0x3c;                 // 0x3c
	AiCoord m_spawnPosition;              // 0x40
	C3DVector m_flightVelocity;           // 0x4c
	undefined4 m_unk0x58[2];              // 0x58
	BaseObjectManager* m_manager;         // 0x60
	eObjectType m_objectType;             // 0x64
	unsigned short m_collisionFlags;      // 0x68
	unsigned short m_objectId;            // 0x6a
	unsigned short m_linkedObjectId;      // 0x6c
	undefined2 m_unk0x6e;                 // 0x6e
	AiDestinationList* m_destinationList; // 0x70
	unsigned int m_hasDestination;        // 0x74
	int m_moveStartXFixed;                // 0x78
	int m_moveStartYFixed;                // 0x7c
	int m_moveDeltaXFixed;                // 0x80
	int m_moveDeltaYFixed;                // 0x84
	int m_moveDurationTicks;              // 0x88
	undefined4 m_unk0x8c;                 // 0x8c
	undefined4 m_unk0x90;                 // 0x90
	unsigned int m_stateTimer;            // 0x94
	eSoundEffect m_soundEffect;           // 0x98
	AiCoord m_position;                   // 0x9c
	AiCoord m_destination;                // 0xa8
	short m_facingDirection;              // 0xb4
	short m_initialFacingDirection;       // 0xb6
	eAction m_action;                     // 0xb8
	short m_actionArgument;               // 0xbc
	undefined2 m_unk0xbe;                 // 0xbe
	undefined4 m_unk0xc0[2];              // 0xc0
	unsigned int m_lastMovementTick;      // 0xc8
	unsigned int m_actionDeadline;        // 0xcc
	undefined4 m_unk0xd0[2];              // 0xd0
	C3DVector m_flightOrigin;             // 0xd8
	undefined4 m_unk0xe4[3];              // 0xe4
	unsigned int m_isFlying;              // 0xf0
	undefined4 m_unk0xf4[8];              // 0xf4
	undefined4 m_unk0x114;                // 0x114
	undefined4 m_unk0x118[3];             // 0x118
};

extern unsigned char g_abObjectIdBitmap[32];
extern GameObject* g_pObjects[256];
extern word g_wObjectCount;
extern unsigned char g_abBitMasks[8];
extern word g_wNetworkLemmingIndex;
extern word g_wLocalLemmingIndex;
extern word g_wLemmingCount;
#endif
