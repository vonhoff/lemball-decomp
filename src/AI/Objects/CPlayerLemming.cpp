#include "CPlayerLemming.h"

#include "../../Control/Game/CDemo.h"
#include "../../Control/Game/CGame.h"
#include "../../Control/Game/CGameStatus.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Map/Ground/CGround.h"
#include "../../Map/Ground/CGroundArray.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CBaseGlobalObject.h"
#include "../Base/CGlobalGameObject.h"
#include "../Base/StateMachine.h"
#include "../Groups/CPlayerLemmingGroup.h"
#include "../Groups/CPlayerLemmingGroupManager.h"
#include "../Messages/CObjectHitMess.h"
#include "../Navigation/CAI.h"
#include "../Navigation/CAICursor.h"
#include "../Navigation/CAiDestinationList.h"
#include "../Navigation/CMover.h"
#include "../Objects/CBalloonPost.h"
#include "../Objects/CBullet.h"
#include "../Objects/CIce.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Messages/GameMessageIds.h"
#include "CViewData.h"
#include "Visos/Foundation/VsDebug.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0040ecb0
CPlayerLemming::CPlayerLemming(int p_x,
							   int p_y,
							   int p_z,
							   int p_facing,
							   unsigned int p_alternatePlayer,
							   unsigned long p_spawnDelay)
	: CGlobalGameObject(p_alternatePlayer ? OBJECT_PLAYER_1 : OBJECT_PLAYER_2, 0x17f, 0x14)
{
	m_alternatePlayer = p_alternatePlayer;
	m_spawnPosition.m_xFixed = p_x << 12;
	m_spawnPosition.m_yFixed = p_y << 12;
	m_spawnPosition.m_zFixed = p_z << 12;
	m_spawnDelay = p_spawnDelay;
	m_initialFacingDirection = (short) p_facing;
	if (p_alternatePlayer != 0) {
		SetId(NextLoadingId());
	}
	else {
		m_payloadCapacity += 0x11;
	}
}

// FUNCTION: LEMBALL 0x0040ed90
void CPlayerLemming::Restart()
{
	CGlobalGameObject::Restart();
	if (m_alternatePlayer == 0) {
		m_playerIndex = g_wNetworkLemmingIndex;
		g_wNetworkLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex] = this;
		g_wLemmingCount++;
		m_action = ACTION_12;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = m_spawnDelay + g_dwGameTick;
		int tileX = m_spawnPosition.m_xFixed >> 12;
		int tileY = m_spawnPosition.m_yFixed >> 12;
		int tileZ = m_spawnPosition.m_zFixed >> 12;
		int collision[6];
		collision[0] = tileX - 8;
		collision[1] = tileY - 8;
		collision[2] = tileZ;
		collision[3] = tileX + 7;
		collision[4] = tileY + 7;
		collision[5] = tileZ + 15;
		memcpy(&m_collisionBounds, collision, sizeof(collision));
		int& objectCount = g_pAI->m_objectCount;
		g_pAI->m_objects[objectCount] = this;
		objectCount++;
		m_flightVelocity.m_xFixed = 0x3000;
		m_flightVelocity.m_yFixed = 0;
		m_flightVelocity.m_zFixed = 0xa000;
		m_isGroupLeader = 0;
		m_wasHitByBullet = 0;
		m_hasDestination = 0;
		m_fireRequestState = FIRE_REQUEST_NONE;
		m_desiredFacingDirection = m_initialFacingDirection;
		SetBored(4000);
		AiCoord dest;
		dest.m_xFixed = ((4 - m_playerIndex) * 16 + tileX) << 12;
		dest.m_yFixed = tileY << 12;
		dest.m_zFixed = tileZ << 12;
		AddDestination(dest);
		m_position.m_xFixed = m_spawnPosition.m_xFixed;
		m_position.m_yFixed = m_spawnPosition.m_yFixed;
		m_position.m_zFixed = m_spawnPosition.m_zFixed;
		m_deathRequested = 0;
		m_sfxChanged = 1;
		m_group = 0;
		m_ice = 0;
		m_onConveyor = 0;
		m_ammoCount = PLAYER_START_AMMO;
	}
	else {
		m_playerIndex = g_wLocalLemmingIndex;
		g_wLocalLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex + 4] = this;
		m_isRemoteObject = 1;
		m_action = ACTION_8;
	}
	m_position.m_zFixed += 0x44000;
	m_facingDirection = m_initialFacingDirection;
	m_inventoryCount = 0;
}

// FUNCTION: LEMBALL 0x0040efd0
CPlayerLemming::~CPlayerLemming()
{
}

// FUNCTION: LEMBALL 0x0040f000
void CPlayerLemming::HitBullet(CBullet* p_bullet)
{
	if (g_pGameStatus->m_status0 == 0) {
		if ((int) m_action < 4 || ((int) m_action > 5 && m_action != 16)) {
			switch (p_bullet->m_owner) {
			case OWNER_ENEMY: {
				int randVal = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
				*g_pSentinel = randVal;
				if (randVal % 2) {
					return;
				}
				break;
			}
			case OWNER_REMOTE_PLAYER:
				if (g_pActiveConnection != 0) {
					g_pObjectHitMessage->Send(p_bullet);
				}
				break;
			default:
				return;
			}
			m_unk0xc0 = 0;
			m_wasHitByBullet = 1;
			m_actionDeadline = g_dwGameTick + 40;
			m_facingDirection = (p_bullet->m_facingDirection + 4) & 7;
		}
	}
}

// FUNCTION: LEMBALL 0x0040f0d0
void CPlayerLemming::SetGroup(CPlayerLemmingGroup* p_group)
{
	m_group = p_group;
}

// FUNCTION: LEMBALL 0x0040f0e0
CPlayerLemmingGroup* CPlayerLemming::GetGroup()
{
	return m_group;
}

// FUNCTION: LEMBALL 0x0040f0f0
void CPlayerLemming::SetGroupLeader(unsigned int p_isLeader)
{
	m_isGroupLeader = p_isLeader;
}

// FUNCTION: LEMBALL 0x0040f100
void CPlayerLemming::SetGroup(unsigned int p_groupIndex)
{
	m_groupIndex = p_groupIndex;
}

// FUNCTION: LEMBALL 0x0040f110
unsigned int CPlayerLemming::IsGroupLeader()
{
	return m_isGroupLeader;
}

// FUNCTION: LEMBALL 0x0040f120
bool CPlayerLemming::Process()
{
	if (m_isRemoteObject == 0) {
		if (g_pAI->IsLemmingPlayerControlled(this)) {
			UserLemming(g_pAI, this);
			return false;
		}
		AIPlayerLemming(g_pAI, this);
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040f160
void CPlayerLemming::TurnToFaceCursor()
{
	if (g_pDemo->m_demoMode == 0) {
		int cursorX;
		int cursorY;
		g_pAI->m_cursor->GetCursorSurfaceCoordinates(cursorX, cursorY);
		unsigned int facing =
			ReturnFacingDirection(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, cursorX, cursorY);
		if (facing != (unsigned int) m_facingDirection) {
			if (g_anRotationDirections[(facing - m_facingDirection) & 7] < 0) {
				RotateAnticlockwise();
			}
			else {
				RotateClockwise();
			}
			SetBored(4000);
		}
		m_actionDeadline = g_dwGameTick + g_anTurnDelayCursor[m_objectType] / GAME_TICK_MILLISECONDS;
	}
}

// FUNCTION: LEMBALL 0x0040f220
void CPlayerLemming::TurnToFaceTarget()
{
	int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
									   m_position.m_yFixed >> 12,
									   m_fireTarget.m_xFixed >> 12,
									   m_fireTarget.m_yFixed >> 12);
	if (facing != (int) m_facingDirection) {
		if (g_anRotationDirections[(facing - m_facingDirection) & 7] < 0) {
			RotateAnticlockwise();
		}
		else {
			RotateClockwise();
		}
		SetBored(4000);
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / GAME_TICK_MILLISECONDS;
}

// FUNCTION: LEMBALL 0x0040f2b0
bool CPlayerLemming::IsRequestingFire()
{
	return m_fireRequestState == 1;
}

// FUNCTION: LEMBALL 0x0040f2c0
void CPlayerLemming::RequestFire(int p_x, int p_y)
{
	if (m_fireRequestState == FIRE_REQUEST_NONE && (m_action == 0 || m_action == 2 || m_action == 6)) {
		m_fireRequestState = FIRE_REQUEST_PENDING;
		m_fireTarget.m_xFixed = p_x << 12;
		m_fireTarget.m_yFixed = p_y << 12;
	}
}

// FUNCTION: LEMBALL 0x0040f310
void CPlayerLemming::Fire()
{
	AiCoord start;
	start.m_xFixed = m_position.m_xFixed;
	int facing = m_facingDirection;
	start.m_yFixed = m_position.m_yFixed;
	start.m_zFixed = m_position.m_zFixed + 0xa000;
	switch (m_action) {
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 14:
	case 15:
		break;
	default:
		if (g_pGameStatus->m_status1 != 0 || m_ammoCount != 0) {
			SetBored(4000);
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_PLAYER, facing, start, m_fireTarget);
			m_soundEffect = SFX_GUN;
			if (g_pGameStatus->m_status1 == 0) {
				m_ammoCount--;
			}
		}
	}
	m_fireRequestState = FIRE_REQUEST_NONE;
}

// FUNCTION: LEMBALL 0x0040f410
void CPlayerLemming::StartFiring()
{
	m_actionDeadline = g_dwGameTick + 10;
}

// FUNCTION: LEMBALL 0x0040f420
void CPlayerLemming::EndFiring()
{
	m_fireRequestState = FIRE_REQUEST_NONE;
}

// FUNCTION: LEMBALL 0x0040f430
bool CPlayerLemming::FacingCursor()
{
	if (g_pDemo->m_demoMode != 0) {
		return true;
	}
	int cursorX;
	int cursorY;
	g_pAI->m_cursor->GetCursorSurfaceCoordinates(cursorX, cursorY);
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, cursorX, cursorY);
	return (int) m_facingDirection == (int) facing;
}

// FUNCTION: LEMBALL 0x0040f4b0
bool CPlayerLemming::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
												m_position.m_yFixed >> 12,
												m_fireTarget.m_xFixed >> 12,
												m_fireTarget.m_yFixed >> 12);
	return (int) m_facingDirection == (int) facing;
}

// FUNCTION: LEMBALL 0x0040f500
void CPlayerLemming::Die()
{
	int inventoryIndex = 0;
	if ((int) m_inventoryCount > 0) {
		CGameObject** inventoryObject = m_inventoryObjects;
		do {
			switch ((*inventoryObject)->m_objectType) {
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x27:
			case 0x29:
			case 0x2b:
			case 0x2d:
				((CBaseGlobalObject*) *inventoryObject)->OldRestart();
				break;
			}
			inventoryObject++;
			inventoryIndex++;
		} while (inventoryIndex < (int) m_inventoryCount);
	}
	int objectIndex = 0;
	int& objectCount = g_pAI->m_objectCount;
	if (objectIndex < objectCount) {
		CGameObject**& objectArray = g_pAI->m_objects;
		do {
			if (objectArray[objectIndex] == this) {
				objectCount--;
				while (objectIndex < objectCount) {
					objectArray[objectIndex] = objectArray[objectIndex + 1];
					objectIndex++;
				}
				objectArray[objectCount] = 0;
				break;
			}
			objectIndex++;
		} while (objectIndex < objectCount);
	}
	g_wLemmingCount--;
	if (g_wLemmingCount == 0) {
		g_pAI->GameState(GAME_STATUS_FAILURE);
	}
}

// FUNCTION: LEMBALL 0x0040f600
void CPlayerLemming::HitMine()
{
	C3DVector vel;
	vel.m_xFixed = 0;
	vel.m_yFixed = 0;
	m_wasHitByMine = 1;
	vel.m_zFixed = 0xa000;
	StartFly(vel, 0);
	m_deathRequested = 1;
}

// FUNCTION: LEMBALL 0x0040f640
void CPlayerLemming::GetData()
{
	unsigned short packedState[8];
	m_position.m_xFixed = (int) (unsigned int) GetWORD() << 12;
	m_position.m_yFixed = (int) (unsigned int) GetWORD() << 12;
	m_position.m_zFixed = (int) (unsigned int) GetWORD() << 12;
	Get(packedState[1]);
	m_facingDirection = packedState[1] & 7;
	m_actionArgument = (packedState[1] & 0x38) >> 3;
	Get(packedState[1]);
	m_action = (eAction) (packedState[1] & 0xff);
	m_soundEffect = (eSoundEffect) (packedState[1] >> 8);
	m_stateTimer = GetDWORD();
}

// FUNCTION: LEMBALL 0x0040f6f0
void CPlayerLemming::AddData()
{
	Add((unsigned short) MESSAGE_PLAYER_LEMMING_STATE);
	Add((unsigned char) m_playerIndex);
	Add((unsigned short) (m_position.m_xFixed >> 12));
	Add((unsigned short) (m_position.m_yFixed >> 12));
	Add((unsigned short) (m_position.m_zFixed >> 12));
	Add((unsigned short) (((m_actionArgument & 7) << 3) | (m_facingDirection & 7)));
	Add((unsigned short) ((m_soundEffect << 8) | (m_action & 0xff)));
	if (g_dwSimulationTimestamp < m_stateTimer) {
		m_stateTimer = g_dwSimulationTimestamp;
	}
	Add((unsigned long) m_stateTimer);
	m_sfxChanged = 0;
}

// FUNCTION: LEMBALL 0x0040f7a0
bool CPlayerLemming::CheckSFX()
{
	unsigned int* pSfx = &m_sfxChanged;
	eSoundEffect sfx = m_soundEffect;
	if (m_cachedSoundEffect == sfx && *pSfx == 0) {
		*pSfx = 0;
		m_cachedSoundEffect = sfx;
		return *pSfx;
	}
	m_cachedSoundEffect = sfx;
	*pSfx = 1;
	return *pSfx;
}

// FUNCTION: LEMBALL 0x0040f7e0
unsigned int CPlayerLemming::CheckNetworkStateChanged()
{
	int x = m_position.m_xFixed;
	m_sfxChanged = ((m_networkPositionCache.m_xFixed ^ x) & 0xfffff000) != 0 || m_sfxChanged;
	int y = m_position.m_yFixed;
	m_sfxChanged = ((m_networkPositionCache.m_yFixed ^ y) & 0xfffff000) != 0 || m_sfxChanged;
	int z = m_position.m_zFixed;
	m_sfxChanged = ((m_networkPositionCache.m_zFixed ^ z) & 0xfffff000) != 0 || m_sfxChanged;
	eAction action = m_action;
	switch (action) {
	case 0:
		m_sfxChanged = m_cachedAction != action || m_sfxChanged;
		m_cachedAction = action;
		break;
	case 1:
		break;
	default:
		m_sfxChanged = m_cachedAction != action || m_sfxChanged;
		m_sfxChanged = m_cachedActionArgument != m_actionArgument || m_sfxChanged;
		m_sfxChanged = m_cachedStateTimer != m_stateTimer || m_sfxChanged;
		m_cachedAction = action;
		break;
	}
	eSoundEffect sound = m_soundEffect;
	m_sfxChanged = m_cachedSoundEffect != sound || m_sfxChanged;
	m_cachedFacingDirection = m_facingDirection;
	m_networkPositionCache.m_xFixed = x;
	m_networkPositionCache.m_yFixed = y;
	m_networkPositionCache.m_zFixed = z;
	m_cachedSoundEffect = sound;
	m_cachedActionArgument = m_actionArgument;
	m_cachedStateTimer = m_stateTimer;
	return m_sfxChanged;
}

// FUNCTION: LEMBALL 0x0040f960
bool CPlayerLemming::HasObject(eObjectType p_objectType)
{
	if (p_objectType != OBJECT_AMMO) {
		int count = m_inventoryCount;
		if (count != PLAYER_INVENTORY_CAPACITY) {
			for (int i = 0; i < count; i++) {
				if (m_inventoryTypes[i] == p_objectType) {
					return 1;
				}
			}
		}
	}
	else if (m_ammoCount == PLAYER_MAX_AMMO) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040f9b0
bool CPlayerLemming::AddObject(eObjectType p_objectType, CGameObject* p_object)
{
	if (m_inventoryCount == PLAYER_INVENTORY_CAPACITY) {
		return 0;
	}
	if (HasObject(p_objectType)) {
		return 0;
	}
	m_inventoryTypes[m_inventoryCount] = p_objectType;
	m_inventoryObjects[m_inventoryCount] = p_object;
	m_inventoryCount++;
	return 1;
}

// FUNCTION: LEMBALL 0x0040fa10
void CPlayerLemming::RandomAction()
{
	int randVal = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	*g_pSentinel = randVal;
	int action = randVal % 3;
	m_actionArgument = (short) action;
	switch (action) {
	case 0:
		m_actionDeadline = g_dwGameTick + 0x38;
		break;
	case 1:
		m_actionDeadline = g_dwGameTick + 0x21;
		break;
	case 2:
		m_actionDeadline = g_dwGameTick + 0x26;
		break;
	}
}

// FUNCTION: LEMBALL 0x0040fa80
void CPlayerLemming::Resurrect(const AiCoord& p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_deathRequested = 0;
	g_wLemmingCount++;
	m_facingDirection = 0;
	m_inventoryCount = 0;
	m_action = ACTION_NONE;
	m_isGroupLeader = 0;
	m_wasHitByBullet = 0;
	m_ice = 0;
	m_onConveyor = 0;
	m_hasDestination = 0;
	short& resetFlags = m_unk0xc4;
	resetFlags = 0;
	m_fireRequestState = FIRE_REQUEST_NONE;
	m_isFlying = 0;
	m_unk0xc0 = 0;
	m_activationReserved = 0;
	m_routeSearchFailed = 0;
	m_routeSearchActive = 0;
	m_isJumping = 0;
	m_isFalling = 0;
	m_wasHitByMine = 0;
	m_liftId = 0xffff;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	unsigned short& switchId = m_invisibleSwitchId;
	switchId = 0xffff;
	m_flightVelocity.m_zFixed = 0;
	short& facing = m_desiredFacingDirection;
	facing = 0;
	m_unk0x58 = 0;
	m_onMover = 0;
	m_ammoCount = PLAYER_START_AMMO;
	SetBored(4000);
	int tileX = m_position.m_xFixed >> 12;
	int tileY = m_position.m_yFixed >> 12;
	int tileZ = m_position.m_zFixed >> 12;
	int collision[6];
	collision[0] = tileX - 8;
	collision[1] = tileY - 8;
	collision[2] = tileZ;
	collision[3] = tileX + 7;
	collision[4] = tileY + 7;
	collision[5] = tileZ + 15;
	memcpy(&m_collisionBounds, collision, sizeof(collision));
	int& objectCount = g_pAI->m_objectCount;
	g_pAI->m_objects[objectCount] = this;
	objectCount++;
	m_destinationList->m_count = 0;
}

// FUNCTION: LEMBALL 0x0040fbe0
int CPlayerLemming::GetLastBalloon()
{
	if (m_inventoryCount == 0) {
		return 0xffff;
	}
	for (int i = m_inventoryCount - 1; i >= 0; i--) {
		switch (m_inventoryTypes[i]) {
		case 0x27:
			return 0x27;
		case 0x29:
			return 0x29;
		case 0x2b:
			return 0x2b;
		case 0x2d:
			return 0x2d;
		}
	}
	return 0xffff;
}

// FUNCTION: LEMBALL 0x0040fc50
void CPlayerLemming::RemoveObject(eObjectType p_objectType)
{
	for (int i = 0; i < (int) m_inventoryCount; i++) {
		if (m_inventoryTypes[i] == p_objectType) {
			for (int j = i + 1; j < (int) m_inventoryCount; j++) {
				m_inventoryTypes[j - 1] = m_inventoryTypes[j];
			}
			m_inventoryCount--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0040fcb0
int CPlayerLemming::GetObject(int p_index)
{
	if ((int) m_inventoryCount <= p_index) {
		return 0xffff;
	}
	return m_inventoryTypes[p_index];
}

// FUNCTION: LEMBALL 0x0040fcd0
void CPlayerLemming::ExternalControlEnd()
{
	int actionArgument = (unsigned short) m_actionArgument;
	switch (actionArgument) {
	case 1:
	case 2:
		Die();
		Action(ACTION_8);
		break;
	default:
		Action(ACTION_NONE);
		break;
	}
}

// FUNCTION: LEMBALL 0x0040fd10
void CPlayerLemming::OnBalloon()
{
	AiCoord postPos;
	postPos.m_xFixed = DEBUG_SENTINEL;
	postPos.m_yFixed = DEBUG_SENTINEL;
	postPos.m_zFixed = DEBUG_SENTINEL;
	g_pAI->m_balloonPost->FindPost(m_balloonObjectType, postPos);
	int dist =
		Distance(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, postPos.m_xFixed >> 12, postPos.m_yFixed >> 12);
	if (dist < 16) {
		m_balloonPostActive = 0;
		SetSndEffect(SFX_BALLOON_EXPLODE);
		m_isFalling = 1;
		m_actionArgument = 0;
		m_lastMovementTick = g_dwGameTick;
		m_action = ACTION_11;
		m_flightZ = m_position.m_zFixed >> 12;
		ResetInstructions();
		int posX = m_position.m_xFixed;
		int posY = m_position.m_yFixed;
		m_groundPosition.m_xFixed = posX;
		m_groundPosition.m_yFixed = posY;
		int tileY = posY >> 12;
		int tileX = posX >> 12;
		int blockY = tileY >> 4;
		int blockX = tileX >> 4;
		unsigned short groundZ;
		if (tileX < 0 || tileY < 0 || blockX >= g_pMap->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			int cellX = tileX & 0xf;
			int cellY = tileY & 0xf;
			groundZ = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(cellX, cellY);
		}
		m_groundPosition.m_zFixed = (int) (unsigned int) groundZ << 12;
		return;
	}
	int posY = m_position.m_yFixed;
	int posX = m_position.m_xFixed;
	int tileY = posY >> 12;
	int tileX = posX >> 12;
	int blockY = tileY >> 4;
	int blockX = tileX >> 4;
	unsigned short groundZ;
	if (tileX < 0 || tileY < 0 || blockX >= g_pMap->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
		groundZ = 0;
	}
	else {
		int cellX = tileX & 0xf;
		int cellY = tileY & 0xf;
		groundZ = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	int baseZ = (int) groundZ + 32;
	int curZ = m_position.m_zFixed;
	int tileZ = curZ >> 12;
	if (dist != 0) {
		int factor = (g_dwGameTick - m_lastMovementTick) * 2;
		m_position.m_xFixed += ((postPos.m_xFixed - m_position.m_xFixed) * factor) / dist;
		m_position.m_yFixed += ((postPos.m_yFixed - m_position.m_yFixed) * factor) / dist;
	}
	if (tileZ < baseZ - 6 || tileZ > baseZ + 6) {
		if (tileZ > baseZ) {
			curZ -= 0x2000;
		}
		else {
			curZ += 0x2000;
		}
		m_position.m_zFixed = curZ;
	}
	m_lastMovementTick = g_dwGameTick;
}

// FUNCTION: LEMBALL 0x0040ff60
void CPlayerLemming::StartBalloon()
{
}

// FUNCTION: LEMBALL 0x0040ff70
void CPlayerLemming::RequestBalloon()
{
	AiCoord postPos;
	postPos.m_xFixed = DEBUG_SENTINEL;
	postPos.m_yFixed = DEBUG_SENTINEL;
	postPos.m_zFixed = DEBUG_SENTINEL;
	int lastBalloon = GetLastBalloon();
	switch (lastBalloon) {
	case 0xffff:
		m_balloonPostActive = 0;
		return;
	case OBJECT_BALLOON_0:
		m_balloonObjectType = OBJECT_BALLOON_1;
		RemoveObject(OBJECT_BALLOON_0);
		m_actionArgument = 3;
		break;
	case OBJECT_BALLOON_2:
		m_balloonObjectType = OBJECT_BALLOON_3;
		RemoveObject(OBJECT_BALLOON_2);
		m_actionArgument = 1;
		break;
	case OBJECT_BALLOON_4:
		m_balloonObjectType = OBJECT_BALLOON_5;
		RemoveObject(OBJECT_BALLOON_4);
		m_actionArgument = 4;
		break;
	case OBJECT_BALLOON_6:
		m_balloonObjectType = OBJECT_BALLOON_7;
		RemoveObject(OBJECT_BALLOON_6);
		m_actionArgument = 0;
		break;
	}
	m_balloonPostActive = g_pAI->m_balloonPost->FindPost(m_balloonObjectType, postPos);
	m_lastMovementTick = g_dwGameTick;
	g_pAI->Score(10);
}

// FUNCTION: LEMBALL 0x00410090
void CPlayerLemming::SetBored(unsigned long p_minimumDelay)
{
	int random = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	*g_pSentinel = random;
	m_boredDeadline = p_minimumDelay + random % 5000;
	m_boredDeadline = m_boredDeadline - m_boredDeadline % 0x42;
	m_boredDeadline = m_boredDeadline / GAME_TICK_MILLISECONDS;
	m_boredDeadline = m_actionDeadline + m_boredDeadline;
}

// FUNCTION: LEMBALL 0x00410100
void CPlayerLemming::StartStanding()
{
	CMover* mover = 0;
	volatile unsigned int groundZ = g_pMap->GetZ(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, &mover);
	int tileZ = m_position.m_zFixed >> 12;
	if (m_onMover == 0 && mover != 0) {
		mover->GetOn(this);
	}
	if (tileZ <= (int) groundZ + 2) {
		if (mover == 0) {
			m_position.m_zFixed = groundZ << 12;
		}
		g_pAI->StepOn(m_position, this, m_collisionFlags);
		return;
	}
	m_actionDeadline = g_dwGameTick;
	if ((m_collisionFlags & 4) != 0) {
		m_flightVelocity.m_yFixed = 0;
		m_isFalling = 1;
		m_flightVelocity.m_xFixed = 0x3000;
		m_flightVelocity.m_zFixed = (((tileZ - (int) groundZ) / 8) + 1) * 0x1000;
		unsigned int now = g_dwGameTick;
		int posY = m_position.m_yFixed;
		m_actionArgument = 0;
		m_lastMovementTick = now;
		int posX = m_position.m_xFixed;
		m_flightZ = tileZ;
		m_groundPosition.m_xFixed = posX;
		m_groundPosition.m_yFixed = posY;
		m_groundPosition.m_zFixed = groundZ << 12;
	}
}

// FUNCTION: LEMBALL 0x00410220
void CPlayerLemming::Action(eAction p_action)
{
	m_stateTimer = g_dwSimulationTimestamp;
	if (p_action == ACTION_8) {
		CGlobalGameObject::Action(p_action);
		return;
	}
	m_action = p_action;
}

// FUNCTION: LEMBALL 0x00410250
void CPlayerLemming::OnConveyor(unsigned int p_onConveyor, CIce* p_ice, unsigned int p_leave)
{
	if (p_onConveyor == 0 && m_onConveyor != 0 && p_leave != 0) {
		((CIce*) m_ice)->Leave(this);
	}
	m_onConveyor = p_onConveyor;
	m_ice = p_ice;
	if (p_onConveyor != 0) {
		if (m_group->GetElementsInGroup() > 1) {
			g_pAI->m_playerGroupManager->CreateNewGroup(1, &m_objectId);
		}
		m_group->ClearExistingWaypoints();
	}
}

// FUNCTION: LEMBALL 0x004102d0
bool CPlayerLemming::IsSelectable()
{
	if (!CGameObject::IsSelectable()) {
		return 0;
	}
	if (m_action == 0xf) {
		int actionArgument = (unsigned short) m_actionArgument;
		if (actionArgument >= 1 && actionArgument <= 2) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004108b0
void CPlayerLemming::GetViewData(CViewData& p_viewData)
{
	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> 12;
	p_viewData.m_positionY = m_position.m_yFixed >> 12;
	p_viewData.m_positionZ = m_position.m_zFixed >> 12;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int timer = m_stateTimer;
	eAction action = m_action;
	p_viewData.m_actionArgument = (unsigned short) m_actionArgument;
	p_viewData.m_action = action;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_stateTimer = timer;
	p_viewData.m_unk0x30 = m_unk0xc0;
	p_viewData.m_auxiliaryPosition.m_xFixed = m_auxiliaryPosition.m_xFixed;
	p_viewData.m_auxiliaryPosition.m_yFixed = m_auxiliaryPosition.m_yFixed;
	p_viewData.m_auxiliaryPosition.m_zFixed = m_auxiliaryPosition.m_zFixed;
	p_viewData.m_soundEffect = m_soundEffect;
	if (m_isRemoteObject != 0) {
		p_viewData.m_animationTime = g_dwNetworkSimulationTimestamp;
	}
	else {
		p_viewData.m_animationTime = g_dwSimulationTimestamp;
	}
	SetSndEffect(SFX_NONE);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
	int flags = (m_isGroupLeader != 0 ? 1 : 0) | (m_groupIndex != 0 ? 2 : 0);
	p_viewData.m_statusFlags = flags;
	p_viewData.m_playerIndex = m_playerIndex;
}

// FUNCTION: LEMBALL 0x004109f0
int CPlayerLemming::Bored()
{
	return m_boredDeadline < g_dwGameTick;
}

// FUNCTION: LEMBALL 0x00410a10
int CPlayerLemming::IsHit()
{
	return m_wasHitByBullet;
}

// FUNCTION: LEMBALL 0x00410a20
void CPlayerLemming::GetHit()
{
	int& count = g_pAI->m_objectCount;
	int index = 0;
	int objectCount = count;
	if (index < objectCount) {
		CGameObject**& objects = g_pAI->m_objects;
		do {
			if (objects[index] == this) {
				count = objectCount - 1;
				while (index < count) {
					objects[index] = objects[index + 1];
					index++;
				}
				objects[count] = 0;
				return;
			}
			index++;
		} while (index < objectCount);
	}
}

// FUNCTION: LEMBALL 0x00410aa0
void CPlayerLemming::HitBall()
{
	m_wasHitByBullet = 1;
	m_actionDeadline = g_dwGameTick + 0x3c;
}

// FUNCTION: LEMBALL 0x00410ac0
void CPlayerLemming::PickUpAmmo(unsigned short p_amount)
{
	m_ammoCount += p_amount;
	if (m_ammoCount > PLAYER_MAX_AMMO) {
		m_ammoCount = PLAYER_MAX_AMMO;
	}
}

// FUNCTION: LEMBALL 0x00410af0
int CPlayerLemming::OnConveyor()
{
	return m_onConveyor;
}

// FUNCTION: LEMBALL 0x00410b00
CIce* CPlayerLemming::Conveyor()
{
	return m_ice;
}

// FUNCTION: LEMBALL 0x00410b10
int CPlayerLemming::QOnBalloon()
{
	return m_balloonPostActive;
}
