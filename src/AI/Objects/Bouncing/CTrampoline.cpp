#include "../CTrampoline.h"

#include "../../../Control/Game/CGame.h"
#include "../../../Control/Game/GameTime.h"
#include "../../../Map/Base/CMap.h"
#include "../../../Visos/Foundation/CFixed.h"
#include "../../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Views/Sound/SoundEffects.h"

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
