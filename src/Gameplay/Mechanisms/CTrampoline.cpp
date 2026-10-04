#include "CTrampoline.h"

#include "Game/CGame.h"
#include "Game/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

enum {
	TRAMPOLINE_ACTIVATION_RADIUS_PIXELS = 32,
	TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED = 4 * FIXED_POINT_ONE
};

#include "Visos/Math/CFixed.h"
#include "Gameplay/Geometry/C3DVector.h"

#include <stddef.h>

enum {
	TRAMPOLINE_HIT_BOUNDS_INSET_PIXELS = 12,
	TRAMPOLINE_HIT_BOUNDS_SPAN_PIXELS = 24,
	TRAMPOLINE_HIT_BOUNDS_BELOW_BASE_PIXELS = 4,
	TRAMPOLINE_HIT_BOUNDS_ABOVE_BASE_PIXELS = 8,
	TRAMPOLINE_PLANAR_BOUNCE_IMPULSE_FIXED = 0x2000,
	TRAMPOLINE_VERTICAL_BOUNCE_IMPULSE_FIXED = 0x4000,
	TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED = 0x14000,
	TRAMPOLINE_SURFACE_HEIGHT_FIXED = 0x8000,
	TRAMPOLINE_ACTIVE_DURATION_TICKS = 16,
	TRAMPOLINE_SCORE_BONUS_POINTS = 50
};

// FUNCTION: LEMBALL 0x0042a990
CTrampoline::CTrampoline() : CGlobalGameObject(OBJECT_TRAMPOLINE, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042a9b0
void CTrampoline::Restart()
{
	CGlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// FUNCTION: LEMBALL 0x0042a9d0
CTrampoline::~CTrampoline()
{
}

// FUNCTION: LEMBALL 0x0042a9e0
void CTrampoline::Set(unsigned short p_id, const AICOORD& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_action = ACTION_READY;
	m_active = 1;
	m_enabled = 1;

	int blockX = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0) {
		int blockY = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
		if (blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (width > blockX && map->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042aa80
bool CTrampoline::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_RUNNING) {
				SetSndEffect(SFX_TRMPLINE);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	if (m_enabled == 0) {
		return true;
	}
	if (m_action == ACTION_RUNNING && m_actionDeadline < g_dwGameTick) {
		Action(ACTION_READY);
	}
	return true;
}

// FUNCTION: LEMBALL 0x0042aaf0
int CTrampoline::TryEnableNearPosition(const AICOORD& p_position, CGameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) < TRAMPOLINE_ACTIVATION_RADIUS_PIXELS) {
		m_enabled = 1;
		m_lastMovementTick = g_dwGameTick;
		m_stateTimer = g_dwSimulationTimestamp;
		m_position.m_xFixed = p_position.m_xFixed + TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED;
		m_position.m_yFixed = p_position.m_yFixed + TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED;
		int z = p_position.m_zFixed;
		m_position.m_zFixed = z;
		m_relocationZ = z >> FIXED_POINT_FRACTION_BITS;
		p_object->m_deathRequested = 1;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042ab90
int CTrampoline::Hit(const AICOORD& p_position, CGameObject* p_object)
{
	int positionX = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int positionY = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int positionZ = p_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	int minimumX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - TRAMPOLINE_HIT_BOUNDS_INSET_PIXELS;
	int maximumX = minimumX + TRAMPOLINE_HIT_BOUNDS_SPAN_PIXELS;
	int minimumY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - TRAMPOLINE_HIT_BOUNDS_INSET_PIXELS;
	int maximumY = minimumY + TRAMPOLINE_HIT_BOUNDS_SPAN_PIXELS;
	int minimumZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	int maximumZ = minimumZ + TRAMPOLINE_HIT_BOUNDS_ABOVE_BASE_PIXELS;
	minimumZ -= TRAMPOLINE_HIT_BOUNDS_BELOW_BASE_PIXELS;

	if (minimumX > positionX || maximumX < positionX || minimumY > positionY || maximumY < positionY ||
		minimumZ > positionZ || maximumZ < positionZ) {
		return 0;
	}

	C3DVector flightVelocity;
	flightVelocity.m_xFixed = DEBUG_SENTINEL;
	flightVelocity.m_yFixed = DEBUG_SENTINEL;
	flightVelocity.m_zFixed = DEBUG_SENTINEL;
	CFixed incomingVelocity[3] = {CFixed(p_object->m_flightVelocity.m_xFixed),
								  CFixed(p_object->m_flightVelocity.m_yFixed),
								  CFixed(p_object->m_flightVelocity.m_zFixed)};
	if (incomingVelocity[2].m_value < 0) {
		incomingVelocity[2].m_value = -incomingVelocity[2].m_value;
	}

	if (incomingVelocity[1].m_value != 0) {
		if (incomingVelocity[1].m_value > 0) {
			CFixed impulse[3] = {CFixed(0),
								 CFixed(TRAMPOLINE_PLANAR_BOUNCE_IMPULSE_FIXED),
								 CFixed(TRAMPOLINE_VERTICAL_BOUNCE_IMPULSE_FIXED)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(0),
								 CFixed(-TRAMPOLINE_PLANAR_BOUNCE_IMPULSE_FIXED),
								 CFixed(TRAMPOLINE_VERTICAL_BOUNCE_IMPULSE_FIXED)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
	}
	else {
		if (incomingVelocity[0].m_value > 0) {
			CFixed impulse[3] = {CFixed(TRAMPOLINE_PLANAR_BOUNCE_IMPULSE_FIXED),
								 CFixed(0),
								 CFixed(TRAMPOLINE_VERTICAL_BOUNCE_IMPULSE_FIXED)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(-TRAMPOLINE_PLANAR_BOUNCE_IMPULSE_FIXED),
								 CFixed(0),
								 CFixed(TRAMPOLINE_VERTICAL_BOUNCE_IMPULSE_FIXED)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
	}

	if (flightVelocity.m_xFixed > TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED) {
		flightVelocity.m_xFixed = TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED;
	}
	if (flightVelocity.m_xFixed < -TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED) {
		flightVelocity.m_xFixed = -TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED;
	}
	if (flightVelocity.m_yFixed > TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED) {
		flightVelocity.m_yFixed = TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED;
	}
	if (flightVelocity.m_yFixed < -TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED) {
		flightVelocity.m_yFixed = -TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED;
	}
	if (flightVelocity.m_zFixed > TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED) {
		flightVelocity.m_zFixed = TRAMPOLINE_BOUNCE_VELOCITY_LIMIT_FIXED;
	}

	AICOORD position(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed + TRAMPOLINE_SURFACE_HEIGHT_FIXED);
	p_object->m_position = position;
	p_object->StartFly(flightVelocity, NULL);
	p_object->m_balloonPostId = 1;
	p_object->ResetInstructions();
	m_actionDeadline = g_dwGameTick + TRAMPOLINE_ACTIVE_DURATION_TICKS;
	m_stateTimer = g_dwSimulationTimestamp;
	Action(ACTION_RUNNING);
	SetSndEffect(SFX_TRMPLINE);
	g_pAI->Score(TRAMPOLINE_SCORE_BONUS_POINTS);
	return 1;
}

// FUNCTION: LEMBALL 0x0042b9b0
void CTrampoline::DoActivate()
{
}
