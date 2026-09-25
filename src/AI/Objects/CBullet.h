#ifndef LEMBALL_AI_OBJECTS_CBULLET_H
#define LEMBALL_AI_OBJECTS_CBULLET_H

#include "../../Visos/Messaging/CNetworkMessage.h"
#include "../Base/AiCoord.h"
#include "../Base/CGlobalGameObject.h"
#include "../Base/CMove3d.h"

enum eBulletType {
	BULLET_TYPE_DEFAULT = 0
};

enum eOwner {
	OWNER_PLAYER = 0,
	OWNER_ENEMY = 1,
	OWNER_REMOTE_PLAYER = 2
};

// SIZE 0x1a4
// VTABLE: LEMBALL 0x004940f0 CGlobalGameObject
// VTABLE: LEMBALL 0x004940d0 CNetworkMessage
class CBullet : public CGlobalGameObject, public CNetworkMessage {
public:
	CBullet();
	bool Receive(unsigned short p_messageId, CNetworkMessage* p_message);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	void AddData();
	void FireBullet();
	void Free();
	void GetData();
	void Set(unsigned short p_id,
			 eBulletType p_bulletType,
			 eOwner p_owner,
			 int p_sourceObjectId,
			 AiCoord p_start,
			 AiCoord p_target);
	void TriggerBullet();

	friend class CPlayerLemming;
	friend class CBulletManager;
	friend class CEnemy;

private:
	unsigned int m_active;           // 0x164
	eBulletType m_bulletType;        // 0x168
	eOwner m_owner;                  // 0x16c
	unsigned short m_sourceObjectId; // 0x170
	unsigned short m_pad0x172;       // 0x172
	int m_unk0x174;                  // 0x174
	int m_unk0x178;                  // 0x178
	int m_unk0x17c;                  // 0x17c
	int m_unk0x180;                  // 0x180
	CMove3d m_movement;              // 0x184
	unsigned int m_unk0x1a0;         // 0x1a0
};

// SYNTHETIC: LEMBALL 0x00418330
// CBullet::`vector deleting destructor'

// SYNTHETIC: LEMBALL 0x004183d0
// CBullet::~CBullet

// SYNTHETIC: LEMBALL 0x0041af50
// ??_ECBullet@@WBDI@AEPAXI@Z

#endif
