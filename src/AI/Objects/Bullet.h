#ifndef LEMBALL_AI_OBJECTS_BULLET_H
#define LEMBALL_AI_OBJECTS_BULLET_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type
#include "../Base/AiCoord.h"                      // complete type
#include "../Base/GlobalGameObject.h"             // complete type
#include "../Base/Move3d.h"                       // complete type

// SIZE 0x1a4
// VTABLE: LEMBALL 0x004940f0
// VTABLE: LEMBALL 0x004940d0 secondary NetworkMessage subobject
class Bullet : public GlobalGameObject, public NetworkMessage {
public:
	Bullet();
	bool Receive(unsigned short p_messageId, NetworkMessage* p_message);
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~Bullet();         // vtable+0x00
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

	friend class PlayerLemming;

private:
	unsigned int m_active;           // 0x164
	eBulletType m_bulletType;        // 0x168
	eOwner m_owner;                  // 0x16c
	unsigned short m_sourceObjectId; // 0x170
	undefined2 m_pad0x172;           // 0x172
	int m_unk0x174;                  // 0x174
	int m_unk0x178;                  // 0x178
	int m_unk0x17c;                  // 0x17c
	int m_unk0x180;                  // 0x180
	Move3d m_movement;               // 0x184
};

#endif
