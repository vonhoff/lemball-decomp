#include "CBullet.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Map/Ground/CGround.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Network/CConnect.h"
#include "../Managers/CBaseObjectManager.h"
#include "../Managers/CBulletManager.h"
#include "../Messages/GameMessageIds.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/CMove3d.h"
#include "AI/Base/CPt3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Diagnostics/VsDebug.h"
#include "Visos/Network/Protocol/CNetworkMessage.h"

enum {
	BULLET_TRAVEL_DURATION_TICKS = 10
};

enum {
	BULLET_NETWORK_STATE_PAYLOAD_SIZE_BYTES = 40
};

// FUNCTION: LEMBALL 0x0041a510
CBullet::CBullet()
	: CGlobalGameObject(OBJECT_BULLET, GAME_OBJECT_COLLISION_STEP_ON_INVISIBLE_SWITCHES, 0), m_unk0x174(DEBUG_SENTINEL),
	  m_unk0x178(DEBUG_SENTINEL), m_unk0x17c(DEBUG_SENTINEL), m_unk0x180(DEBUG_SENTINEL)
{
	m_payloadCapacity += BULLET_NETWORK_STATE_PAYLOAD_SIZE_BYTES;
}

// FUNCTION: LEMBALL 0x0041a5a0
void CBullet::Restart()
{
	CGlobalGameObject::Restart();
	m_active = 0;
	m_action = ACTION_DEAD;
}

// FUNCTION: LEMBALL 0x0041a5c0
void CBullet::Set(unsigned short p_id,
				  eBulletType p_bulletType,
				  eOwner p_owner,
				  int p_sourceObjectId,
				  AICOORD p_start,
				  AICOORD p_target)
{
	m_bulletType = p_bulletType;
	m_owner = p_owner;
	m_position.m_xFixed = p_start.m_xFixed;
	m_position.m_yFixed = p_start.m_yFixed;
	m_position.m_zFixed = p_start.m_zFixed;
	m_destination.m_xFixed = p_target.m_xFixed;
	m_destination.m_yFixed = p_target.m_yFixed;
	m_active = 1;
	int targetX = p_target.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int targetY = p_target.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX = targetX >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = targetY >> GROUND_BLOCK_PIXEL_SHIFT;
	int width = g_pMap->m_ground.m_width;
	unsigned short z;
	if (targetX < 0 || targetY < 0 || width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		z = g_pMap->m_ground.m_ground[blockY * width + blockX].GetZ(targetX & GROUND_BLOCK_PIXEL_MASK,
																	targetY & GROUND_BLOCK_PIXEL_MASK);
	}
	m_sourceObjectId = p_id;
	m_destination.m_zFixed = (z + 12) << FIXED_POINT_FRACTION_BITS;
	m_facingDirection = (short) ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													  m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
													  m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS,
													  m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS);
}

// FUNCTION: LEMBALL 0x0041a6d0
void CBullet::TriggerBullet()
{
	CPt3 start;
	CPt3 end;
	start.m_x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	start.m_y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	start.m_z = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	end.m_x = m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	end.m_y = m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	end.m_z = m_destination.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	m_movement.Set(start, end, m_lastMovementTick, 12);
	m_action = ACTION_RUNNING;
	m_actionDeadline = m_lastMovementTick + BULLET_TRAVEL_DURATION_TICKS;
}

// FUNCTION: LEMBALL 0x0041a760
void CBullet::FireBullet()
{
	m_lastMovementTick = g_dwGameTick;
	TriggerBullet();
	if (g_pActiveConnection != NULL) {
		m_manager->Add(this);
	}
}

// FUNCTION: LEMBALL 0x0041a7a0
bool CBullet::Process()
{
	CAI* ai;
	CMap* map;
	CGameObject* candidate;
	unsigned int currentTick;
	if (m_isRemoteObject != 0) {
		currentTick = g_dwRemoteGameTick;
	}
	else {
		currentTick = g_dwGameTick;
	}
	switch (m_action) {
	default: {
		CPt3 pos;
		pos.m_x = 0;
		pos.m_y = 0;
		pos.m_z = 0;
		unsigned int tick = m_lastMovementTick;
		if ((int) currentTick >= (int) tick) {
			do {
				if (m_actionDeadline < tick) {
					return false;
				}
				m_movement.Position(pos, tick);
				if (pos.m_x < 0 || pos.m_x > MAP_COORDINATE_MAX - 1 || pos.m_y < 0 ||
					pos.m_y > MAP_COORDINATE_MAX - 1) {
					return false;
				}
				int tileX = pos.m_x / GROUND_BLOCK_PIXEL_SIZE;
				int tileY;
				int width;
				CMap* collisionMap;
				unsigned short collision;
				if (tileX < 0 || (tileY = pos.m_y / GROUND_BLOCK_PIXEL_SIZE) < 0 ||
					(width = (collisionMap = g_pMap)->m_ground.m_width) <= tileX ||
					g_pMap->m_ground.m_height <= tileY) {
					collision = GROUND_COLLISION_OUT_OF_BOUNDS;
				}
				else {
					collision = g_pMap->m_ground.m_ground[width * tileY + tileX].m_collision;
				}
				if ((collision & GROUND_COLLISION_BLOCKS_BULLETS) != 0) {
					return false;
				}
				if (m_isRemoteObject == 0) {
					unsigned short groundZ;
					map = g_pMap;
					int groundWidth;
					int blockX = pos.m_x >> GROUND_BLOCK_PIXEL_SHIFT;
					int blockY = pos.m_y >> GROUND_BLOCK_PIXEL_SHIFT;
					if (pos.m_x < 0 || pos.m_y < 0 || (groundWidth = map->m_ground.m_width) <= blockX ||
						map->m_ground.m_height <= blockY) {
						groundZ = 0;
					}
					else {
						int x = pos.m_x & GROUND_BLOCK_PIXEL_MASK;
						int y = pos.m_y & GROUND_BLOCK_PIXEL_MASK;
						groundZ = map->m_ground.m_ground[groundWidth * blockY + blockX].GetZ(x, y);
					}
					if (pos.m_z <= (int) groundZ) {
						m_position.m_xFixed = pos.m_x << FIXED_POINT_FRACTION_BITS;
						m_position.m_yFixed = pos.m_y << FIXED_POINT_FRACTION_BITS;
						m_position.m_zFixed = pos.m_z << FIXED_POINT_FRACTION_BITS;
						g_pAI->StepOn(m_position, this, m_collisionFlags);
						return false;
					}
				}
				ai = g_pAI;
				ai->m_collisionExclude = NULL;
				ai->m_collisionPoint = pos;
				ai->m_collisionIndex = 0;
				while (ai->m_collisionIndex < ai->m_objectCount) {
					CGameObject* object = ai->m_objects[ai->m_collisionIndex];
					if (object != ai->m_collisionExclude && object->Collision(ai->m_collisionPoint)) {
						candidate = ai->m_objects[ai->m_collisionIndex];
						ai->m_collisionIndex++;
						goto hitFound;
					}
					ai->m_collisionIndex++;
				}
				candidate = NULL;
			hitFound:
				CGameObject* hitObject = candidate;
				if (hitObject != NULL && (unsigned short) hitObject->GetId() != m_sourceObjectId) {
					if (m_owner != OWNER_REMOTE_PLAYER || hitObject->m_objectType == OBJECT_PLAYER_2) {
						hitObject->HitBullet(this);
					}
					return false;
				}
				tick++;
			} while ((int) currentTick >= (int) tick);
		}
		m_position.m_xFixed = pos.m_x << FIXED_POINT_FRACTION_BITS;
		m_position.m_yFixed = pos.m_y << FIXED_POINT_FRACTION_BITS;
		m_position.m_zFixed = pos.m_z << FIXED_POINT_FRACTION_BITS;
		m_lastMovementTick = currentTick;
		return true;
	}
	case ACTION_DEAD:
		return false;
	}
}

// FUNCTION: LEMBALL 0x0041aaa0
void CBullet::AddData()
{
	Add((unsigned short) MESSAGE_BULLET_STATE);
	Add(m_linkedObjectId);
	Add(g_dwSimulationTimestamp);
	Add((unsigned short) (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) (m_destination.m_zFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned short) m_facingDirection);
	Add((unsigned long) m_soundEffect);
	Add((unsigned long) m_lastMovementTick);
	Add((unsigned long) m_bulletType);
	Add((unsigned long) m_owner);
	Add(m_sourceObjectId);
}

// FUNCTION: LEMBALL 0x0041ab80
void CBullet::GetData()
{
	SetRemoteGameTimeReal(GetDWORD());
	const int x = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_position.m_xFixed = x;
	const int y = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_position.m_yFixed = y;
	const int z = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = z;
	const int destinationX = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_destination.m_xFixed = destinationX;
	const int destinationY = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_destination.m_yFixed = destinationY;
	const int destinationZ = (int) (short) GetWORD() << FIXED_POINT_FRACTION_BITS;
	m_destination.m_zFixed = destinationZ;
	m_facingDirection = (short) GetWORD();
	m_soundEffect = (eSoundEffect) GetDWORD();
	m_lastMovementTick = GetDWORD();
	m_bulletType = (eBulletType) GetDWORD();
	m_owner = (eOwner) GetDWORD();
	if (m_owner == OWNER_PLAYER) {
		m_owner = OWNER_REMOTE_PLAYER;
	}
	m_sourceObjectId = GetWORD();
	m_active = 1;
	m_isRemoteObject = 1;
}

// FUNCTION: LEMBALL 0x0041ac70
void CBullet::Free()
{
	m_active = 0;
	if (m_action != ACTION_DEAD) {
		Action(ACTION_DEAD);
	}
	m_isRemoteObject = 0;
}

// FUNCTION: LEMBALL 0x0041aca0
bool CBullet::Receive(unsigned short p_messageId, CNetworkMessage* p_message)
{
	switch ((int) p_messageId) {
	default:
		return CGlobalGameObject::Receive(p_messageId, p_message);
	case MESSAGE_BULLET_STATE:
		break;
	}
	if (CNetworkMessage::Set(p_message->m_readCursor)) {
		p_message->m_readCursor = m_readCursor;
	}
	((CBulletManager*) m_manager)->RequestRemoteBullet(this);
	TriggerBullet();
	return true;
}

// FUNCTION: LEMBALL 0x0041af00
void CBullet::DoActivate()
{
}
