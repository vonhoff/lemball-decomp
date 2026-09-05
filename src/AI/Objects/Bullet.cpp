#include "Bullet.h"

#include "../Messages/GameMessageIds.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Map/Ground/Ground.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../../Visos/Network/Connect.h"
#include "../Managers/BaseObjectManager.h"
#include "../Managers/BulletManager.h"
#include "../Navigation/Ai.h"

// 68K 0x10119804 __dt__7CBulletFv
Bullet::~Bullet()
{
}

// 68K 0x10604572 __ct__7CBulletFv
// FUNCTION: LEMBALL 0x0041a510
Bullet::Bullet() : GlobalGameObject((eObjectType) OBJECT_BULLET, 0x100, 0)
{
	m_unk0x174 = DEBUG_SENTINEL;
	m_unk0x178 = DEBUG_SENTINEL;
	m_unk0x17c = DEBUG_SENTINEL;
	m_unk0x180 = DEBUG_SENTINEL;
	m_payloadCapacity += 0x28;
}

// 68K 0x1060466a Restart__7CBulletFv
// FUNCTION: LEMBALL 0x0041a5a0
void Bullet::Restart()
{
	GlobalGameObject::Restart();
	m_active = 0;
	m_action = (eAction) 8;
}

// 68K 0x106046a4 Set__7CBulletFUs11eBulletType6eOwneri7AICOORD7AICOORD
// FUNCTION: LEMBALL 0x0041a5c0
void Bullet::Set(unsigned short p_id,
				 eBulletType p_bulletType,
				 eOwner p_owner,
				 int p_sourceObjectId,
				 AiCoord p_start,
				 AiCoord p_target)
{
	m_bulletType = p_bulletType;
	m_owner = p_owner;
	m_position.m_xFixed = p_start.m_xFixed;
	m_position.m_yFixed = p_start.m_yFixed;
	m_position.m_zFixed = p_start.m_zFixed;
	m_destination.m_xFixed = p_target.m_xFixed;
	m_destination.m_yFixed = p_target.m_yFixed;
	m_active = 1;
	int targetX = p_target.m_xFixed >> 12;
	int targetY = p_target.m_yFixed >> 12;
	int blockX = targetX >> 4;
	int blockY = targetY >> 4;
	int width = g_pMap->m_ground.m_width;
	unsigned short z;
	if (targetX < 0 || targetY < 0 || width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		z = g_pMap->m_ground.m_ground[blockY * width + blockX].GetZ(targetX & 0xf, targetY & 0xf);
	}
	m_sourceObjectId = p_id;
	m_destination.m_zFixed = (z + 12) << 12;
	m_facingDirection = (short) ReturnFacingDirection(m_position.m_xFixed >> 12,
													  m_position.m_yFixed >> 12,
													  m_destination.m_xFixed >> 12,
													  m_destination.m_yFixed >> 12);
}

// 68K 0x1060479e TriggerBullet__7CBulletFv
// FUNCTION: LEMBALL 0x0041a6d0
void Bullet::TriggerBullet()
{
	Pt3 start;
	Pt3 end;
	start.m_x = m_position.m_xFixed >> 12;
	start.m_y = m_position.m_yFixed >> 12;
	start.m_z = m_position.m_zFixed >> 12;
	end.m_x = m_destination.m_xFixed >> 12;
	end.m_y = m_destination.m_yFixed >> 12;
	end.m_z = m_destination.m_zFixed >> 12;
	m_movement.Set(start, end, m_lastMovementTick, 12);
	m_action = (eAction) 0x1b;
	m_actionDeadline = m_lastMovementTick + 10;
}

// 68K 0x1060483a FireBullet__7CBulletFv
// FUNCTION: LEMBALL 0x0041a760
void Bullet::FireBullet()
{
	m_lastMovementTick = g_dwGameTick;
	TriggerBullet();
	if (g_pActiveConnection != 0) {
		m_manager->Add(this);
	}
}

// 68K 0x1060489a Process__7CBulletFv
// FUNCTION: LEMBALL 0x0041a7a0
bool Bullet::Process()
{
	unsigned int currentTick = g_dwGameTick;
	if (m_isRemoteObject != 0) {
		currentTick = g_dwRemoteGameTick;
	}
	if (m_action == 8) {
		return 0;
	}
	Pt3 pos;
	pos.m_x = 0;
	pos.m_y = 0;
	pos.m_z = 0;
	unsigned int tick = m_lastMovementTick;
	if ((int) tick <= (int) currentTick) {
		do {
			if (m_actionDeadline < tick) {
				return 0;
			}
			m_movement.Position(pos, tick);
			if (pos.m_x < 0 || pos.m_x > 0x3ff || pos.m_y < 0 || pos.m_y > 0x3ff) {
				return 0;
			}
			int tileX = (pos.m_x + ((pos.m_x >> 31) & 0xf)) >> 4;
			int tileY = (pos.m_y + ((pos.m_y >> 31) & 0xf)) >> 4;
			int width = g_pMap->m_ground.m_width;
			int height = g_pMap->m_ground.m_height;
			unsigned short collision;
			if (tileX < 0 || tileY < 0 || width <= tileX || height <= tileY) {
				collision = 3;
			}
			else {
				collision = g_pMap->m_ground.m_ground[width * tileY + tileX].m_collision;
			}
			if ((collision & 2) != 0) {
				return 0;
			}
			if (m_isRemoteObject == 0) {
				unsigned short groundZ;
				if (pos.m_x < 0 || pos.m_y < 0 || width <= (pos.m_x >> 4) || height <= (pos.m_y >> 4)) {
					groundZ = 0;
				}
				else {
					groundZ = g_pMap->m_ground.GetZ(pos.m_x, pos.m_y);
				}
				if (pos.m_z <= (int) groundZ) {
					m_position.m_xFixed = pos.m_x << 12;
					m_position.m_yFixed = pos.m_y << 12;
					m_position.m_zFixed = pos.m_z << 12;
					g_pAI->StepOn(m_position, this, m_collisionFlags);
					return 0;
				}
			}
			GameObject* hitObject = 0;
			for (int i = 0; i < g_pAI->m_objectCount; i++) {
				GameObject* obj = g_pAI->m_objects[i];
				if (obj != this && obj->Collision(pos)) {
					hitObject = obj;
					break;
				}
			}
			if (hitObject != 0 && hitObject->GetId() != m_sourceObjectId) {
				if (m_owner != 2 || hitObject->m_objectType == 2) {
					hitObject->HitBullet(this);
				}
				return 0;
			}
			tick++;
		} while ((int) tick <= (int) currentTick);
	}
	m_position.m_xFixed = pos.m_x << 12;
	m_position.m_yFixed = pos.m_y << 12;
	m_position.m_zFixed = pos.m_z << 12;
	m_lastMovementTick = currentTick;
	return 1;
}

// 68K 0x10604b20 AddData__7CBulletFv
// FUNCTION: LEMBALL 0x0041aaa0
void Bullet::AddData()
{
	Add((unsigned short) MESSAGE_BULLET_STATE);
	Add(m_linkedObjectId);
	Add(g_dwSimulationTimestamp);
	Add((unsigned short) (m_position.m_xFixed >> 12));
	Add((unsigned short) (m_position.m_yFixed >> 12));
	Add((unsigned short) (m_position.m_zFixed >> 12));
	Add((unsigned short) (m_destination.m_xFixed >> 12));
	Add((unsigned short) (m_destination.m_yFixed >> 12));
	Add((unsigned short) (m_destination.m_zFixed >> 12));
	Add((unsigned short) m_facingDirection);
	Add((unsigned long) m_soundEffect);
	Add((unsigned long) m_lastMovementTick);
	Add((unsigned long) m_bulletType);
	Add((unsigned long) m_owner);
	Add(m_sourceObjectId);
}

// 68K 0x10604c4e GetData__7CBulletFv
// FUNCTION: LEMBALL 0x0041ab80
void Bullet::GetData()
{
	SetRemoteGameTimeReal(GetDword());
	m_position.m_xFixed = (int) (short) GetWord() << 12;
	m_position.m_yFixed = (int) (short) GetWord() << 12;
	m_position.m_zFixed = (int) (short) GetWord() << 12;
	m_destination.m_xFixed = (int) (short) GetWord() << 12;
	m_destination.m_yFixed = (int) (short) GetWord() << 12;
	m_destination.m_zFixed = (int) (short) GetWord() << 12;
	m_facingDirection = (short) GetWord();
	m_soundEffect = (eSoundEffect) GetDword();
	m_lastMovementTick = GetDword();
	m_bulletType = (eBulletType) GetDword();
	m_owner = (eOwner) GetDword();
	if (m_owner == 0) {
		m_owner = (eOwner) 2;
	}
	m_sourceObjectId = GetWord();
	m_active = 1;
	m_isRemoteObject = 1;
}

// 68K 0x10604dbe Free__7CBulletFv
// FUNCTION: LEMBALL 0x0041ac70
void Bullet::Free()
{
	m_active = 0;
	if (m_action != 8) {
		Action((eAction) 8);
	}
	m_isRemoteObject = 0;
}

// 68K 0x10604e14 Receive__7CBulletFUsP15CNetworkMessage
// FUNCTION: LEMBALL 0x0041aca0
bool Bullet::Receive(unsigned short p_messageId, NetworkMessage* p_message)
{
	int messageId = p_messageId;
	if (messageId != MESSAGE_BULLET_STATE) {
		return GlobalGameObject::Receive(messageId, p_message);
	}
	if (NetworkMessage::Set(p_message->m_readCursor)) {
		p_message->m_readCursor = m_readCursor;
	}
	((BulletManager*) m_manager)->RequestRemoteBullet(this);
	TriggerBullet();
	return 1;
}

// 68K 0x101197e2 DoActivate__7CBulletFv
// FUNCTION: LEMBALL 0x0041af00
void Bullet::DoActivate()
{
}
