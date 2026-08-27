#ifndef LEMBALL_AI_OBJECTS_PLAYERLEMMING_H
#define LEMBALL_AI_OBJECTS_PLAYERLEMMING_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type
#include "../Base/AiCoord.h"                      // complete type
#include "../Base/GlobalGameObject.h"             // complete type

// SIZE 0x22c
// VTABLE: LEMBALL 0x00493890
// VTABLE: LEMBALL 0x00493870 secondary NetworkMessage subobject
class PlayerLemming : public GlobalGameObject, public NetworkMessage {
public:
	PlayerLemming(int p_arg0, int p_arg1, int p_arg2, int p_arg3, unsigned int p_arg4, unsigned long p_arg5);
	virtual ~PlayerLemming() {}
	PlayerLemmingGroup* GetGroup();
	bool CheckSfx();
	int GetLastBalloon();
	int GetObject(int p_arg0);
	virtual void Action(eAction p_arg0, int p_arg1);                                  // vtable+0x04
	virtual void Action(eAction p_arg0);                                              // vtable+0x08
	virtual void GetViewData(ViewData& p_viewData);                                   // vtable+0x0c
	virtual bool Process();                                                           // vtable+0x14
	virtual void StartStanding();                                                     // vtable+0x30
	virtual int HitBullet(Bullet* p_bullet);                                          // vtable+0x54
	virtual void HitBall();                                                           // vtable+0x58
	virtual int IsHit();                                                              // vtable+0x5c
	virtual void GetHit();                                                            // vtable+0x60
	virtual void HitMine();                                                           // vtable+0x64
	virtual void Die();                                                               // vtable+0x68
	virtual bool IsSelectable();                                                      // vtable+0x70
	virtual bool FacingCursor();                                                      // vtable+0x74
	virtual void TurnToFaceCursor();                                                  // vtable+0x78
	virtual bool IsRequestingFire();                                                  // vtable+0x7c
	virtual int Bored();                                                              // vtable+0x80
	virtual void SetBored(unsigned long p_arg0);                                      // vtable+0x84
	virtual bool Fire();                                                              // vtable+0x88
	virtual void StartFiring();                                                       // vtable+0x8c
	virtual void EndFiring();                                                         // vtable+0x90
	virtual int RandomAction();                                                       // vtable+0x94
	virtual bool FacingTarget();                                                      // vtable+0x98
	virtual void TurnToFaceTarget();                                                  // vtable+0x9c
	virtual bool HasObject(eObjectType p_arg0);                                       // vtable+0xb4
	virtual bool AddObject(eObjectType p_arg0, GameObject* p_arg1);                   // vtable+0xb8
	virtual void PickUpAmmo(unsigned short p_arg0);                                   // vtable+0xcc
	virtual void ExternalControlEnd();                                                // vtable+0xd0
	virtual int RequestBalloon();                                                     // vtable+0xd4
	virtual void StartBalloon();                                                      // vtable+0xd8
	virtual void OnBalloon();                                                         // vtable+0xdc
	virtual int QOnBalloon();                                                         // vtable+0xe0
	virtual int OnConveyor();                                                         // vtable+0xe4
	virtual void OnConveyor(unsigned int p_arg0, Ice* p_arg1, unsigned int p_arg2);  // vtable+0xe8
	virtual Ice* Conveyor();                                                          // vtable+0xec
	virtual void Restart();                                                           // vtable+0x104
	void AddData();
	void GetData();
	void RemoveObject(eObjectType p_arg0);
	void RequestFire(int p_arg0, int p_arg1);
	void Resurrect(const AiCoord& p_arg0);
	void SetGroup(PlayerLemmingGroup* p_arg0);
	void SetGroup(unsigned int p_arg0);
	void SetGroupLeader(unsigned int p_arg0);

private:
	unsigned int m_sfxChanged;            // 0x164
	unsigned short m_playerIndex;         // 0x168
	eObjectType m_balloonObjectType;      // 0x16c
	unsigned int m_alternatePlayer;       // 0x170
	unsigned int m_spawnDelay;            // 0x174
	unsigned int m_wasHitByBullet;        // 0x178
	unsigned int m_isGroupLeader;         // 0x17c
	unsigned int m_groupIndex;            // 0x180
	eFireRequestState m_fireRequestState; // 0x184
	unsigned int m_onConveyor;            // 0x188
	Ice* m_ice;                           // 0x18c
	unsigned int m_boredDeadline;         // 0x190
	AiCoord m_networkPositionCache;       // 0x194
	short m_cachedFacingDirection;        // 0x1a0
	eAction m_cachedAction;               // 0x1a4
	short m_cachedActionArgument;         // 0x1a8
	unsigned int m_cachedStateTimer;      // 0x1ac
	eSoundEffect m_cachedSoundEffect;     // 0x1b0
	AiCoord m_fireTarget;                 // 0x1b4
	eObjectType m_inventoryTypes[12];     // 0x1c0
	GameObject* m_inventoryObjects[12];   // 0x1f0
	unsigned int m_inventoryCount;        // 0x220
	PlayerLemmingGroup* m_group;          // 0x224
	unsigned short m_ammoCount;           // 0x228
};

// 68K 0x1061c15c __dt__14CPlayerLemmingFv
// SYNTHETIC: LEMBALL 0x00410b20
// PlayerLemming::`scalar deleting destructor'


#endif

