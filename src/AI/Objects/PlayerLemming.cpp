#include "PlayerLemming.h"

#include "../../Control/Game/Demo.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameStatus.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Map/Ground/Ground.h"
#include "../../Map/Ground/GroundArray.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../../Visos/Network/Connect.h"
#include "../Base/BaseGlobalObject.h"
#include "../Base/GlobalGameObject.h"
#include "../Base/StateMachine.h"
#include "../Groups/PlayerLemmingGroup.h"
#include "../Groups/PlayerLemmingGroupManager.h"
#include "../Messages/ObjectHitMess.h"
#include "../Navigation/Ai.h"
#include "../Navigation/AiCursor.h"
#include "../Navigation/AiDestinationList.h"
#include "../Navigation/Mover.h"
#include "../Objects/BalloonPost.h"
#include "../Objects/Bullet.h"
#include "../Objects/Ice.h"
#include "ViewData.h"

#include <string.h>

// 68K 0x1061bdc0 __ct__14CPlayerLemmingFiiiiUcUl
// FUNCTION: LEMBALL 0x0040ecb0
PlayerLemming::PlayerLemming(int p_x,
							 int p_y,
							 int p_z,
							 int p_facing,
							 unsigned int p_alternatePlayer,
							 unsigned long p_spawnDelay)
	: GlobalGameObject((eObjectType) (p_alternatePlayer ? 1 : 2), 0x17f, 0x14)
{
	m_alternatePlayer = p_alternatePlayer;
	m_spawnPosition.m_xFixed = p_x << 12;
	m_spawnPosition.m_yFixed = p_y << 12;
	m_spawnDelay = p_spawnDelay;
	m_initialFacingDirection = (short) p_facing;
	m_spawnPosition.m_zFixed = p_z << 12;
	if (p_alternatePlayer != 0) {
		SetId(NextLoadingId());
	}
	else {
		m_payloadCapacity += 0x11;
	}
}

// 68K 0x1061bf0a Restart__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040ed90
void PlayerLemming::Restart()
{
	GlobalGameObject::Restart();
	if (m_alternatePlayer == 0) {
		m_playerIndex = g_wNetworkLemmingIndex;
		g_wNetworkLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex] = this;
		g_wLemmingCount++;
		m_action = (eAction) 12;
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
		memcpy(&m_collisionMinX, collision, sizeof(collision));
		g_pAI->m_objects[g_pAI->m_objectCount] = this;
		g_pAI->m_objectCount++;
		m_flightVelocity.m_xFixed = 0x3000;
		m_flightVelocity.m_yFixed = 0;
		m_flightVelocity.m_zFixed = 0xa000;
		m_isGroupLeader = 0;
		m_desiredFacingDirection = m_initialFacingDirection;
		m_wasHitByBullet = 0;
		m_hasDestination = 0;
		m_fireRequestState = (eFireRequestState) 0;
		SetBored(4000);
		AiCoord dest;
		dest.m_xFixed = ((4 - m_playerIndex) * 16 + tileX) << 12;
		dest.m_yFixed = tileY << 12;
		dest.m_zFixed = tileZ << 12;
		AddDestination(dest);
		m_position = m_spawnPosition;
		m_unk0x2c = 0;
		m_sfxChanged = 1;
		m_group = 0;
		m_ice = 0;
		m_onConveyor = 0;
		m_ammoCount = 25;
	}
	else {
		m_playerIndex = g_wLocalLemmingIndex;
		g_wLocalLemmingIndex++;
		g_pAI->m_networkLemmings[m_playerIndex + 4] = this;
		m_isRemoteObject = 1;
		m_action = (eAction) 8;
	}
	m_position.m_zFixed += 0x44000;
	m_facingDirection = m_initialFacingDirection;
	m_inventoryCount = 0;
}

// 68K 0x1061c1d8 HitBullet__14CPlayerLemmingFP7CBullet
// FUNCTION: LEMBALL 0x0040f000
void PlayerLemming::HitBullet(Bullet* p_bullet)
{
	if (g_pGameStatus->m_status0 == 0) {
		if ((int) m_action < 4 || ((int) m_action > 5 && m_action != 16)) {
			if (p_bullet->m_owner == 1) {
				int randVal = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
				*g_pSentinel = randVal;
				if (randVal % 2) {
					return;
				}
			}
			else {
				if (p_bullet->m_owner != 2) {
					return;
				}
				if (g_pActiveConnection != 0) {
					g_pObjectHitMessage->Send(p_bullet);
				}
			}
			m_unk0xc0 = 0;
			m_wasHitByBullet = 1;
			m_actionDeadline = g_dwGameTick + 40;
			m_facingDirection = (p_bullet->m_facingDirection + 4) & 7;
		}
	}
}

// 68K 0x1061c2c6 SetGroup__14CPlayerLemmingFP19CPlayerLemmingGroup
// FUNCTION: LEMBALL 0x0040f0d0
void PlayerLemming::SetGroup(PlayerLemmingGroup* p_arg0)
{
	m_group = p_arg0;
}

// 68K 0x1061c30e GetGroup__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f0e0
PlayerLemmingGroup* PlayerLemming::GetGroup()
{
	return m_group;
}

// 68K 0x1061c33e SetGroupLeader__14CPlayerLemmingFUc
// FUNCTION: LEMBALL 0x0040f0f0
void PlayerLemming::SetGroupLeader(unsigned int p_arg0)
{
	m_isGroupLeader = p_arg0;
}

// 68K 0x1061c378 SetGroup__14CPlayerLemmingFUc
// FUNCTION: LEMBALL 0x0040f100
void PlayerLemming::SetGroup(unsigned int p_arg0)
{
	m_groupIndex = p_arg0;
}

// 68K 0x1061c3aa Process__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f120
bool PlayerLemming::Process()
{
	if (m_isRemoteObject == 0) {
		if (g_pAI->IsLemmingPlayerControlled(this)) {
			UserLemming(g_pAI, this);
			return false;
		}
		AiPlayerLemming(g_pAI, this);
	}
	return false;
}

// 68K 0x1061c416 TurnToFaceCursor__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f160
void PlayerLemming::TurnToFaceCursor()
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
		m_actionDeadline = g_dwGameTick + g_anTurnDelayCursor[m_objectType] / 50;
	}
}

// 68K 0x1061c50e TurnToFaceTarget__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f220
void PlayerLemming::TurnToFaceTarget()
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
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / 50;
}

// 68K 0x1061c600 IsRequestingFire__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f2b0
bool PlayerLemming::IsRequestingFire()
{
	return m_fireRequestState == 1;
}

// 68K 0x1061c640 RequestFire__14CPlayerLemmingFii
// FUNCTION: LEMBALL 0x0040f2c0
void PlayerLemming::RequestFire(int p_x, int p_y)
{
	if (m_fireRequestState == 0 && (m_action == 0 || m_action == 2 || m_action == 6)) {
		m_fireRequestState = (eFireRequestState) 1;
		m_fireTarget.m_xFixed = p_x << 12;
		m_fireTarget.m_yFixed = p_y << 12;
	}
}

// 68K 0x1061c6b0 Fire__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f310
void PlayerLemming::Fire()
{
	AiCoord start;
	start.m_xFixed = m_position.m_xFixed;
	short facing = m_facingDirection;
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
			g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 0, facing, start, m_fireTarget);
			m_soundEffect = (eSoundEffect) 13;
			if (g_pGameStatus->m_status1 == 0) {
				m_ammoCount--;
			}
		}
	}
	m_fireRequestState = (eFireRequestState) 0;
}

// 68K 0x1061c7c6 StartFiring__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f410
void PlayerLemming::StartFiring()
{
	m_actionDeadline = g_dwGameTick + 10;
}

// 68K 0x1061c800 EndFiring__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f420
void PlayerLemming::EndFiring()
{
	m_fireRequestState = (eFireRequestState) 0;
}

// 68K 0x1061c830 FacingCursor__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f430
bool PlayerLemming::FacingCursor()
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

// 68K 0x1061c8c4 FacingTarget__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f4b0
bool PlayerLemming::FacingTarget()
{
	unsigned int facing = ReturnFacingDirection(m_position.m_xFixed >> 12,
												m_position.m_yFixed >> 12,
												m_fireTarget.m_xFixed >> 12,
												m_fireTarget.m_yFixed >> 12);
	return (int) m_facingDirection == (int) facing;
}

// 68K 0x1061c950 Die__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f500
void PlayerLemming::Die()
{
	for (int i = 0; i < (int) m_inventoryCount; i++) {
		switch (m_inventoryObjects[i]->m_objectType) {
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			((BaseGlobalObject*) m_inventoryObjects[i])->OldRestart();
			break;
		}
	}
	GetHit();
	g_wLemmingCount--;
	if (g_wLemmingCount == 0) {
		g_pAI->GameState((eGameStatus) 5);
	}
}

// 68K 0x1061ca00 HitMine__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f600
void PlayerLemming::HitMine()
{
	C3DVector vel;
	vel.m_xFixed = 0;
	vel.m_yFixed = 0;
	m_unk0x10c = 1;
	vel.m_zFixed = 0xa000;
	StartFly(vel, 0);
	m_unk0x2c = 1;
}

// 68K 0x1061ca64 GetData__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f640
void PlayerLemming::GetData()
{
	unsigned short val;
	m_position.m_xFixed = (int) (unsigned int) GetWord() << 12;
	m_position.m_yFixed = (int) (unsigned int) GetWord() << 12;
	m_position.m_zFixed = (int) (unsigned int) GetWord() << 12;
	Get(val);
	m_facingDirection = val & 7;
	m_actionArgument = (val & 0x38) >> 3;
	Get(val);
	m_action = (eAction) (val & 0xff);
	m_soundEffect = (eSoundEffect) (val >> 8);
	m_stateTimer = GetDword();
}

// 68K 0x1061cb66 AddData__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f6f0
void PlayerLemming::AddData()
{
	Add((unsigned short) 0x2c);
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

// 68K 0x1061cc5e CheckSFX__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040f7a0
bool PlayerLemming::CheckSfx()
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

// 68K 0x1061ccb2 HasObject__14CPlayerLemmingF11eObjectType
// FUNCTION: LEMBALL 0x0040f960
bool PlayerLemming::HasObject(eObjectType p_arg0)
{
	if (p_arg0 == (eObjectType) 5) {
		if (m_ammoCount == 50) {
			return 1;
		}
		return 0;
	}
	int count = m_inventoryCount;
	if (count != 12) {
		for (int i = 0; i < count; i++) {
			if (m_inventoryTypes[i] == p_arg0) {
				return 1;
			}
		}
	}
	return 0;
}

// 68K 0x1061cd36 AddObject__14CPlayerLemmingF11eObjectTypeP11CGameObject
// FUNCTION: LEMBALL 0x0040f9b0
bool PlayerLemming::AddObject(eObjectType p_arg0, GameObject* p_arg1)
{
	if (m_inventoryCount == 12) {
		return 0;
	}
	if (HasObject(p_arg0)) {
		return 0;
	}
	m_inventoryTypes[m_inventoryCount] = p_arg0;
	m_inventoryObjects[m_inventoryCount] = p_arg1;
	m_inventoryCount++;
	return 1;
}

// 68K 0x1061cdd0 RandomAction__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040fa10
void PlayerLemming::RandomAction()
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

// 68K 0x1061ce66 Resurrect__14CPlayerLemmingFRC7AICOORD
// FUNCTION: LEMBALL 0x0040fa80
void PlayerLemming::Resurrect(const AiCoord& p_arg0)
{
	m_position.m_xFixed = p_arg0.m_xFixed;
	m_position.m_yFixed = p_arg0.m_yFixed;
	m_position.m_zFixed = p_arg0.m_zFixed;
	m_unk0x2c = 0;
	g_wLemmingCount++;
	m_facingDirection = 0;
	m_inventoryCount = 0;
	m_action = (eAction) 0;
	m_isGroupLeader = 0;
	m_wasHitByBullet = 0;
	m_ice = 0;
	m_onConveyor = 0;
	m_hasDestination = 0;
	m_unk0xc4 = 0;
	m_fireRequestState = (eFireRequestState) 0;
	m_isFlying = 0;
	m_unk0xc0 = 0;
	m_unk0x8c = 0;
	m_routeSearchFailed = 0;
	m_routeSearchActive = 0;
	m_unk0x104 = 0;
	m_unk0x108 = 0;
	m_unk0x10c = 0;
	m_unk0x110 = 0xffff;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	m_unk0x120 = 0xffff;
	m_flightVelocity.m_zFixed = 0;
	m_desiredFacingDirection = 0;
	m_unk0x58 = 0;
	m_activator = 0;
	m_unk0x11c = 0;
	m_ammoCount = 25;
	SetBored(4000);
	int tileX = m_position.m_xFixed >> 12;
	int tileY = m_position.m_yFixed >> 12;
	int tileZ = m_position.m_zFixed >> 12;
	m_collisionMinX = tileX - 8;
	m_collisionMinY = tileY - 8;
	m_collisionMinZ = tileZ;
	m_collisionMaxX = tileX + 7;
	m_collisionMaxY = tileY + 7;
	m_collisionMaxZ = tileZ + 15;
	g_pAI->m_objects[g_pAI->m_objectCount] = this;
	g_pAI->m_objectCount++;
	m_destinationList->m_count = 0;
}

// 68K 0x1061cfd8 GetLastBalloon__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040fbe0
int PlayerLemming::GetLastBalloon()
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

// 68K 0x1061d064 RemoveObject__14CPlayerLemmingF11eObjectType
// FUNCTION: LEMBALL 0x0040fc50
void PlayerLemming::RemoveObject(eObjectType p_arg0)
{
	for (int i = 0; i < (int) m_inventoryCount; i++) {
		if (m_inventoryTypes[i] == p_arg0) {
			for (int j = i + 1; j < (int) m_inventoryCount; j++) {
				m_inventoryTypes[j - 1] = m_inventoryTypes[j];
			}
			m_inventoryCount--;
			return;
		}
	}
}

// 68K 0x1061d0ee GetObject__14CPlayerLemmingFi
// FUNCTION: LEMBALL 0x0040fcb0
int PlayerLemming::GetObject(int p_arg0)
{
	if ((int) m_inventoryCount <= p_arg0) {
		return 0xffff;
	}
	return m_inventoryTypes[p_arg0];
}

// 68K 0x1061d136 ExternalControlEnd__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040fcd0
void PlayerLemming::ExternalControlEnd()
{
	if (m_actionArgument >= 1 && m_actionArgument <= 2) {
		Die();
		Action((eAction) 8);
	}
	else {
		Action((eAction) 0);
	}
}

// 68K 0x1061d1b8 OnBalloon__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040fd10
void PlayerLemming::OnBalloon()
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
		SetSndEffect((eSoundEffect) 0x2b);
		m_unk0x108 = 1;
		m_actionArgument = 0;
		m_lastMovementTick = g_dwGameTick;
		m_action = (eAction) 11;
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

// 68K 0x1061d408 StartBalloon__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040ff60
void PlayerLemming::StartBalloon()
{
}

// 68K 0x1061d434 RequestBalloon__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x0040ff70
void PlayerLemming::RequestBalloon()
{
	AiCoord postPos;
	postPos.m_xFixed = DEBUG_SENTINEL;
	postPos.m_yFixed = DEBUG_SENTINEL;
	postPos.m_zFixed = DEBUG_SENTINEL;
	int lastBalloon = GetLastBalloon();
	if (lastBalloon <= 0xffff) {
		if (lastBalloon == 0xffff) {
			m_balloonPostActive = 0;
			return;
		}
		switch (lastBalloon) {
		case 0x27:
			m_balloonObjectType = (eObjectType) 0x28;
			RemoveObject((eObjectType) 0x27);
			m_actionArgument = 3;
			break;
		case 0x29:
			m_balloonObjectType = (eObjectType) 0x2a;
			RemoveObject((eObjectType) 0x29);
			m_actionArgument = 1;
			break;
		case 0x2b:
			m_balloonObjectType = (eObjectType) 0x2c;
			RemoveObject((eObjectType) 0x2b);
			m_actionArgument = 4;
			break;
		case 0x2d:
			m_balloonObjectType = (eObjectType) 0x2e;
			RemoveObject((eObjectType) 0x2d);
			m_actionArgument = 0;
			break;
		}
	}
	m_balloonPostActive = g_pAI->m_balloonPost->FindPost(m_balloonObjectType, postPos);
	m_lastMovementTick = g_dwGameTick;
	g_pAI->Score(10);
}

// 68K 0x1061d572 SetBored__14CPlayerLemmingFUl
// FUNCTION: LEMBALL 0x00410090
void PlayerLemming::SetBored(unsigned long p_minimumDelay)
{
	*g_pSentinel = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	m_boredDeadline = p_minimumDelay + *g_pSentinel % 5000;
	m_boredDeadline = m_boredDeadline - m_boredDeadline % 0x42;
	m_boredDeadline = m_boredDeadline / 50;
	m_boredDeadline = m_actionDeadline + m_boredDeadline;
}

// 68K 0x1061d5f6 StartStanding__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410100
void PlayerLemming::StartStanding()
{
	Mover* mover = 0;
	unsigned int groundZ = g_pMap->GetZ(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, &mover);
	int tileZ = m_position.m_zFixed >> 12;
	if (m_unk0x11c == 0 && mover != 0) {
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
		m_unk0x108 = 1;
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

// 68K 0x1061d746 Action__14CPlayerLemmingF7eAction
// FUNCTION: LEMBALL 0x00410220
void PlayerLemming::Action(eAction p_action)
{
	m_stateTimer = g_dwSimulationTimestamp;
	if (p_action == 8) {
		GlobalGameObject::Action(p_action);
		return;
	}
	m_action = p_action;
}

void PlayerLemming::Action(eAction p_action, int p_arg1)
{
	GlobalGameObject::Action(p_action, p_arg1);
}

// 68K 0x1061d7a4 OnConveyor__14CPlayerLemmingFUcP4CIceUc
// FUNCTION: LEMBALL 0x00410250
void PlayerLemming::OnConveyor(unsigned int p_onConveyor, Ice* p_ice, unsigned int p_leave)
{
	if (p_onConveyor == 0 && m_onConveyor != 0 && p_leave != 0) {
		((Ice*) m_ice)->Leave(this);
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

// 68K 0x1061d864 IsSelectable__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x004102d0
bool PlayerLemming::IsSelectable()
{
	if (!GameObject::IsSelectable()) {
		return 0;
	}
	if (m_action == 0xf) {
		int arg = (unsigned short) m_actionArgument;
		if (arg >= 1 && arg <= 2) {
			return 0;
		}
	}
	return 1;
}

// 68K 0x1011a500 GetViewData__14CPlayerLemmingFR9CViewData
// FUNCTION: LEMBALL 0x004108b0
void PlayerLemming::GetViewData(ViewData& p_viewData)
{
	p_viewData.m_objectId = m_objectId;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_positionX = m_position.m_xFixed >> 12;
	p_viewData.m_positionY = m_position.m_yFixed >> 12;
	p_viewData.m_positionZ = m_position.m_zFixed >> 12;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int actionArg = (unsigned short) m_actionArgument;
	unsigned int timer = m_stateTimer;
	eAction action = m_action;
	p_viewData.m_actionArgument = actionArg;
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
	SetSndEffect((eSoundEffect) 0);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
	int flags = (m_isGroupLeader == 0 ? 1 : 0);
	if (m_groupIndex != 0) {
		flags |= 2;
	}
	p_viewData.m_statusFlags = flags;
	p_viewData.m_playerIndex = m_playerIndex;
}

// 68K 0x1011b08c Bored__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x004109f0
int PlayerLemming::Bored()
{
	return m_boredDeadline < g_dwGameTick;
}

// 68K 0x1011b0c4 IsHit__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410a10
int PlayerLemming::IsHit()
{
	return m_wasHitByBullet;
}

// 68K 0x1011b0f0 GetHit__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410a20
void PlayerLemming::GetHit()
{
	int count = g_pAI->m_objectCount;
	int i = 0;
	if (count > 0) {
		GameObject** objects = g_pAI->m_objects;
		for (i = 0; i < count; i++) {
			if (objects[i] == this) {
				g_pAI->m_objectCount = --count;
				for (; i < count; i++) {
					g_pAI->m_objects[i] = g_pAI->m_objects[i + 1];
				}
				g_pAI->m_objects[count] = 0;
				return;
			}
		}
	}
}

// 68K 0x1011b130 HitBall__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410aa0
void PlayerLemming::HitBall()
{
	m_wasHitByBullet = 1;
	m_actionDeadline = g_dwGameTick + 0x3c;
}

// 68K 0x1011b16c PickUpAmmo__14CPlayerLemmingFUs
// FUNCTION: LEMBALL 0x00410ac0
void PlayerLemming::PickUpAmmo(unsigned short p_arg0)
{
	m_ammoCount += p_arg0;
	if (m_ammoCount > 0x32) {
		m_ammoCount = 0x32;
	}
}

// 68K 0x1011a6f4 OnConveyor__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410af0
int PlayerLemming::OnConveyor()
{
	return m_onConveyor;
}

// 68K 0x1011b1b0 Conveyor__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410b00
Ice* PlayerLemming::Conveyor()
{
	return m_ice;
}

// 68K 0x1011b1e0 QOnBalloon__14CPlayerLemmingFv
// FUNCTION: LEMBALL 0x00410b10
int PlayerLemming::QOnBalloon()
{
	return m_balloonPostActive;
}
