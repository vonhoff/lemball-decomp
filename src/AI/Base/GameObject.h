#ifndef LEMBALL_SCAFFOLD_AI_BASE_GAMEOBJECT_H
#define LEMBALL_SCAFFOLD_AI_BASE_GAMEOBJECT_H

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
	virtual AiCoord ActivatePosition();                                               // vtable+0x2c
	virtual Ice* Conveyor();                                                          // vtable+0xec
	virtual bool Activate(struct GameObject* p_object);                               // vtable+0x18
	virtual bool AddObject(eObjectType p_arg0, struct GameObject* p_arg1);            // vtable+0xb8
	virtual bool Bored();                                                             // vtable+0x80
	virtual bool Collision(const Pt3& p_arg0);                                        // vtable+0x50
	virtual bool Collision(const Rect3& p_arg0);                                      // vtable+0x4c
	virtual bool FacingCursor();                                                      // vtable+0x74
	virtual bool FacingTarget();                                                      // vtable+0x98
	virtual bool Fire();                                                              // vtable+0x88
	virtual bool HasObject(eObjectType p_arg0);                                       // vtable+0xb4
	virtual bool IsFlying();                                                          // vtable+0x1c
	virtual bool IsHit();                                                             // vtable+0x5c
	virtual bool IsRequestingFire();                                                  // vtable+0x7c
	virtual bool IsSelectable();                                                      // vtable+0x70
	virtual bool IsUsable(eAction p_action);                                          // vtable+0x100
	virtual bool Move();                                                              // vtable+0x44
	virtual bool NeedsNode(int p_arg0);                                               // vtable+0xc0
	virtual bool OnConveyor();                                                        // vtable+0xe4
	virtual bool OnLift(Coord3d& p_arg0);                                             // vtable+0xa4
	virtual bool OnLift(Coord3d& p_arg0, Coord3d& p_arg1);                            // vtable+0xa0
	virtual bool PossiblyOnLift();                                                    // vtable+0xb0
	virtual bool Process();                                                           // vtable+0x14
	virtual bool QOnBalloon();                                                        // vtable+0xe0
	virtual bool SearchRoute();                                                       // vtable+0x40
	virtual bool StartRoute();                                                        // vtable+0x3c
	virtual int HitBullet(Bullet* p_bullet);                                          // vtable+0x54
	virtual int RandomAction();                                                       // vtable+0x94
	virtual int RequestBalloon();                                                     // vtable+0xd4
	virtual int UsableState();                                                        // vtable+0xfc
	virtual int Usage();                                                              // vtable+0x28
	virtual unsigned char GetSndEffect();                                             // vtable+0x38
	virtual unsigned int ConvertVer0ToVer1();                                         // vtable+0xc4
	virtual unsigned int StartStanding();                                             // vtable+0x30
	virtual void Action(eAction p_arg0);                                              // vtable+0x08
	virtual void Action(eAction p_arg0, int p_arg1);                                  // vtable+0x04
	virtual void Delete();                                                            // vtable+0xc8
	virtual void Die();                                                               // vtable+0x68
	virtual void EndFiring();                                                         // vtable+0x90
	virtual void ExternalControlEnd();                                                // vtable+0xd0
	virtual void Fly();                                                               // vtable+0x24
	virtual void ForgetObjectLink(unsigned short p_arg0);                             // vtable+0x10
	virtual void GetBoundingBox(VsRect& p_rect);                                      // vtable+0x48
	virtual void GetHit();                                                            // vtable+0x60
	virtual void GetViewData(ViewData& p_viewData);                                   // vtable+0x0c
	virtual void HitBall();                                                           // vtable+0x58
	virtual void HitMine();                                                           // vtable+0x64
	virtual void Land();                                                              // vtable+0x6c
	virtual void OffLift(Coord3d& p_arg0);                                            // vtable+0xac
	virtual void OffLift(Coord3d& p_arg0, Coord3d& p_arg1);                           // vtable+0xa8
	virtual void OnBalloon();                                                         // vtable+0xdc
	virtual void OnConveyor(unsigned char p_arg0, Ice* p_arg1, unsigned char p_arg2); // vtable+0xe8
	virtual void PickUpAmmo(unsigned short p_arg0);                                   // vtable+0xcc
	virtual void ReNumberNode(int p_arg0, int p_arg1);                                // vtable+0xbc
	virtual void Restart();                                                           // vtable+0x104
	virtual void SendCancel();                                                        // vtable+0xf8
	virtual void SendRemove();                                                        // vtable+0xf4
	virtual void SetBored(unsigned long p_arg0);                                      // vtable+0x84
	virtual void SetSndEffect(eSoundEffect p_arg0);                                   // vtable+0x34
	virtual void StartBalloon();                                                      // vtable+0xd8
	virtual void StartFiring();                                                       // vtable+0x8c
	virtual void StartFly(C3DVector& p_arg0, C3DVector* p_arg1);                      // vtable+0x20
	virtual void StartLand();                                                         // vtable+0xf0
	virtual void TurnToFaceCursor();                                                  // vtable+0x78
	virtual void TurnToFaceTarget();                                                  // vtable+0x9c
	virtual ~GameObject();                                                            // vtable+0x00
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

private:
	unsigned short m_runtimeFlags;        // 0x04
	unsigned int m_routeSearchFailed;     // 0x08
	unsigned int m_routeSearchActive;     // 0x0c
	int m_collisionMinX;                  // 0x14
	int m_collisionMinY;                  // 0x18
	int m_collisionMinZ;                  // 0x1c
	int m_collisionMaxX;                  // 0x20
	int m_collisionMaxY;                  // 0x24
	int m_collisionMaxZ;                  // 0x28
	AiCoord m_spawnPosition;              // 0x40
	C3DVector m_flightVelocity;           // 0x4c
	eObjectType m_objectType;             // 0x64
	unsigned short m_collisionFlags;      // 0x68
	unsigned short m_objectId;            // 0x6a
	unsigned short m_linkedObjectId;      // 0x6c
	AiDestinationList* m_destinationList; // 0x70
	unsigned int m_hasDestination;        // 0x74
	int m_moveStartXFixed;                // 0x78
	int m_moveStartYFixed;                // 0x7c
	int m_moveDeltaXFixed;                // 0x80
	int m_moveDeltaYFixed;                // 0x84
	int m_moveDurationTicks;              // 0x88
	unsigned int m_stateTimer;            // 0x94
	eSoundEffect m_soundEffect;           // 0x98
	AiCoord m_position;                   // 0x9c
	AiCoord m_destination;                // 0xa8
	short m_facingDirection;              // 0xb4
	short m_initialFacingDirection;       // 0xb6
	eAction m_action;                     // 0xb8
	short m_actionArgument;               // 0xbc
	unsigned int m_lastMovementTick;      // 0xc8
	unsigned int m_actionDeadline;        // 0xcc
	C3DVector m_flightOrigin;             // 0xd8
	unsigned int m_isFlying;              // 0xf0
};

#endif
