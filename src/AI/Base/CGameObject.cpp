#include "CGameObject.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../Navigation/CAI.h"
#include "../Navigation/CAiDestinationEntry.h"
#include "../Navigation/CAiDestinationList.h"
#include "../Navigation/CMaze.h"
#include "../Navigation/CMover.h"
#include "CPt3.h"
#include "Solution.h"

#include <string.h>

#pragma intrinsic(memcpy, memset)

// FUNCTION: LEMBALL 0x0040a7f0
void CGameObject::ForgetObjectLink(unsigned short p_arg0)
{
}

// FUNCTION: LEMBALL 0x0040a800
bool CGameObject::Activate(class CGameObject* p_object)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040a810
bool CGameObject::IsFlying()
{
	return m_isFlying;
}

// FUNCTION: LEMBALL 0x0040a820
int CGameObject::Usage()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a830 FOLDED
AiCoord CGameObject::Position()
{
	return m_position;
}

// FUNCTION: LEMBALL 0x0040a830 FOLDED
AiCoord CGameObject::ActivatePosition()
{
	return m_position;
}

// FUNCTION: LEMBALL 0x0040a860
void CGameObject::StartStanding()
{
}

// FUNCTION: LEMBALL 0x0040a870
void CGameObject::SetSndEffect(eSoundEffect p_soundEffect)
{
	m_soundEffect = p_soundEffect;
}

// FUNCTION: LEMBALL 0x0040a880
eSoundEffect CGameObject::GetSndEffect()
{
	return m_soundEffect;
}

// FUNCTION: LEMBALL 0x0040a890
bool CGameObject::Collision(const CPt3& p_point)
{
	if (m_collisionBounds.m_x1 <= p_point.m_x && p_point.m_x <= m_collisionBounds.m_x2 &&
		m_collisionBounds.m_y1 <= p_point.m_y && p_point.m_y <= m_collisionBounds.m_y2 &&
		m_collisionBounds.m_z1 <= p_point.m_z && p_point.m_z <= m_collisionBounds.m_z2) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040a8e0
bool CGameObject::Collision(const CRect3& p_bounds)
{
	if (m_collisionBounds.m_x1 <= p_bounds.m_x2 && p_bounds.m_x1 <= m_collisionBounds.m_x2 &&
		m_collisionBounds.m_y1 <= p_bounds.m_y2 && p_bounds.m_y1 <= m_collisionBounds.m_y2 &&
		m_collisionBounds.m_z1 <= p_bounds.m_z2 && p_bounds.m_z1 <= m_collisionBounds.m_z2) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040a930
void CGameObject::HitBullet(CBullet* p_bullet)
{
}

// FUNCTION: LEMBALL 0x0040a940
void CGameObject::HitBall()
{
}

// FUNCTION: LEMBALL 0x0040a950
int CGameObject::IsHit()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a960
void CGameObject::GetHit()
{
}

// FUNCTION: LEMBALL 0x0040a970
void CGameObject::HitMine()
{
}

// FUNCTION: LEMBALL 0x0040a980
void CGameObject::Die()
{
}

// FUNCTION: LEMBALL 0x0040a990
void CGameObject::Land()
{
}

// FUNCTION: LEMBALL 0x0040a9a0
bool CGameObject::FacingCursor()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9b0
void CGameObject::TurnToFaceCursor()
{
}

// FUNCTION: LEMBALL 0x0040a9c0
bool CGameObject::IsRequestingFire()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9d0
int CGameObject::Bored()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9e0
void CGameObject::SetBored(unsigned long p_arg0)
{
}

// FUNCTION: LEMBALL 0x0040a9f0
void CGameObject::Fire()
{
}

// FUNCTION: LEMBALL 0x0040aa00
void CGameObject::StartFiring()
{
}

// FUNCTION: LEMBALL 0x0040aa10
void CGameObject::EndFiring()
{
}

// FUNCTION: LEMBALL 0x0040aa20
void CGameObject::RandomAction()
{
}

// FUNCTION: LEMBALL 0x0040aa30
bool CGameObject::FacingTarget()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa40
void CGameObject::TurnToFaceTarget()
{
}

// FUNCTION: LEMBALL 0x0040aa50
bool CGameObject::PossiblyOnLift()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040aa60
bool CGameObject::HasObject(eObjectType p_objectType)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa70
bool CGameObject::AddObject(eObjectType p_objectType, class CGameObject* p_object)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa80
void CGameObject::ReNumberNode(int p_arg0, int p_arg1)
{
}

// FUNCTION: LEMBALL 0x0040aa90
bool CGameObject::NeedsNode(int p_arg0)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040aaa0
void CGameObject::ConvertVer0ToVer1()
{
}

// FUNCTION: LEMBALL 0x0040aab0
void CGameObject::Delete()
{
}

// FUNCTION: LEMBALL 0x0040aac0
void CGameObject::PickUpAmmo(unsigned short p_amount)
{
}

// FUNCTION: LEMBALL 0x0040aad0
void CGameObject::ExternalControlEnd()
{
}

// FUNCTION: LEMBALL 0x0040aae0
void CGameObject::RequestBalloon()
{
	m_balloonPostActive = 1;
}

// FUNCTION: LEMBALL 0x0040aaf0
void CGameObject::StartBalloon()
{
	m_balloonPostActive = 0;
}

// FUNCTION: LEMBALL 0x0040ab00
void CGameObject::OnBalloon()
{
	m_balloonPostActive = 0;
}

// FUNCTION: LEMBALL 0x0040ab10
int CGameObject::QOnBalloon()
{
	return m_balloonPostActive;
}

// FUNCTION: LEMBALL 0x0040ab20
void CGameObject::OnConveyor(unsigned int p_arg0, CIce* p_arg1, unsigned int p_arg2)
{
}

// FUNCTION: LEMBALL 0x0040ab30
int CGameObject::OnConveyor()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab40
CIce* CGameObject::Conveyor()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab50
bool CGameObject::IsUsable(eAction p_action)
{
	return p_action == ACTION_0x18;
}

// FUNCTION: LEMBALL 0x0040c170
void CGameObject::Action(eAction p_action)
{
	m_action = p_action;
}

// FUNCTION: LEMBALL 0x0040c180
void CGameObject::Action(eAction p_action, int p_actionArgument)
{
	m_action = p_action;
	m_actionArgument = (short) p_actionArgument;
}

// FUNCTION: LEMBALL 0x0040c1a0
void CGameObject::SendRemove()
{
}

// FUNCTION: LEMBALL 0x0040c1b0
void CGameObject::SendCancel()
{
}

// FUNCTION: LEMBALL 0x0040c1c0
int CGameObject::UsableState()
{
	return 2;
}

// FUNCTION: LEMBALL 0x00414f30
CGameObject::CGameObject(eObjectType p_objectType,
						 unsigned short p_collisionFlags,
						 unsigned short p_destinationCapacity)
	: m_moveStartXFixed(DEBUG_SENTINEL), m_moveStartYFixed(DEBUG_SENTINEL), m_moveDeltaXFixed(DEBUG_SENTINEL),
	  m_moveDeltaYFixed(DEBUG_SENTINEL)
{
	m_objectType = p_objectType;
	m_collisionFlags = p_collisionFlags;
	CAiDestinationList* list;
	if (p_destinationCapacity != 0 && (list = new CAiDestinationList) != 0) {
		list->m_count = 0;
		list->m_capacity = p_destinationCapacity;
		list->m_entries = new CAiDestinationEntry[p_destinationCapacity];
		m_destinationList = list;
	}
	else {
		m_destinationList = 0;
	}
	m_linkedObjectId = 0xffff;
	bool found = false;
	int i = 0;
	if (0 < g_wObjectCount) {
		do {
			if (g_pObjects[i] == 0) {
				m_objectId = (unsigned short) i;
				found = true;
				break;
			}
			i++;
		} while (i < (int) (unsigned int) g_wObjectCount);
	}
	if (!found) {
		m_objectId = g_wObjectCount;
		g_wObjectCount++;
	}
	g_pObjects[m_objectId] = this;
}

// FUNCTION: LEMBALL 0x004150d0
void CGameObject::Restart()
{
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_auxiliaryPosition.m_xFixed = 0;
	m_auxiliaryPosition.m_yFixed = 0;
	m_auxiliaryPosition.m_zFixed = 0;
	Initialise();
	m_position.m_zFixed = 0;
	m_invisibleSwitchId = (unsigned short) 0xffff;
	m_liftId = 0xffff;
	m_position.m_yFixed = 0;
	m_position.m_xFixed = 0;
	if (m_destinationList != 0) {
		m_destinationList->m_count = 0;
	}
	switch (m_objectType) {
	case OBJECT_PLAYER_1:
		m_runtimeFlags = 0x200;
		break;
	case OBJECT_PLAYER_2:
		m_runtimeFlags = 0x100;
		break;
	}
}

// FUNCTION: LEMBALL 0x00415160
CGameObject::~CGameObject()
{
	ReSetId();
	g_pObjects[m_objectId] = 0;
	CAiDestinationList* destinationList = m_destinationList;
	if (destinationList != 0) {
		operator delete(destinationList->m_entries);
		operator delete(destinationList);
	}
	m_objectId = 0xffff;
}

// FUNCTION: LEMBALL 0x004151b0
void CGameObject::Initialise()
{
	m_actionArgument = 0;
	m_deathRequested = 0;
	m_action = ACTION_NONE;
	m_isRemoteObject = 0;
	m_facingDirection = 0;
	m_objectActive = 0;
	m_unk0xc4 = 0;
	m_unk0x3c = 0;
	m_isFlying = 0;
	m_unk0xc0 = 0;
	m_activationReserved = 0;
	m_routeSearchFailed = 0;
	m_routeSearchActive = 0;
	m_isJumping = 0;
	m_isFalling = 0;
	m_wasHitByMine = 0;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	m_desiredFacingDirection = 0;
	m_flightVelocity.m_zFixed = 0;
	m_unk0x58 = 0;
	m_onMover = 0;
	m_hasDestination = 0;
	m_actionDeadline = g_dwGameTick;
	m_soundEffect = SFX_NONE;
	m_transientFlags = 0;
}

// FUNCTION: LEMBALL 0x00415240
void CGameObject::StartFly(C3DVector& p_velocity, C3DVector* p_origin)
{
	m_isJumping = 0;
	m_isFalling = 0;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	if (p_origin != 0) {
		int x = p_origin->m_xFixed;
		m_flightOrigin.m_xFixed = x;
		int y = p_origin->m_yFixed;
		m_flightOrigin.m_yFixed = y;
		int z = p_origin->m_zFixed;
		m_flightOrigin.m_zFixed = z;
		m_position.m_xFixed = x;
		m_position.m_yFixed = y;
		m_position.m_zFixed = z;
	}
	else {
		m_flightOrigin.m_xFixed = m_position.m_xFixed;
		m_flightOrigin.m_yFixed = m_position.m_yFixed;
		m_flightOrigin.m_zFixed = m_position.m_zFixed;
	}
	m_isFlying = 1;
	m_flightVelocity.m_xFixed = p_velocity.m_xFixed;
	m_flightVelocity.m_yFixed = p_velocity.m_yFixed;
	m_flightVelocity.m_zFixed = p_velocity.m_zFixed;
	m_stateTimer = g_dwSimulationTimestamp;
	m_lastMovementTick = g_dwGameTick;
	m_actionDeadline = g_dwGameTick + 10;
}

// FUNCTION: LEMBALL 0x00415300
void CGameObject::Fly()
{
	int timeDelta = (int) (g_dwGameTick - m_lastMovementTick);
	if (timeDelta > 0) {
		int x = m_flightVelocity.m_xFixed * 2 + m_flightOrigin.m_xFixed;
		int y = m_flightVelocity.m_yFixed * 2 + m_flightOrigin.m_yFixed;
		m_lastMovementTick = g_dwGameTick;
		m_flightOrigin.m_xFixed = x;
		m_flightOrigin.m_yFixed = y;
		m_flightVelocity.m_zFixed -= 0x2000;
		m_flightOrigin.m_zFixed = m_flightVelocity.m_zFixed * 2 + 0x4000 + m_flightOrigin.m_zFixed;
		if (m_flightVelocity.m_zFixed < -0xa000) {
			m_flightVelocity.m_zFixed = -0xa000;
		}

		CMover* mover = 0;
		int groundZ = g_pMap->GetZ(x >> 12, y >> 12, &mover);
		int flightZ = m_flightOrigin.m_zFixed >> 12;
		if (flightZ > groundZ) {
			m_position.m_xFixed = x;
			m_position.m_yFixed = y;
			m_position.m_zFixed = m_flightOrigin.m_zFixed;
		}
		else {
			m_isFlying = 0;
			m_balloonPostId = 0;
			if (flightZ > groundZ - 12) {
				m_position.m_xFixed = x;
				m_position.m_yFixed = y;
				m_position.m_zFixed = m_flightOrigin.m_zFixed;
				m_position.m_zFixed = groundZ << 12;
				if (g_pAI->HitTrampoline(m_position, this) == 0) {
					m_flightVelocity.m_xFixed = 0;
					m_flightVelocity.m_yFixed = 0;
					m_flightVelocity.m_zFixed = 0;
				}
				else {
					m_isFlying = 1;
				}
				if (m_onMover == 0 && mover != 0) {
					mover->GetOn(this);
				}
			}
			else {
				m_actionDeadline = g_dwGameTick;
				if ((m_collisionFlags & 4) != 0) {
					m_isFalling = 1;
					m_flightVelocity.m_xFixed = 0x3000;
					m_flightVelocity.m_yFixed = 0;
					int objectZ = m_position.m_zFixed >> 12;
					int deltaZ = objectZ - groundZ;
					deltaZ += (deltaZ >> 31) & 7;
					m_flightVelocity.m_zFixed = ((deltaZ >> 3) + 1) << 12;
					m_actionArgument = 0;
					m_lastMovementTick = g_dwGameTick;
					m_flightZ = objectZ;
					m_groundPosition.m_xFixed = m_position.m_xFixed;
					m_groundPosition.m_yFixed = m_position.m_yFixed;
					m_groundPosition.m_zFixed = groundZ << 12;
				}
			}
		}
		if (m_balloonPostId == 0 && m_actionDeadline <= g_dwGameTick) {
			m_balloonPostId = 0;
			g_pAI->HitTrampoline(m_position, this);
		}
	}
}

// FUNCTION: LEMBALL 0x00415520
void CGameObject::RotateClockwise()
{
	m_facingDirection++;
	if (m_facingDirection >= 8) {
		unsigned short turns = (unsigned short) m_facingDirection / 8;
		m_facingDirection -= turns * 8;
	}
}

// FUNCTION: LEMBALL 0x00415550
void CGameObject::RotateAnticlockwise()
{
	m_facingDirection--;
	if (m_facingDirection < 0) {
		unsigned short turns = (unsigned short) (7 - m_facingDirection) / 8;
		m_facingDirection += turns * 8;
	}
}

// FUNCTION: LEMBALL 0x00415580
void CGameObject::StartMoving()
{
	if (m_destinationList != 0) {
		CMover* mover = 0;
		int groundZ = g_pMap->GetZ(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, &mover);
		int objectZ = m_position.m_zFixed >> 12;
		if (m_onMover == 0 && mover != 0) {
			mover->GetOn(this);
		}
		if (objectZ == groundZ) {
			AiCoord destination = GetDestination();
			m_destination.m_xFixed = destination.m_xFixed;
			m_destination.m_yFixed = destination.m_yFixed;
			m_destination.m_zFixed = destination.m_zFixed;
			int distance = Distance(m_position.m_xFixed >> 12,
									m_position.m_yFixed >> 12,
									m_destination.m_xFixed >> 12,
									m_destination.m_yFixed >> 12);
			m_lastMovementTick = g_dwGameTick;
			m_moveDurationTicks = (g_anTurnDelayCursor[m_objectType] * distance) / 50;
			if (m_moveDurationTicks == 0) {
				m_moveDurationTicks = 1;
			}
			m_actionDeadline = m_moveDurationTicks + g_dwGameTick;
			m_moveStartXFixed = m_position.m_xFixed;
			m_moveStartYFixed = m_position.m_yFixed;
			m_moveDeltaXFixed = m_destination.m_xFixed - m_moveStartXFixed;
			m_moveDeltaYFixed = m_destination.m_yFixed - m_moveStartYFixed;
		}
		else if (m_balloonPostActive == 0) {
			m_actionDeadline = g_dwGameTick;
			if ((m_collisionFlags & 4) != 0) {
				m_flightVelocity.m_yFixed = 0;
				m_isFalling = 1;
				m_flightVelocity.m_xFixed = 0x3000;
				int deltaZ = objectZ - groundZ;
				deltaZ += (deltaZ >> 31) & 7;
				m_flightVelocity.m_zFixed = ((deltaZ >> 3) + 1) << 12;
				m_actionArgument = 0;
				m_lastMovementTick = g_dwGameTick;
				m_flightZ = objectZ;
				m_groundPosition.m_xFixed = m_position.m_xFixed;
				m_groundPosition.m_yFixed = m_position.m_yFixed;
				m_groundPosition.m_zFixed = (int) groundZ << 12;
			}
		}
		else {
			m_position.m_zFixed = (int) groundZ << 12;
		}
	}
}

// FUNCTION: LEMBALL 0x00415780
void CGameObject::StopMoving()
{
	unsigned int tick;

	DeleteFirstEntryFromDestinationList();
	m_moveDurationTicks = 0;
	tick = g_dwGameTick;
	m_actionDeadline = tick;
	m_lastMovementTick = tick;
}

// FUNCTION: LEMBALL 0x004157b0
unsigned short CGameObject::MapCheck(int p_x, int p_y)
{
	int blockY = (p_y + ((p_y >> 31) & 0xf)) >> 4;
	int blockX = (p_x + ((p_x >> 31) & 0xf)) >> 4;
	unsigned short collision = 0;

	for (int x = blockX; x <= blockX; x++) {
		for (int y = blockY; y <= blockY; y++) {
			if (x >= 0 && y >= 0 && x < g_pMap->m_ground.m_width && y < g_pMap->m_ground.m_height) {
				collision |= g_pMap->m_ground.m_ground[y * g_pMap->m_ground.m_width + x].m_collision;
			}
			else {
				collision |= 3;
			}
		}
	}
	return collision;
}

// FUNCTION: LEMBALL 0x00415830
bool CGameObject::StartRoute()
{
	unsigned int* routeSearchBusy = &g_pMaze->m_routeSearchBusy;
	m_routeSearchActive = *routeSearchBusy == 0;
	*routeSearchBusy = 1;
	if (m_routeSearchActive != 0) {
		g_pMaze->BInitialise(0,
							 (m_position.m_xFixed >> 12) / 16,
							 (m_position.m_yFixed >> 12) / 16,
							 (m_destination.m_xFixed >> 12) / 16,
							 (m_destination.m_yFixed >> 12) / 16);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004158b0
bool CGameObject::SearchRoute()
{
	if (m_routeSearchActive != 0) {
		int solutionCount;
		int complete;
		unsigned int reached;
		unsigned int noChanges;
		Solution solutions[120];

		complete = g_pMaze->BIteration(reached, noChanges);
		m_routeSearchFailed = complete == 0;
		if (reached != 0) {
			g_pMaze->BSolution(solutionCount, solutions);
			m_destinationList->m_count = 0;
			if (solutionCount < 80) {
				int index = solutionCount - 1;
				if (index >= 0) {
					Solution* solution = &solutions[index];
					do {
						AiCoord coordinate;
						coordinate.m_xFixed = ((unsigned int) (unsigned short) solution->m_x << 16) + 0x8000;
						coordinate.m_yFixed = ((unsigned int) (unsigned short) solution->m_y << 16) + 0x8000;
						coordinate.m_zFixed = 0;
						CAiDestinationList* list = m_destinationList;
						unsigned short count = list->m_count;
						if (count < list->m_capacity) {
							list->m_count = count + 1;
							CAiDestinationEntry* entry = &list->m_entries[count];
							entry->m_type = DESTINATION_COORD;
							entry->m_coordinate.m_xFixed = coordinate.m_xFixed;
							entry->m_coordinate.m_yFixed = coordinate.m_yFixed;
							entry->m_coordinate.m_zFixed = coordinate.m_zFixed;
						}
						solution--;
					} while (solution >= solutions);
				}
			}
		}
		if (complete != 0) {
			g_pMaze->m_routeSearchBusy = 0;
			m_routeSearchActive = 0;
		}
	}
	else {
		unsigned int* routeSearchBusy = &g_pMaze->m_routeSearchBusy;
		m_routeSearchActive = *routeSearchBusy == 0;
		*routeSearchBusy = 1;
		if (m_routeSearchActive != 0) {
			g_pMaze->BInitialise(0,
								 (m_position.m_xFixed >> 12) / 16,
								 (m_position.m_yFixed >> 12) / 16,
								 (m_destination.m_xFixed >> 12) / 16,
								 (m_destination.m_yFixed >> 12) / 16);
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00415a20
void CGameObject::Blocked()
{
	if ((m_collisionFlags & 1) != 0) {
		m_routeSearchFailed = 1;
	}
}

// FUNCTION: LEMBALL 0x00415a30
bool CGameObject::Move()
{
	int elapsed = (int) (g_dwGameTick - m_lastMovementTick);
	AiCoord position;
	position.m_xFixed = m_moveStartXFixed + (m_moveDeltaXFixed * elapsed) / m_moveDurationTicks;
	position.m_yFixed = m_moveStartYFixed + (m_moveDeltaYFixed * elapsed) / m_moveDurationTicks;
	int x = position.m_xFixed >> 12;
	int y = position.m_yFixed >> 12;
	if (x < 0 || (x >> 4) >= g_pMap->m_ground.m_width || y < 0 || (y >> 4) >= g_pMap->m_ground.m_height) {
		m_actionDeadline = g_dwGameTick;
		return false;
	}

	CMover* mover = 0;
	unsigned short groundZ = g_pMap->GetZ(x, y, &mover);
	if (m_onMover == 0 && mover != 0) {
		mover->GetOn(this);
	}
	if ((MapCheck(x, y) & 1) != 0) {
		position.m_xFixed = x << 12;
		position.m_yFixed = y << 12;
		position.m_zFixed = (int) groundZ << 12;
		if (g_pAI->OpenDoor(position, this, m_collisionFlags)) {
			m_actionDeadline = g_dwGameTick;
			return false;
		}
		Blocked();
		m_actionDeadline = g_dwGameTick;
		return false;
	}

	unsigned short currentGroundZ = g_pMap->GetZ(m_position.m_xFixed >> 12, m_position.m_yFixed >> 12, &mover);
	if ((int) currentGroundZ + 7 <= (int) groundZ) {
		m_actionDeadline = g_dwGameTick;
		if ((int) currentGroundZ + 15 < (int) groundZ) {
			Blocked();
			return false;
		}
		if ((m_collisionFlags & 2) != 0) {
			m_isJumping = 1;
			m_lastMovementTick = g_dwGameTick;
			m_flightZ = currentGroundZ;
			m_groundPosition.m_xFixed = position.m_xFixed;
			m_groundPosition.m_yFixed = position.m_yFixed;
			m_groundPosition.m_zFixed = (int) groundZ << 12;
			m_actionArgument = 0;
			return false;
		}
		Blocked();
		return false;
	}

	if ((int) groundZ <= (int) currentGroundZ - 7) {
		m_actionDeadline = g_dwGameTick;
		if ((m_collisionFlags & 4) != 0) {
			m_isFalling = 1;
			m_actionArgument = 0;
			m_lastMovementTick = g_dwGameTick;
			m_flightZ = currentGroundZ;
			int deltaX = x - (m_position.m_xFixed >> 12);
			int deltaY = y - (m_position.m_yFixed >> 12);
			int absDeltaX = deltaX < 0 ? -deltaX : deltaX;
			int absDeltaY = deltaY < 0 ? -deltaY : deltaY;
			int velocityX;
			int velocityY;
			if (absDeltaX > absDeltaY) {
				velocityX = deltaX <= 0 ? -1 : 1;
				velocityY = 0;
			}
			else {
				velocityX = 0;
				velocityY = deltaY <= 0 ? -1 : 1;
			}
			m_position.m_xFixed = position.m_xFixed;
			m_position.m_yFixed = position.m_yFixed;
			m_groundPosition.m_xFixed = position.m_xFixed;
			m_groundPosition.m_yFixed = position.m_yFixed;
			m_groundPosition.m_zFixed = (int) groundZ << 12;
			m_position.m_xFixed = position.m_xFixed;
			m_position.m_yFixed = position.m_yFixed;
			m_flightVelocity.m_xFixed = velocityX << 12;
			m_flightVelocity.m_yFixed = velocityY << 12;
			int deltaZ = (int) currentGroundZ - groundZ;
			m_flightVelocity.m_zFixed = ((deltaZ / 8) + 1) * 0x1000;
			return false;
		}
		Blocked();
		return false;
	}

	m_position.m_xFixed = position.m_xFixed;
	m_position.m_yFixed = position.m_yFixed;
	m_position.m_zFixed = (int) groundZ << 12;
	g_pAI->StepOn(m_position, this, m_collisionFlags);
	return true;
}

// FUNCTION: LEMBALL 0x00415d90
void CGameObject::TurnToFaceDestination()
{
	AiCoord destination = GetDestination();
	int direction = (int) ReturnFacingDirection(m_position.m_xFixed >> 12,
												m_position.m_yFixed >> 12,
												destination.m_xFixed >> 12,
												destination.m_yFixed >> 12);
	if (direction != m_facingDirection) {
		if (g_anRotationDirections[(direction - (int) m_facingDirection) & 7] < 0) {
			RotateAnticlockwise();
		}
		else {
			RotateClockwise();
		}
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / 50;
}

// FUNCTION: LEMBALL 0x00415e20
bool CGameObject::FacingDestination()
{
	AiCoord dest = GetDestination();
	int dir = ReturnFacingDirection(m_position.m_xFixed >> 12,
									m_position.m_yFixed >> 12,
									dest.m_xFixed >> 12,
									dest.m_yFixed >> 12);
	return (int) m_facingDirection == dir;
}

// FUNCTION: LEMBALL 0x00415e80
void CGameObject::DeleteFirstEntryFromDestinationList()
{
	int i;
	CAiDestinationList* list = m_destinationList;
	if (list->m_count != 0) {
		for (i = 0; i < list->m_count - 1; i++) {
			CAiDestinationEntry* entry = &list->m_entries[i];
			CAiDestinationEntry* next = entry + 1;
			entry->m_type = next->m_type;
			entry->m_coordinate.m_xFixed = next->m_coordinate.m_xFixed;
			entry->m_coordinate.m_yFixed = next->m_coordinate.m_yFixed;
			entry->m_coordinate.m_zFixed = next->m_coordinate.m_zFixed;
			entry->m_metadata = next->m_metadata;
		}
		list->m_count--;
	}
	m_hasDestination = (unsigned short) 0 < m_destinationList->m_count;
}
// FUNCTION: LEMBALL 0x00415ef0
void CGameObject::AddDestination(const AiCoord& p_destination)
{
	CAiDestinationList* list = m_destinationList;
	if (list != 0 && list->m_count < list->m_capacity) {
		unsigned short count = list->m_count;
		list->m_count = count + 1;
		CAiDestinationEntry* entry = &list->m_entries[count];
		entry->m_type = DESTINATION_COORD;
		entry->m_coordinate.m_xFixed = p_destination.m_xFixed;
		entry->m_coordinate.m_yFixed = p_destination.m_yFixed;
		entry->m_coordinate.m_zFixed = p_destination.m_zFixed;
	}
}

// FUNCTION: LEMBALL 0x00415f30
void CGameObject::AlterDestination(const AiCoord& p_destination)
{
	int i;
	CAiDestinationList* list = m_destinationList;
	if (list->m_count != 0) {
		for (i = 0; i < list->m_count - 1; i++) {
			CAiDestinationEntry* entry = &list->m_entries[i];
			CAiDestinationEntry* next = entry + 1;
			entry->m_type = next->m_type;
			entry->m_coordinate.m_xFixed = next->m_coordinate.m_xFixed;
			entry->m_coordinate.m_yFixed = next->m_coordinate.m_yFixed;
			entry->m_coordinate.m_zFixed = next->m_coordinate.m_zFixed;
			entry->m_metadata = next->m_metadata;
		}
		list->m_count--;
	}

	CAiDestinationList* destinationList = m_destinationList;
	unsigned short count = destinationList->m_count;
	if (count < destinationList->m_capacity) {
		for (int i = count; i > 0; i--) {
			CAiDestinationEntry* entry = &destinationList->m_entries[i];
			CAiDestinationEntry* previous = entry - 1;
			entry->m_type = previous->m_type;
			entry->m_coordinate.m_xFixed = previous->m_coordinate.m_xFixed;
			entry->m_coordinate.m_yFixed = previous->m_coordinate.m_yFixed;
			entry->m_coordinate.m_zFixed = previous->m_coordinate.m_zFixed;
			entry->m_metadata = previous->m_metadata;
		}
		destinationList->m_count++;
		CAiDestinationEntry* entry = destinationList->m_entries;
		entry->m_type = DESTINATION_COORD;
		entry->m_coordinate.m_xFixed = p_destination.m_xFixed;
		entry->m_coordinate.m_yFixed = p_destination.m_yFixed;
		entry->m_coordinate.m_zFixed = p_destination.m_zFixed;
	}
	StartMoving();
}

// FUNCTION: LEMBALL 0x00416000
AiCoord CGameObject::GetDestination()
{
	if (m_destinationList->m_count > 0) {
		return m_destinationList->m_entries[0].GetCoordinate();
	}
	return m_position;
}

// FUNCTION: LEMBALL 0x004160c0
bool CGameObject::DestinationExists()
{
	return 0 < m_destinationList->m_count;
}

// FUNCTION: LEMBALL 0x004160e0
void CGameObject::EmptyDestinationList()
{
	m_destinationList->m_count = 0;
}

// FUNCTION: LEMBALL 0x004160f0
void CGameObject::GetBoundingBox(CVsRect& p_rect)
{
	p_rect.m_x = (short) (m_position.m_xFixed >> 12) - 24;
	p_rect.m_y = (short) (m_position.m_yFixed >> 12) - 24;
	p_rect.m_width = 48;
	p_rect.m_height = 48;
}

// FUNCTION: LEMBALL 0x00416130
bool CGameObject::Jump()
{
	bool result = false;
	CMover* mover = 0;
	unsigned int actionArgument = (unsigned short) m_actionArgument;
	if (actionArgument != 0) {
		return (bool) actionArgument;
	}

	int elapsed = g_dwGameTick - m_lastMovementTick;
	unsigned int groundZ = g_pMap->GetZ(m_groundPosition.m_xFixed >> 12, m_groundPosition.m_yFixed >> 12, &mover);
	m_position.m_zFixed = (elapsed * 3 + m_flightZ) << 12;
	groundZ <<= 12;
	if (m_position.m_zFixed >= (int) groundZ) {
		AiCoord* position = &m_position;
		m_position.m_xFixed = m_groundPosition.m_xFixed;
		m_position.m_yFixed = m_groundPosition.m_yFixed;
		position->m_zFixed = groundZ;
		m_isJumping = 0;
		if (m_onMover == 0 && mover != 0) {
			if (mover->GetOn(this)) {
				return true;
			}
		}
		g_pAI->StepOn(*position, this, m_collisionFlags);
	}
	return result;
}

// FUNCTION: LEMBALL 0x00416220
bool CGameObject::Fall()
{
	CMover* mover = 0;
	unsigned int actionArgument = (unsigned short) m_actionArgument;
	if (actionArgument != 0) {
		return (bool) actionArgument;
	}
	else {
		AiCoord* position = &m_position;
		int x = position->m_xFixed >> 12;
		int y = position->m_yFixed >> 12;
		m_position.m_zFixed = ((m_lastMovementTick - g_dwGameTick) * 3 + m_flightZ) << 12;

		if ((x & 0xf) != 8) {
			int centeredX;
			if ((x & 0xf) < 8) {
				centeredX = x + 1;
			}
			else {
				centeredX = x - 1;
			}
			position->m_xFixed = centeredX << 12;
		}

		if ((y & 0xf) != 8) {
			int centeredY;
			if ((y & 0xf) < 8) {
				centeredY = y + 1;
			}
			else {
				centeredY = y - 1;
			}
			position->m_yFixed = centeredY << 12;
		}

		int groundZ = (int) g_pMap->GetZ(x, y, &mover) << 12;
		if (m_position.m_zFixed <= groundZ) {
			m_position.m_zFixed = groundZ;
			m_flightVelocity.m_xFixed = 0;
			m_flightVelocity.m_yFixed = 0;
			m_flightVelocity.m_zFixed = 0;
			m_isFalling = 0;
			if (m_onMover == 0 && mover != 0 && mover->GetOn(this)) {
				ResetInstructions();
			}
		}

		if (m_balloonPostId == 0) {
			g_pAI->HitTrampoline(m_position, this);
			if (m_balloonPostId == 0) {
				g_pAI->StepOn(m_position, this, m_collisionFlags);
			}
		}
	}
	return false;
}

// Keeping type completion local preserves the original MSVC 4.00 register allocation in preceding functions.
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CRect3.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Coord3d.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/VsDebug.h"

// FUNCTION: LEMBALL 0x00416340
bool CGameObject::OnLift(Coord3d& p_liftPosition)
{
	if (m_action == ACTION_8) {
		return false;
	}

	int x = m_position.m_xFixed >> 12;
	int left = (int) p_liftPosition.m_x - 8;
	int top = (int) p_liftPosition.m_y - 8;
	int right = (int) p_liftPosition.m_x + 7;
	int bottom = (int) p_liftPosition.m_y + 7;
	int y = m_position.m_yFixed >> 12;
	if (left <= x && right >= x && y >= top && y <= bottom) {
		CMap* map = g_pMap;
		int blockX = left >> 4;
		int blockY = top >> 4;
		unsigned short groundZ;
		if (left < 0 || top < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(left & 0xf, top & 0xf);
		}
		m_position.m_zFixed = (unsigned int) groundZ << 12;
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00416410
void CGameObject::OffLift(Coord3d& p_liftPosition)
{
	OnLift(p_liftPosition);
}

// FUNCTION: LEMBALL 0x00416420
bool CGameObject::OnLift(Coord3d& p_liftMin, Coord3d& p_liftMax)
{
	if (m_action == ACTION_8) {
		return false;
	}

	int left = (int) p_liftMin.m_x - 8;
	int right = (int) p_liftMax.m_x + 7;
	int top = (int) p_liftMin.m_y - 8;
	int y;
	int bottom = (int) p_liftMax.m_y + 7;
	y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	if (x >= left && x <= right && y >= top && y <= bottom) {
		CMap* map = g_pMap;
		int blockX = left >> 4;
		int blockY = top >> 4;
		int width;
		unsigned short groundZ;
		if (left < 0 || top < 0 || (width = map->m_ground.m_width) <= blockX || map->m_ground.m_height <= blockY) {
			groundZ = 0;
		}
		else {
			left &= 0xf;
			top &= 0xf;
			groundZ = map->m_ground.m_ground[width * blockY + blockX].GetZ(left, top);
		}
		m_position.m_zFixed = (unsigned int) groundZ << 12;
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x004164f0
void CGameObject::OffLift(Coord3d& p_liftMin, Coord3d& p_liftMax)
{
	OnLift(p_liftMin, p_liftMax);
}

// FUNCTION: LEMBALL 0x00416510
void CGameObject::StartSommersault()
{
	int random = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	*g_pSentinel = random;
	m_actionDeadline = g_dwGameTick + (random % 500 + 50) / GAME_TICK_MILLISECONDS;
	m_actionArgument = (short) g_dwSommersaultDirection;
	g_dwSommersaultDirection ^= 1;
}

// FUNCTION: LEMBALL 0x00416570
bool CGameObject::IsSelectable()
{
	if (m_action < ACTION_7) {
		goto selectable;
	}
	if (m_action <= ACTION_8 || m_action == ACTION_0x15) {
		return 0;
	}
selectable:
	return 1;
}

// FUNCTION: LEMBALL 0x00416590
void CGameObject::ResetInstructions()
{
	if (m_action != 4 && m_action != 12 && m_destinationList != 0) {
		if (IsSelectable()) {
			m_actionDeadline = g_dwGameTick;
		}
		m_destinationList->m_count = 0;
		if (m_routeSearchActive != 0) {
			g_pMaze->m_routeSearchBusy = 0;
		}
		m_routeSearchFailed = 0;
	}
}

// FUNCTION: LEMBALL 0x004165e0
void CGameObject::Init(CAI* p_ai)
{
	g_pAI = p_ai;
	g_nGameOver = 0;
	memset(g_abObjectIdBitmap, 0, sizeof(g_abObjectIdBitmap));
	g_abObjectIdBitmap[0] |= g_abBitMasks[0];
}

// FUNCTION: LEMBALL 0x00416610
short CGameObject::GetId()
{
	return m_linkedObjectId;
}

// FUNCTION: LEMBALL 0x00416620
void CGameObject::SetId(unsigned short p_id)
{
	m_linkedObjectId = p_id;
	RegisterId();
}

// FUNCTION: LEMBALL 0x00416640
void CGameObject::ReSetId()
{
	if (m_linkedObjectId != (unsigned short) 0xffff) {
		g_abObjectIdBitmap[m_linkedObjectId >> 3] &= ~g_abBitMasks[m_linkedObjectId & 7];
	}
}

// FUNCTION: LEMBALL 0x00416670
short CGameObject::NextId()
{
	int i = 0;
	int j;
	do {
		if (g_abObjectIdBitmap[i] != 0xff) {
			j = 0;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					return j + i * 8;
				}
				j++;
			} while (j < 8);
		}
		i++;
	} while (i < 0x100);
	return 0;
}

// FUNCTION: LEMBALL 0x004166a0
short CGameObject::NextLoadingId()
{
	int i = 0xff;
	do {
		if (g_abObjectIdBitmap[i] != 0xff) {
			int j = 7;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					return j + i * 8;
				}
				j--;
			} while (j > 0);
		}
		i--;
	} while (i > 0);
	return 0;
}

// FUNCTION: LEMBALL 0x004166d0
short CollectUnusedObjectIds(unsigned short* p_ids, int p_capacity)
{
	int count = 0;
	unsigned short* output;
	int i = 0;
	do {
		if (g_abObjectIdBitmap[i] != 0xff) {
			int j = 0;
			output = p_ids + count;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					*output++ = j + i * 8;
					count++;
					if (count == p_capacity) {
						return p_capacity;
					}
				}
				j++;
			} while (j < 8);
		}
		i++;
	} while (i < 0x100);
	return count;
}

// FUNCTION: LEMBALL 0x00416740
void CGameObject::RegisterId()
{
	unsigned short id = m_linkedObjectId;
	if (id != (unsigned short) 0xffff) {
		unsigned short byteIndex = id >> 3;
		unsigned short bitIndex = id & 7;
		unsigned char mask = g_abBitMasks[bitIndex];
		unsigned char* bitmapBytePtr = &g_abObjectIdBitmap[byteIndex];
		unsigned char bitmapByte = *bitmapBytePtr;
		if ((mask & bitmapByte) != 0) {
			unsigned int objectCount = g_wObjectCount;
			for (unsigned int objectIndex = 0; (int) objectIndex < (int) objectCount; objectIndex++) {
				if (g_pObjects[(unsigned short) objectIndex] != 0) {
					g_pObjects[(unsigned short) objectIndex]->GetId();
				}
			}
			m_linkedObjectId = 0xffff;
			return;
		}
		*bitmapBytePtr = mask | bitmapByte;
	}
}

// FUNCTION: LEMBALL 0x004167c0
void CGameObject::UpdateCollision()
{
	int x = (m_position.m_xFixed >> 12) - 8;
	int y = (m_position.m_yFixed >> 12) - 8;
	int z = m_position.m_zFixed >> 12;
	int collision[6];
	collision[0] = x;
	collision[1] = y;
	collision[2] = z;
	collision[3] = x + 15;
	collision[4] = y + 15;
	collision[5] = z + 15;
	memcpy(&m_collisionBounds, collision, sizeof(collision));
}

// FUNCTION: LEMBALL 0x00416820
void CGameObject::StartLand()
{
	m_actionDeadline = g_dwGameTick + 8;
	g_pAI->StepOn(m_position, this, m_collisionFlags);
}

// FUNCTION: LEMBALL 0x00417aa0
bool CGameObject::Process()
{
	return 0;
}

// GLOBAL: LEMBALL 0x0049cf4c
unsigned short g_wNetworkLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049cf50
unsigned short g_wLocalLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049d070
int g_anTurnDelayCursor[16] = {0, 30, 20, 12, 0, 0, 0, 15, 32, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d0b0
int g_anTurnDelayTarget[16] = {0, 87, 75, 0, 0, 0, 0, 75, 75, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d108
unsigned char g_abBitMasks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

// GLOBAL: LEMBALL 0x004a640c
int g_wLemmingCount;

// GLOBAL: LEMBALL 0x004a6410
unsigned char g_abObjectIdBitmap[256];

// GLOBAL: LEMBALL 0x004a6510
CGameObject* g_pObjects[256];

// GLOBAL: LEMBALL 0x004a74bc
unsigned short g_wObjectCount;
