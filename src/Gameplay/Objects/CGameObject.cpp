#include "CGameObject.h"

#include "Application/GameMain.h"
#include "Engine/Math/CVector.h"
#include "Engine/Math/RandomConstants.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Mechanisms/CMover.h"
#include "Gameplay/Navigation/CAiDestinationEntry.h"
#include "Gameplay/Navigation/CAiDestinationList.h"
#include "Gameplay/Navigation/CMaze.h"
#include "Gameplay/Navigation/tSolution.h"
#include "Gameplay/Objects/ObjectIds.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"
#include "ObjectInteractionStates.h"

#include <string.h>

#pragma intrinsic(memcpy, memset)

#define PLAYER_ONE_RUNTIME_FLAGS 0x200
#define PLAYER_TWO_RUNTIME_FLAGS 0x100

#include "Engine/Math/CVSRect.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/CRect3.h"
#include "Gameplay/Geometry/tCoord3d.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "ObjectActions.h"
#include "ObjectTypes.h"

// GLOBAL: LEMBALL 0x0049cf4c
unsigned short g_wNetworkLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049cf50
unsigned short g_wLocalLemmingIndex = 0;

// GLOBAL: LEMBALL 0x0049d070
int g_anTurnDelayCursor[16] = {0, 30, 20, 12, 0, 0, 0, 15, 15, 32, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d0b0
int g_anTurnDelayTarget[16] = {0, 87, 75, 0, 0, 0, 0, 75, 75, 0, 0, 0, 0, 0, 0, 0};

// GLOBAL: LEMBALL 0x0049d108
unsigned char g_abBitMasks[OBJECT_ID_BITMAP_BITS_PER_BYTE] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

// GLOBAL: LEMBALL 0x004a640c
int g_wLemmingCount;

// GLOBAL: LEMBALL 0x004a6410
unsigned char g_abObjectIdBitmap[OBJECT_ID_BITMAP_BYTE_CAPACITY];

// GLOBAL: LEMBALL 0x004a6510
CGameObject* g_pObjects[OBJECT_REGISTRY_CAPACITY];

// GLOBAL: LEMBALL 0x004a74bc
unsigned short g_wObjectCount;

// FUNCTION: LEMBALL 0x0040a7f0
void CGameObject::ForgetObjectLink(unsigned short p_arg0)
{
}

// FUNCTION: LEMBALL 0x0040a800
bool CGameObject::Activate(CGameObject* p_object)
{
	return true;
}

// FUNCTION: LEMBALL 0x0040a810
bool CGameObject::IsFlying()
{
	return m_isFlying;
}

// FUNCTION: LEMBALL 0x0040a820
int CGameObject::Usage()
{
	return GROUP_OBJECT_USAGE_NONE;
}

// FUNCTION: LEMBALL 0x0040a830 FOLDED
AICOORD CGameObject::Position()
{
	return AICOORD(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed);
}

// FUNCTION: LEMBALL 0x0040a830 FOLDED
AICOORD CGameObject::ActivatePosition()
{
	return AICOORD(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed);
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
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0040a8e0
bool CGameObject::Collision(const CRect3& p_bounds)
{
	if (m_collisionBounds.m_x1 <= p_bounds.m_x2 && p_bounds.m_x1 <= m_collisionBounds.m_x2 &&
		m_collisionBounds.m_y1 <= p_bounds.m_y2 && p_bounds.m_y1 <= m_collisionBounds.m_y2 &&
		m_collisionBounds.m_z1 <= p_bounds.m_z2 && p_bounds.m_z1 <= m_collisionBounds.m_z2) {
		return true;
	}
	return false;
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
bool CGameObject::IsHit()
{
	return false;
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
	return false;
}

// FUNCTION: LEMBALL 0x0040a9b0
void CGameObject::TurnToFaceCursor()
{
}

// FUNCTION: LEMBALL 0x0040a9c0
bool CGameObject::IsRequestingFire()
{
	return false;
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
	return false;
}

// FUNCTION: LEMBALL 0x0040aa40
void CGameObject::TurnToFaceTarget()
{
}

// FUNCTION: LEMBALL 0x0040aa50
bool CGameObject::PossiblyOnLift()
{
	return true;
}

// FUNCTION: LEMBALL 0x0040aa60
bool CGameObject::HasObject(eObjectType p_objectType)
{
	return false;
}

// FUNCTION: LEMBALL 0x0040aa70
bool CGameObject::AddObject(eObjectType p_objectType, CGameObject* p_object)
{
	return false;
}

// FUNCTION: LEMBALL 0x0040aa80
void CGameObject::ReNumberNode(int p_arg0, int p_arg1)
{
}

// FUNCTION: LEMBALL 0x0040aa90
bool CGameObject::NeedsNode(int p_arg0)
{
	return false;
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
	return NULL;
}

// FUNCTION: LEMBALL 0x0040ab50
bool CGameObject::IsUsable(eAction p_action)
{
	return p_action == ACTION_READY;
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
	return GROUP_OBJECT_REQUEST_ACCEPTED;
}

// FUNCTION: LEMBALL 0x00413050
void ReindexAllObjects()
{
	unsigned int count = g_wObjectCount;
	for (unsigned int index = 0; index < count; ++index) {
		CGameObject* object = g_pObjects[index & OBJECT_ID_MASK];
		if (object != 0 && object->m_objectId != index) {
			object->m_objectId = (unsigned short) index;
		}
	}
}

// FUNCTION: LEMBALL 0x00414f30
CGameObject::CGameObject(eObjectType p_objectType,
						 unsigned short p_collisionFlags,
						 unsigned short p_destinationCapacity)
{
	bool found;
	int i;
	m_objectType = p_objectType;
	m_collisionFlags = p_collisionFlags;
	CAiDestinationList* list;
	if (p_destinationCapacity != 0 && (list = new CAiDestinationList) != NULL) {
		list->m_count = 0;
		list->m_capacity = p_destinationCapacity;
		list->m_entries = new CAiDestinationEntry[p_destinationCapacity];
		m_destinationList = list;
	}
	else {
		m_destinationList = NULL;
	}
	m_linkedObjectId = INVALID_OBJECT_ID;
	found = false;
	i = 0;
	if (0 < g_wObjectCount) {
		do {
			if (g_pObjects[i] == NULL) {
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
	m_invisibleSwitchId = (unsigned short) INVALID_OBJECT_ID;
	m_liftId = INVALID_OBJECT_ID;
	m_position.m_yFixed = 0;
	m_position.m_xFixed = 0;
	if (m_destinationList != NULL) {
		m_destinationList->m_count = 0;
	}
	switch (m_objectType) {
	case OBJECT_PLAYER_1:
		m_runtimeFlags = PLAYER_ONE_RUNTIME_FLAGS;
		break;
	case OBJECT_PLAYER_2:
		m_runtimeFlags = PLAYER_TWO_RUNTIME_FLAGS;
		break;
	}
}

// FUNCTION: LEMBALL 0x00415160
CGameObject::~CGameObject()
{
	ReSetId();
	g_pObjects[m_objectId] = NULL;
	CAiDestinationList* destinationList = m_destinationList;
	if (destinationList != NULL) {
		operator delete(destinationList->m_entries);
		operator delete(destinationList);
	}
	m_objectId = INVALID_OBJECT_REGISTRY_INDEX;
}

// FUNCTION: LEMBALL 0x004151b0
void CGameObject::Initialise()
{
	m_actionArgument = 0;
	m_deathRequested = false;
	m_action = ACTION_NONE;
	m_isRemoteObject = 0;
	m_facingDirection = 0;
	m_objectActive = 0;
	m_unk0xc4 = 0;
	m_initiallyActive = 0;
	m_isFlying = false;
	m_hidden = 0;
	m_activationReserved = 0;
	m_routeSearchFailed = false;
	m_routeSearchActive = false;
	m_isJumping = false;
	m_isFalling = false;
	m_wasHitByMine = false;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	m_flightVelocity.m_xFixed = 0;
	m_flightVelocity.m_yFixed = 0;
	m_desiredFacingDirection = 0;
	m_flightVelocity.m_zFixed = 0;
	m_unk0x58 = 0;
	m_onMover = false;
	m_hasDestination = 0;
	m_actionDeadline = g_dwGameTick;
	m_soundEffect = SFX_NONE;
	m_transientFlags = 0;
}

// FUNCTION: LEMBALL 0x00415240
void CGameObject::StartFly(C3DVector& p_velocity, C3DVector* p_origin)
{
	m_isJumping = false;
	m_isFalling = false;
	m_balloonPostActive = 0;
	m_balloonPostId = 0;
	if (p_origin != NULL) {
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
	m_isFlying = true;
	m_flightVelocity.m_xFixed = p_velocity.m_xFixed;
	m_flightVelocity.m_yFixed = p_velocity.m_yFixed;
	m_flightVelocity.m_zFixed = p_velocity.m_zFixed;
	m_stateTimer = g_dwSimulationTimestamp;
	m_lastMovementTick = g_dwGameTick;
	m_actionDeadline = g_dwGameTick + GAME_OBJECT_FLIGHT_START_DELAY_TICKS;
}

// FUNCTION: LEMBALL 0x00415300
void CGameObject::Fly()
{
	enum {
		GAME_OBJECT_FLIGHT_GRAVITY_FIXED_PER_TICK = 2 * FIXED_POINT_ONE,
		GAME_OBJECT_FLIGHT_POSITION_ADJUSTMENT_FIXED = 4 * FIXED_POINT_ONE,
		GAME_OBJECT_FLIGHT_TERMINAL_DOWNWARD_SPEED_FIXED = 10 * FIXED_POINT_ONE,
		GAME_OBJECT_FALL_HORIZONTAL_SPEED_FIXED = 3 * FIXED_POINT_ONE
	};
	int timeDelta = (int) (g_dwGameTick - m_lastMovementTick);
	if (timeDelta > 0) {
		m_lastMovementTick = g_dwGameTick;
		int x;
		int z;
		int y;
		x = m_flightVelocity.m_xFixed * 2 + m_flightOrigin.m_xFixed;
		y = m_flightVelocity.m_yFixed * 2 + m_flightOrigin.m_yFixed;
		m_flightOrigin.m_xFixed = x;
		m_flightOrigin.m_yFixed = y;
		m_flightVelocity.m_zFixed -= GAME_OBJECT_FLIGHT_GRAVITY_FIXED_PER_TICK;
		z = m_flightVelocity.m_zFixed * 2 + GAME_OBJECT_FLIGHT_POSITION_ADJUSTMENT_FIXED + m_flightOrigin.m_zFixed;
		m_flightOrigin.m_zFixed = z;
		if (m_flightVelocity.m_zFixed < -GAME_OBJECT_FLIGHT_TERMINAL_DOWNWARD_SPEED_FIXED) {
			m_flightVelocity.m_zFixed = -GAME_OBJECT_FLIGHT_TERMINAL_DOWNWARD_SPEED_FIXED;
		}
		CMover* mover = NULL;
		int groundZ = g_pMap->GetZ(x >> FIXED_POINT_FRACTION_BITS, y >> FIXED_POINT_FRACTION_BITS, &mover);
		int flightZ = z >> FIXED_POINT_FRACTION_BITS;
		if (flightZ <= groundZ) {
			m_isFlying = false;
			m_balloonPostId = 0;
			if (groundZ - 12 >= flightZ) {
				m_actionDeadline = g_dwGameTick;
				if ((m_collisionFlags & GAME_OBJECT_COLLISION_ALLOW_FALL) != 0) {
					m_isFalling = true;
					m_flightVelocity.m_xFixed = GAME_OBJECT_FALL_HORIZONTAL_SPEED_FIXED;
					m_flightVelocity.m_yFixed = 0;
					int objectZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
					m_flightVelocity.m_zFixed = ((objectZ - groundZ) / 8 + 1) * FIXED_POINT_ONE;
					m_lastMovementTick = g_dwGameTick;
					m_flightZ = objectZ;
					m_groundPosition.m_yFixed = m_position.m_yFixed;
					m_groundPosition.m_xFixed = m_position.m_xFixed;
					m_actionArgument = 0;
					m_groundPosition.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
				}
			}
			else {
				m_position.m_xFixed = x;
				m_position.m_yFixed = y;
				m_position.m_zFixed = z;
				m_position.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
				if (g_pAI->HitTrampoline(m_position, this) == 0) {
					m_flightVelocity.m_xFixed = 0;
					m_flightVelocity.m_yFixed = 0;
					m_flightVelocity.m_zFixed = 0;
				}
				else {
					m_isFlying = true;
				}
				if (m_onMover == 0 && mover != NULL) {
					mover->GetOn(this);
				}
			}
		}
		else {
			m_position.m_xFixed = x;
			m_position.m_yFixed = y;
			m_position.m_zFixed = z;
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
	if (m_facingDirection >= FACING_DIRECTION_COUNT) {
		unsigned short turns = (unsigned short) m_facingDirection / FACING_DIRECTION_COUNT;
		m_facingDirection -= turns * FACING_DIRECTION_COUNT;
	}
}

// FUNCTION: LEMBALL 0x00415550
void CGameObject::RotateAnticlockwise()
{
	m_facingDirection--;
	if (m_facingDirection < 0) {
		unsigned short turns = (unsigned short) (FACING_DIRECTION_MASK - m_facingDirection) / FACING_DIRECTION_COUNT;
		m_facingDirection += turns * FACING_DIRECTION_COUNT;
	}
}

// FUNCTION: LEMBALL 0x00415580
void CGameObject::StartMoving()
{
	enum {
		GAME_OBJECT_FALL_HORIZONTAL_SPEED_FIXED = 3 * FIXED_POINT_ONE
	};
	if (m_destinationList != NULL) {
		CMover* mover = NULL;
		const int& groundZ = g_pMap->GetZ(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
										  m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
										  &mover);
		int objectZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
		if (m_onMover == 0 && mover != NULL) {
			mover->GetOn(this);
		}
		if (objectZ == groundZ) {
			m_destination = GetDestination();
			int distance = Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
									m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			m_lastMovementTick = g_dwGameTick;
			m_moveDurationTicks = (g_anTurnDelayCursor[m_objectType] * distance) / GAME_TICK_MILLISECONDS;
			if (m_moveDurationTicks == 0) {
				m_moveDurationTicks = 1;
			}
			m_actionDeadline = m_moveDurationTicks + g_dwGameTick;
			CVector start(m_position.m_xFixed, m_position.m_yFixed);
			CVector end(m_destination.m_xFixed, m_destination.m_yFixed);
			m_movement.SetEndpoints(start, end);
		}
		else if (m_balloonPostActive == 0) {
			m_actionDeadline = g_dwGameTick;
			if ((m_collisionFlags & GAME_OBJECT_COLLISION_ALLOW_FALL) != 0) {
				m_flightVelocity.m_yFixed = 0;
				m_isFalling = true;
				m_flightVelocity.m_xFixed = GAME_OBJECT_FALL_HORIZONTAL_SPEED_FIXED;
				const int& fallSteps = (objectZ - groundZ) / 8;
				m_flightVelocity.m_zFixed = (fallSteps + 1) << FIXED_POINT_FRACTION_BITS;
				short& actionArgument = m_actionArgument;
				actionArgument = 0;
				m_lastMovementTick = g_dwGameTick;
				m_flightZ = objectZ;
				m_groundPosition.m_xFixed = m_position.m_xFixed;
				m_groundPosition.m_yFixed = m_position.m_yFixed;
				m_groundPosition.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
			}
		}
		else {
			m_position.m_zFixed = groundZ << FIXED_POINT_FRACTION_BITS;
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
	int blockY = p_y / GROUND_BLOCK_PIXEL_SIZE;
	int blockX = p_x / GROUND_BLOCK_PIXEL_SIZE;
	unsigned short collision = 0;

	for (int x = blockX; x <= blockX; x++) {
		for (int y = blockY; y <= blockY; y++) {
			int width;
			CMap* map;
			unsigned short cellCollision;
			if (x >= 0 && y >= 0 && x < (width = g_pMap->m_ground.m_width) && y < (map = g_pMap)->m_ground.m_height) {
				cellCollision = map->m_ground.m_ground[y * width + x].m_collision;
			}
			else {
				cellCollision = GROUND_COLLISION_OUT_OF_BOUNDS;
			}
			collision |= cellCollision;
		}
	}
	return collision;
}

// FUNCTION: LEMBALL 0x00415830
bool CGameObject::StartRoute()
{
	bool* routeSearchBusy = &g_pMaze->m_routeSearchBusy;
	m_routeSearchActive = *routeSearchBusy == 0;
	*routeSearchBusy = true;
	if (m_routeSearchActive != 0) {
		g_pMaze->BInitialise(0,
							 (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
							 (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
							 (m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
							 (m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE);
	}
	return false;
}

// FUNCTION: LEMBALL 0x004158b0
bool CGameObject::SearchRoute()
{
	enum {
		GAME_OBJECT_ROUTE_SOLUTION_CAPACITY = 120,
		GAME_OBJECT_ROUTE_MAX_ACCEPTED_SOLUTIONS = 80,
		GAME_OBJECT_ROUTE_COORDINATE_FIXED_SHIFT = FIXED_POINT_FRACTION_BITS + GROUND_BLOCK_PIXEL_SHIFT
	};
	if (m_routeSearchActive != 0) {
		int solutionCount;
		bool complete;
		unsigned int reached;
		unsigned int noChanges;
		tSolution solutions[GAME_OBJECT_ROUTE_SOLUTION_CAPACITY];

		complete = g_pMaze->BIteration(reached, noChanges);
		m_routeSearchFailed = complete == 0;
		if (reached != 0) {
			g_pMaze->BSolution(solutionCount, solutions);
			m_destinationList->m_count = 0;
			if (solutionCount < GAME_OBJECT_ROUTE_MAX_ACCEPTED_SOLUTIONS) {
				int index = solutionCount - 1;
				if (index >= 0) {
					tSolution* solution = &solutions[index];
					do {
						CAiDestinationList* list;
						AICOORD coordinate;
						coordinate.m_xFixed = ((unsigned int) (unsigned short) solution->m_x
											   << GAME_OBJECT_ROUTE_COORDINATE_FIXED_SHIFT) +
											  (GROUND_BLOCK_PIXEL_SIZE / 2) * FIXED_POINT_ONE;
						list = m_destinationList;
						coordinate.m_yFixed = ((unsigned int) (unsigned short) solution->m_y
											   << GAME_OBJECT_ROUTE_COORDINATE_FIXED_SHIFT) +
											  (GROUND_BLOCK_PIXEL_SIZE / 2) * FIXED_POINT_ONE;
						coordinate.m_zFixed = 0;
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
			g_pMaze->m_routeSearchBusy = false;
			m_routeSearchActive = false;
		}
	}
	else {
		bool* routeSearchBusy = &g_pMaze->m_routeSearchBusy;
		m_routeSearchActive = *routeSearchBusy == 0;
		*routeSearchBusy = true;
		if (m_routeSearchActive != 0) {
			g_pMaze->BInitialise(0,
								 (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
								 (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
								 (m_destination.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE,
								 (m_destination.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE);
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x00415a20
void CGameObject::Blocked()
{
	if ((m_collisionFlags & GAME_OBJECT_COLLISION_AFFECT_ROUTE_ON_BLOCK) != 0) {
		m_routeSearchFailed = true;
	}
}

// FUNCTION: LEMBALL 0x00415a30
bool CGameObject::Move()
{
	int elapsed = (int) (g_dwGameTick - m_lastMovementTick);
	AICOORD position;
	position.m_xFixed = m_movement.m_start.m_xFixed + (m_movement.m_delta.m_xFixed * elapsed) / m_moveDurationTicks;
	position.m_yFixed = m_movement.m_start.m_yFixed + (m_movement.m_delta.m_yFixed * elapsed) / m_moveDurationTicks;
	int x = position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map;
	if (x < 0 || (x >> GROUND_BLOCK_PIXEL_SHIFT) >= (map = g_pMap)->m_ground.m_width || y < 0 ||
		(y >> GROUND_BLOCK_PIXEL_SHIFT) >= map->m_ground.m_height) {
		m_actionDeadline = g_dwGameTick;
		return false;
	}

	int height;
	unsigned short currentGroundZ;
	{
		CMover* mover = NULL;
		height = map->GetZ(x, y, &mover);
		if (m_onMover == 0 && mover != NULL) {
			mover->GetOn(this);
		}
		if ((MapCheck(x, y) & GROUND_COLLISION_BLOCKS_WALKING) != 0) {
			position.m_xFixed = x << FIXED_POINT_FRACTION_BITS;
			position.m_yFixed = y << FIXED_POINT_FRACTION_BITS;
			position.m_zFixed = height << FIXED_POINT_FRACTION_BITS;
			if (g_pAI->OpenDoor(position, this, m_collisionFlags)) {
				m_actionDeadline = g_dwGameTick;
				return false;
			}
			Blocked();
			m_actionDeadline = g_dwGameTick;
			return false;
		}

		currentGroundZ = g_pMap->GetZ(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									  m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
									  &mover);
	}
	if ((int) currentGroundZ + 7 <= (int) height) {
		m_actionDeadline = g_dwGameTick;
		if ((int) currentGroundZ + 15 < (int) height) {
			Blocked();
			return false;
		}
		if ((m_collisionFlags & GAME_OBJECT_COLLISION_ALLOW_JUMP) != 0) {
			m_isJumping = true;
			m_lastMovementTick = g_dwGameTick;
			m_flightZ = currentGroundZ;
			m_groundPosition.m_xFixed = position.m_xFixed;
			m_groundPosition.m_yFixed = position.m_yFixed;
			m_groundPosition.m_zFixed = (int) height << FIXED_POINT_FRACTION_BITS;
			m_actionArgument = 0;
			return false;
		}
		Blocked();
		return false;
	}

	if ((int) height <= (int) currentGroundZ - 7) {
		m_actionDeadline = g_dwGameTick;
		if ((m_collisionFlags & GAME_OBJECT_COLLISION_ALLOW_FALL) != 0) {
			m_isFalling = true;
			unsigned int movementTick = g_dwGameTick;
			int velocityY = 0;
			m_actionArgument = 0;
			m_lastMovementTick = movementTick;
			m_flightZ = currentGroundZ;
			int deltaZ = (int) currentGroundZ - height;
			int deltaX = x - (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
			int deltaY = y - (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
			int velocityX;
			int magnitudeX = deltaX < 0 ? -deltaX : deltaX;
			int magnitudeY = deltaY < 0 ? -deltaY : deltaY;
			if (magnitudeX > magnitudeY) {
				velocityX = deltaX <= 0 ? -1 : 1;
			}
			else {
				velocityX = 0;
				velocityY = deltaY <= 0 ? -1 : 1;
			}
			m_position.m_xFixed = position.m_xFixed;
			m_position.m_yFixed = position.m_yFixed;
			m_groundPosition.m_xFixed = position.m_xFixed;
			m_groundPosition.m_yFixed = position.m_yFixed;
			m_groundPosition.m_zFixed = (int) height << FIXED_POINT_FRACTION_BITS;
			m_position.m_xFixed = position.m_xFixed;
			m_position.m_yFixed = position.m_yFixed;
			m_flightVelocity.m_xFixed = velocityX << FIXED_POINT_FRACTION_BITS;
			m_flightVelocity.m_yFixed = velocityY << FIXED_POINT_FRACTION_BITS;
			m_flightVelocity.m_zFixed = ((deltaZ / 8) + 1) * FIXED_POINT_ONE;
			return false;
		}
		Blocked();
		return false;
	}

	m_position.m_xFixed = position.m_xFixed;
	m_position.m_yFixed = position.m_yFixed;
	m_position.m_zFixed = (int) height << FIXED_POINT_FRACTION_BITS;
	g_pAI->StepOn(m_position, this, m_collisionFlags);
	return true;
}

// FUNCTION: LEMBALL 0x00415d90
void CGameObject::TurnToFaceDestination()
{
	AICOORD destination = GetDestination();
	int direction = (int) ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
												destination.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												destination.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	if (direction != m_facingDirection) {
		if (g_anRotationDirections[(direction - (int) m_facingDirection) & FACING_DIRECTION_MASK] < 0) {
			RotateAnticlockwise();
		}
		else {
			RotateClockwise();
		}
	}
	m_actionDeadline = g_dwGameTick + g_anTurnDelayTarget[m_objectType] / GAME_TICK_MILLISECONDS;
}

// FUNCTION: LEMBALL 0x00415e20
bool CGameObject::FacingDestination()
{
	AICOORD dest = GetDestination();
	int dir = ReturnFacingDirection(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
									dest.m_xFixed >> FIXED_POINT_FRACTION_BITS,
									dest.m_yFixed >> FIXED_POINT_FRACTION_BITS);
	return (int) m_facingDirection == dir;
}

// FUNCTION: LEMBALL 0x00415e80
void CGameObject::DeleteFirstEntryFromDestinationList()
{
	CAiDestinationList* list = m_destinationList;
	int i;
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
void CGameObject::AddDestination(const AICOORD& p_destination)
{
	CAiDestinationList* list = m_destinationList;
	if (list != NULL && list->m_count < list->m_capacity) {
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
void CGameObject::AlterDestination(const AICOORD& p_destination)
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
AICOORD CGameObject::GetDestination()
{
	if (m_destinationList->m_count > 0) {
		return m_destinationList->m_entries[0].GetCoordinate();
	}
	return AICOORD(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed);
}

// FUNCTION: LEMBALL 0x00416050
AICOORD CGameObject::GetNextDestination()
{
	CAiDestinationList* list = m_destinationList;
	if (list->m_count != 0) {
		for (int index = 0; index < list->m_count - 1; ++index) {
			CAiDestinationEntry* entry = &list->m_entries[index];
			CAiDestinationEntry* next = entry + 1;
			entry->m_type = next->m_type;
			entry->m_coordinate.m_xFixed = next->m_coordinate.m_xFixed;
			entry->m_coordinate.m_yFixed = next->m_coordinate.m_yFixed;
			entry->m_coordinate.m_zFixed = next->m_coordinate.m_zFixed;
			entry->m_metadata = next->m_metadata;
		}
		--list->m_count;
	}
	return GetDestination();
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
void CGameObject::GetBoundingBox(CVSRect& p_rect)
{
	p_rect.m_x = (short) (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 24;
	p_rect.m_y = (short) (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 24;
	p_rect.m_width = 48;
	p_rect.m_height = 48;
}

// FUNCTION: LEMBALL 0x00416130
void CGameObject::Jump()
{
	CMover* mover = NULL;
	unsigned int actionArgumentValue = (unsigned short) m_actionArgument;
	const unsigned int& actionArgument = actionArgumentValue;
	if (actionArgument != 0) {
		return;
	}

	int elapsed = g_dwGameTick - m_lastMovementTick;
	unsigned int groundHeightValue = g_pMap->GetZ(m_groundPosition.m_xFixed >> FIXED_POINT_FRACTION_BITS,
												  m_groundPosition.m_yFixed >> FIXED_POINT_FRACTION_BITS,
												  &mover);
	const unsigned int& groundHeight = groundHeightValue;
	int& positionZ = m_position.m_zFixed;
	unsigned int groundZ = groundHeight << FIXED_POINT_FRACTION_BITS;
	positionZ = (elapsed * 3 + m_flightZ) << FIXED_POINT_FRACTION_BITS;
	if (m_position.m_zFixed >= (int) groundZ) {
		AICOORD* position = &m_position;
		m_position = m_groundPosition;
		m_position.m_zFixed = groundZ;
		m_isJumping = false;
		if (m_onMover == 0 && mover != NULL) {
			if (!mover->GetOn(this)) {
				g_pAI->StepOn(*position, this, m_collisionFlags);
			}
		}
		else {
			g_pAI->StepOn(*position, this, m_collisionFlags);
		}
	}
}

// FUNCTION: LEMBALL 0x00416220
bool CGameObject::Fall()
{
	CMover* mover = NULL;
	unsigned int actionArgument = (unsigned short) m_actionArgument;
	if (actionArgument != 0) {
		return (bool) actionArgument;
	}
	else {
		AICOORD* position = &m_position;
		int x = position->m_xFixed >> FIXED_POINT_FRACTION_BITS;
		int y = position->m_yFixed >> FIXED_POINT_FRACTION_BITS;
		m_position.m_zFixed = ((m_lastMovementTick - g_dwGameTick) * 3 + m_flightZ) << FIXED_POINT_FRACTION_BITS;

		if ((x & GROUND_BLOCK_PIXEL_MASK) != GROUND_BLOCK_PIXEL_HALF_SIZE) {
			int centreedX;
			if ((x & GROUND_BLOCK_PIXEL_MASK) < GROUND_BLOCK_PIXEL_HALF_SIZE) {
				centreedX = x + 1;
			}
			else {
				centreedX = x - 1;
			}
			position->m_xFixed = centreedX * FIXED_POINT_ONE;
		}

		if ((y & GROUND_BLOCK_PIXEL_MASK) != GROUND_BLOCK_PIXEL_HALF_SIZE) {
			int centreedY;
			if ((y & GROUND_BLOCK_PIXEL_MASK) < GROUND_BLOCK_PIXEL_HALF_SIZE) {
				centreedY = y + 1;
			}
			else {
				centreedY = y - 1;
			}
			m_position.m_yFixed = centreedY * FIXED_POINT_ONE;
		}

		int groundZ = (int) g_pMap->GetZ(x, y, &mover) << FIXED_POINT_FRACTION_BITS;
		if (m_position.m_zFixed <= groundZ) {
			m_position.m_zFixed = groundZ;
			m_flightVelocity.m_xFixed = 0;
			m_flightVelocity.m_yFixed = 0;
			m_flightVelocity.m_zFixed = 0;
			m_isFalling = false;
			if (m_onMover == 0 && mover != NULL && mover->GetOn(this)) {
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

// FUNCTION: LEMBALL 0x00416340
bool CGameObject::OnLift(tCoord3d& p_liftPosition)
{
	if (m_action == ACTION_DEAD) {
		return false;
	}

	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int left = (int) p_liftPosition.m_x - 8;
	int top = (int) p_liftPosition.m_y - 8;
	int right = (int) p_liftPosition.m_x + 7;
	int bottom = (int) p_liftPosition.m_y + 7;
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	if (left > x || right < x || y < top || y > bottom) {
		return false;
	}
	CMap* map = g_pMap;
	int blockX = left >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = top >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short groundZ;
	if (left < 0 || top < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
		groundZ = 0;
	}
	else {
		int offsetX = left & GROUND_BLOCK_PIXEL_MASK;
		int offsetY = top & GROUND_BLOCK_PIXEL_MASK;
		groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(offsetX, offsetY);
	}
	m_position.m_zFixed = (unsigned int) groundZ << FIXED_POINT_FRACTION_BITS;
	return true;
}

// FUNCTION: LEMBALL 0x00416410
void CGameObject::OffLift(tCoord3d& p_liftPosition)
{
	OnLift(p_liftPosition);
}

// FUNCTION: LEMBALL 0x00416420
bool CGameObject::OnLift(tCoord3d& p_liftMin, tCoord3d& p_liftMax)
{
	if (m_action == ACTION_DEAD) {
		return false;
	}

	int left = (int) p_liftMin.m_x - 8;
	int right = (int) p_liftMax.m_x + 7;
	int top = (int) p_liftMin.m_y - 8;
	int y;
	int bottom = (int) p_liftMax.m_y + 7;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	if (x < left || x > right || y < top || y > bottom) {
		return false;
	}
	CMap* map = g_pMap;
	int blockX = left >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = top >> GROUND_BLOCK_PIXEL_SHIFT;
	int width;
	unsigned short groundZ;
	if (left < 0 || top < 0 || (width = map->m_ground.m_width) <= blockX || map->m_ground.m_height <= blockY) {
		groundZ = 0;
	}
	else {
		left &= GROUND_BLOCK_PIXEL_MASK;
		top &= GROUND_BLOCK_PIXEL_MASK;
		groundZ = map->m_ground.m_ground[width * blockY + blockX].GetZ(left, top);
	}
	m_position.m_zFixed = (unsigned int) groundZ << FIXED_POINT_FRACTION_BITS;
	return true;
}

// FUNCTION: LEMBALL 0x004164f0
void CGameObject::OffLift(tCoord3d& p_liftMin, tCoord3d& p_liftMax)
{
	OnLift(p_liftMin, p_liftMax);
}

// FUNCTION: LEMBALL 0x00416510
void CGameObject::StartSommersault()
{
	enum {
		GAME_OBJECT_SOMERSAULT_MINIMUM_DURATION_MS = 50,
		GAME_OBJECT_SOMERSAULT_RANDOM_DURATION_RANGE_MS = 500
	};
	int random = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
	*g_pRandomSeed = random;
	m_actionDeadline = g_dwGameTick + (random % GAME_OBJECT_SOMERSAULT_RANDOM_DURATION_RANGE_MS +
									   GAME_OBJECT_SOMERSAULT_MINIMUM_DURATION_MS) /
										  GAME_TICK_MILLISECONDS;
	m_actionArgument = (short) g_dwSommersaultDirection;
	g_dwSommersaultDirection ^= SOMMERSAULT_DIRECTION_REVERSED;
}

// FUNCTION: LEMBALL 0x00416570
bool CGameObject::IsSelectable()
{
	if (m_action < ACTION_HIT) {
		goto selectable;
	}
	if (m_action <= ACTION_DEAD || m_action == ACTION_WAITING_TO_DIE) {
		return false;
	}
selectable:
	return true;
}

// FUNCTION: LEMBALL 0x00416590
void CGameObject::ResetInstructions()
{
	if (m_action != ACTION_FLYING && m_action != ACTION_WAITING_TO_SPAWN && m_destinationList != NULL) {
		if (IsSelectable()) {
			m_actionDeadline = g_dwGameTick;
		}
		m_destinationList->m_count = 0;
		if (m_routeSearchActive != 0) {
			g_pMaze->m_routeSearchBusy = false;
		}
		m_routeSearchFailed = false;
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
	if (m_linkedObjectId != INVALID_OBJECT_ID) {
		g_abObjectIdBitmap[m_linkedObjectId >> OBJECT_ID_BITMAP_BYTE_INDEX_SHIFT] &=
			~g_abBitMasks[m_linkedObjectId & OBJECT_ID_BITMAP_BIT_INDEX_MASK];
	}
}

// FUNCTION: LEMBALL 0x00416670
short CGameObject::NextId()
{
	int i = 0;
	int j;
	do {
		if (g_abObjectIdBitmap[i] != OBJECT_ID_BITMAP_BYTE_FULL_MASK) {
			j = 0;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					return j + i * OBJECT_ID_BITMAP_BITS_PER_BYTE;
				}
				j++;
			} while (j < OBJECT_ID_BITMAP_BITS_PER_BYTE);
		}
		i++;
	} while (i < OBJECT_ID_BITMAP_BYTE_CAPACITY);
	return 0;
}

// FUNCTION: LEMBALL 0x004166a0
short CGameObject::NextLoadingId()
{
	int i = OBJECT_ID_BITMAP_BYTE_CAPACITY - 1;
	do {
		if (g_abObjectIdBitmap[i] != OBJECT_ID_BITMAP_BYTE_FULL_MASK) {
			int j = OBJECT_ID_BITMAP_BIT_INDEX_MASK;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					return j + i * OBJECT_ID_BITMAP_BITS_PER_BYTE;
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
		if (g_abObjectIdBitmap[i] != OBJECT_ID_BITMAP_BYTE_FULL_MASK) {
			int j = 0;
			output = p_ids + count;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					*output++ = j + i * OBJECT_ID_BITMAP_BITS_PER_BYTE;
					count++;
					if (count == p_capacity) {
						return p_capacity;
					}
				}
				j++;
			} while (j < OBJECT_ID_BITMAP_BITS_PER_BYTE);
		}
		i++;
	} while (i < OBJECT_ID_BITMAP_BYTE_CAPACITY);
	return count;
}

// FUNCTION: LEMBALL 0x00416740
void CGameObject::RegisterId()
{
	unsigned short id = m_linkedObjectId;
	if (id != INVALID_OBJECT_ID) {
		unsigned short byteIndex = id >> OBJECT_ID_BITMAP_BYTE_INDEX_SHIFT;
		unsigned short bitIndex = id & OBJECT_ID_BITMAP_BIT_INDEX_MASK;
		unsigned short mask = g_abBitMasks[bitIndex];
		unsigned char* bitmapBytePtr = &g_abObjectIdBitmap[byteIndex];
		unsigned char bitmapByte = *bitmapBytePtr;
		if (((unsigned int) mask & (unsigned int) bitmapByte) != 0) {
			unsigned int objectCount = g_wObjectCount;
			for (unsigned int objectIndex = 0; (int) objectIndex < (int) objectCount; objectIndex++) {
				if (g_pObjects[(unsigned short) objectIndex] != NULL) {
					g_pObjects[(unsigned short) objectIndex]->GetId();
				}
			}
			m_linkedObjectId = INVALID_OBJECT_ID;
			return;
		}
		*bitmapBytePtr = mask | bitmapByte;
	}
}

// FUNCTION: LEMBALL 0x004167c0
void CGameObject::UpdateCollision()
{
	int x;
	int y;
	int z;
	x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - GAME_OBJECT_COLLISION_XY_MIN_INSET;
	z = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	int collision[6];
	collision[0] = x;
	collision[1] = y;
	collision[2] = z;
	collision[3] = x + GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET;
	collision[4] = y + GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET;
	collision[5] = z + GAME_OBJECT_COLLISION_BOX_LAST_PIXEL_OFFSET;
	memcpy(&m_collisionBounds, collision, sizeof(collision));
}

// FUNCTION: LEMBALL 0x00416820
void CGameObject::StartLand()
{
	m_actionDeadline = g_dwGameTick + GAME_OBJECT_LANDING_TRANSITION_DELAY_TICKS;
	g_pAI->StepOn(m_position, this, m_collisionFlags);
}

// FUNCTION: LEMBALL 0x00417aa0
bool CGameObject::Process()
{
	return false;
}

#undef PLAYER_ONE_RUNTIME_FLAGS
#undef PLAYER_TWO_RUNTIME_FLAGS
