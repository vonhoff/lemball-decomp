#include "StateMachine.h"

#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/GameTime.h"

// SIZE 0x10
struct tStateEntry {
	bool (*m_predicate)(CAI*, CGameObject*, tInfo*);      // 0x00
	void (*m_actionFunction)(CAI*, CGameObject*, tInfo*); // 0x04
	eAction m_nextAction;                                 // 0x08
	eSoundEffect m_soundEffect;                           // 0x0c
};
#include "Application/SoundEffects.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x0049d198
tStateEntry g_userLemmingStateEntries[] = {
	{GameOver, StartSommersault, ACTION_PREPARING_SOMMERSAULT, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{IsStuck, StartRoute, ACTION_FINDING_ROUTE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{IsFalling, NULL, ACTION_FALLING, SFX_NONE},
	{QOnBalloon, StartBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{PlayerRequestingFire, PlayerFire, ACTION_NONE, SFX_NONE},
	{PlayerNotFacingTarget, PlayerTurnToFaceTarget, ACTION_TURNING, SFX_NONE},
	{NotFacingDestination, TurnToFaceDestination, ACTION_TURNING, SFX_NONE},
	{GotDestination, StartWalking, ACTION_WALKING, SFX_NONE},
	{PlayerBored, PlayerRandomAction, ACTION_IDLE_ANIMATION, SFX_NONE},
	{PlayerNotFacingCursor, PlayerTurnToFaceCursor, ACTION_TURNING, SFX_NONE},
	{NULL, PlayerTurnToFaceCursor, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{NotTimeUp, NULL, ACTION_TURNING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{IsJumping, NULL, ACTION_JUMPING, SFX_NONE},
	{IsFalling, NULL, ACTION_FALLING, SFX_NONE},
	{PlayerRequestingFire, NULL, ACTION_NONE, SFX_NONE},
	{AtDestination, PlayerStopWalking, ACTION_NONE, SFX_NONE},
	{NULL, Walk, ACTION_WALKING, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{PlayerWaitingToFire, NULL, ACTION_FIRING, SFX_NONE},
	{PlayerRequestingFire, PlayerFire, ACTION_FIRING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_FIRING, SFX_NONE},
	{NULL, PlayerEndFiring, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{NULL, NULL, ACTION_HIDDEN, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{PlayerRequestingFire, NULL, ACTION_NONE, SFX_NONE},
	{PlayerNotFacingTarget, PlayerTurnToFaceTarget, ACTION_TURNING, SFX_NONE},
	{GotDestination, NULL, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_IDLE_ANIMATION, SFX_NONE},
	{NULL, StartStanding, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_HIT, SFX_NONE},
	{NULL, Die, ACTION_DEAD, SFX_NONE},
	{NULL, NULL, ACTION_DEAD, SFX_NONE},
	{IsStuck, SearchRoute, ACTION_FINDING_ROUTE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsJumping, Jump, ACTION_JUMPING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{IsFalling, Fall, ACTION_FALLING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_WAITING_TO_SPAWN, SFX_NONE},
	{NULL, StartWalking, ACTION_FALLING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_PREPARING_SOMMERSAULT, SFX_NONE},
	{NULL, NULL, ACTION_SOMMERSAULT, SFX_NONE},
	{NULL, NULL, ACTION_SOMMERSAULT, SFX_NONE},
	{NotTimeUp, NULL, ACTION_EXTERNAL_CONTROL, SFX_NONE},
	{NULL, ExternalControlEnd, ACTION_KEEP_CURRENT, SFX_NONE},
	{QOnBalloon, OnBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_LANDING, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NULL, Land, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_DEAD, SFX_NONE},
	{NotTimeUp, NULL, ACTION_WAITING_TO_DIE, SFX_NONE},
	{NULL, Die, ACTION_DEAD, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{QOnBalloon, StartBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{NULL, NULL, ACTION_ON_CONVEYOR, SFX_NONE},
};

// GLOBAL: LEMBALL 0x0049d5f8
tStateEntry g_aiPlayerLemmingStateEntries[] = {
	{GameOver, StartSommersault, ACTION_PREPARING_SOMMERSAULT, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_WAITING_TO_DIE, SFX_NONE},
	{IsStuck, StartRoute, ACTION_FINDING_ROUTE, SFX_NONE},
	{IsFalling, NULL, ACTION_FALLING, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{QOnBalloon, StartBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{NotFacingDestination, TurnToFaceDestination, ACTION_TURNING, SFX_NONE},
	{GotDestination, StartWalking, ACTION_WALKING, SFX_NONE},
	{PlayerBored, PlayerRandomAction, ACTION_IDLE_ANIMATION, SFX_NONE},
	{NULL, StartStanding, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{NotTimeUp, NULL, ACTION_TURNING, SFX_NONE},
	{NULL, StartStanding, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{IsJumping, NULL, ACTION_JUMPING, SFX_NONE},
	{IsFalling, NULL, ACTION_FALLING, SFX_NONE},
	{AtDestination, PlayerStopWalking, ACTION_NONE, SFX_NONE},
	{NULL, Walk, ACTION_WALKING, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{NULL, StartStanding, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{NULL, NULL, ACTION_HIDDEN, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{PlayerRequestingFire, NULL, ACTION_NONE, SFX_NONE},
	{PlayerNotFacingTarget, PlayerTurnToFaceTarget, ACTION_TURNING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_IDLE_ANIMATION, SFX_NONE},
	{NULL, StartStanding, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_HIT, SFX_NONE},
	{NULL, Die, ACTION_DEAD, SFX_NONE},
	{NULL, NULL, ACTION_DEAD, SFX_NONE},
	{IsStuck, SearchRoute, ACTION_FINDING_ROUTE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsJumping, Jump, ACTION_JUMPING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{IsFalling, Fall, ACTION_FALLING, SFX_NONE},
	{NULL, StartLand, ACTION_LANDING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_WAITING_TO_SPAWN, SFX_NONE},
	{NULL, StartWalking, ACTION_FALLING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_PREPARING_SOMMERSAULT, SFX_NONE},
	{NULL, NULL, ACTION_SOMMERSAULT, SFX_NONE},
	{NULL, NULL, ACTION_SOMMERSAULT, SFX_NONE},
	{NotTimeUp, NULL, ACTION_EXTERNAL_CONTROL, SFX_NONE},
	{NULL, ExternalControlEnd, ACTION_KEEP_CURRENT, SFX_NONE},
	{QOnBalloon, OnBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_LANDING, SFX_NONE},
	{NULL, Land, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_DEAD, SFX_NONE},
	{NotTimeUp, NULL, ACTION_WAITING_TO_DIE, SFX_NONE},
	{NULL, Die, ACTION_DEAD, SFX_NONE},
	{QOnBalloon, StartBalloon, ACTION_ON_BALLOON, SFX_NONE},
	{NULL, NULL, ACTION_ON_CONVEYOR, SFX_NONE},
};

// GLOBAL: LEMBALL 0x0049d9b8
tStateEntry g_sheepStateEntries[] = {
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NotFacingDestination, TurnToFaceDestination, ACTION_TURNING, SFX_NONE},
	{GotDestination, StartWalking, ACTION_WALKING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NotTimeUp, NULL, ACTION_TURNING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{AtDestination, StopWalking, ACTION_NONE, SFX_NONE},
	{NULL, Walk, ACTION_WALKING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NULL, Land, ACTION_NONE, SFX_NONE},
};

// GLOBAL: LEMBALL 0x0049da88
tStateEntry g_enemyStateEntries[] = {
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{EnemyNotFacingTarget, EnemyTurnToFaceTarget, ACTION_TURNING, SFX_NONE},
	{EnemyRequestingFire, EnemyStartFiring, ACTION_FIRING, SFX_NONE},
	{NotFacingDestination, TurnToFaceDestination, ACTION_TURNING, SFX_NONE},
	{GotDestination, StartWalking, ACTION_WALKING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{NotTimeUp, NULL, ACTION_TURNING, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{EnemyRequestingFire, NULL, ACTION_NONE, SFX_NONE},
	{AtDestination, StopWalking, ACTION_NONE, SFX_NONE},
	{NULL, Walk, ACTION_WALKING, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{RequestDeath, Die, ACTION_DEAD, SFX_NONE},
	{EnemyWaitingToFire, NULL, ACTION_FIRING, SFX_NONE},
	{EnemyRequestingFire, EnemyFire, ACTION_FIRING, SFX_GUN},
	{NotTimeUp, NULL, ACTION_FIRING, SFX_NONE},
	{NULL, EnemyEndFiring, ACTION_NONE, SFX_NONE},
	{Flying, Fly, ACTION_FLYING, SFX_NONE},
	{NULL, Land, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{IsHit, Hit, ACTION_HIT, SFX_GUNHIT},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NotTimeUp, NULL, ACTION_HIT, SFX_NONE},
	{NULL, Die, ACTION_DEAD, SFX_NONE},
	{NULL, NULL, ACTION_DEAD, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
	{NULL, NULL, ACTION_NONE, SFX_NONE},
};

// GLOBAL: LEMBALL 0x0049dcc8
tStateEntry* g_pUserLemmingStateTables[24] = {
	g_userLemmingStateEntries + 0,  g_userLemmingStateEntries + 14, g_userLemmingStateEntries + 19,
	g_userLemmingStateEntries + 27, g_userLemmingStateEntries + 32, g_userLemmingStateEntries + 34,
	g_userLemmingStateEntries + 35, g_userLemmingStateEntries + 41, g_userLemmingStateEntries + 43,
	g_userLemmingStateEntries + 44, g_userLemmingStateEntries + 46, g_userLemmingStateEntries + 48,
	g_userLemmingStateEntries + 50, g_userLemmingStateEntries + 52, g_userLemmingStateEntries + 54,
	g_userLemmingStateEntries + 55, g_userLemmingStateEntries + 57, g_userLemmingStateEntries + 59,
	g_userLemmingStateEntries + 60, g_userLemmingStateEntries + 61, g_userLemmingStateEntries + 64,
	g_userLemmingStateEntries + 65, g_userLemmingStateEntries + 67, NULL,
};

// GLOBAL: LEMBALL 0x0049dd28
tStateEntry* g_pAiPlayerLemmingStateTables[24] = {
	g_aiPlayerLemmingStateEntries + 0,  g_aiPlayerLemmingStateEntries + 11, g_aiPlayerLemmingStateEntries + 16,
	g_aiPlayerLemmingStateEntries + 23, g_aiPlayerLemmingStateEntries + 25, g_aiPlayerLemmingStateEntries + 27,
	g_aiPlayerLemmingStateEntries + 28, g_aiPlayerLemmingStateEntries + 33, g_aiPlayerLemmingStateEntries + 35,
	g_aiPlayerLemmingStateEntries + 36, g_aiPlayerLemmingStateEntries + 38, g_aiPlayerLemmingStateEntries + 40,
	g_aiPlayerLemmingStateEntries + 42, g_aiPlayerLemmingStateEntries + 44, g_aiPlayerLemmingStateEntries + 46,
	g_aiPlayerLemmingStateEntries + 47, g_aiPlayerLemmingStateEntries + 49, g_aiPlayerLemmingStateEntries + 51,
	g_aiPlayerLemmingStateEntries + 52, g_aiPlayerLemmingStateEntries + 53, g_aiPlayerLemmingStateEntries + 55,
	g_aiPlayerLemmingStateEntries + 56, g_aiPlayerLemmingStateEntries + 58, NULL,
};

// GLOBAL: LEMBALL 0x0049dd88
tStateEntry* g_pSheepStateTables[24] = {
	g_sheepStateEntries + 0,  g_sheepStateEntries + 4, g_sheepStateEntries + 7, g_sheepStateEntries + 10,
	g_sheepStateEntries + 11, g_enemyStateEntries + 0, g_enemyStateEntries + 0, g_enemyStateEntries + 0,
	g_enemyStateEntries + 0,  g_enemyStateEntries + 0, g_enemyStateEntries + 0, g_enemyStateEntries + 0,
	g_enemyStateEntries + 0,  g_enemyStateEntries + 0, g_enemyStateEntries + 0, g_enemyStateEntries + 0,
	g_enemyStateEntries + 0,  g_enemyStateEntries + 0, g_enemyStateEntries + 0, g_enemyStateEntries + 0,
	g_enemyStateEntries + 0,  g_enemyStateEntries + 0, g_enemyStateEntries + 0, NULL,
};

// GLOBAL: LEMBALL 0x0049dde8
tStateEntry* g_pEnemyStateTables[24] = {
	g_enemyStateEntries + 0,  g_enemyStateEntries + 8,  g_enemyStateEntries + 13, g_enemyStateEntries + 19,
	g_enemyStateEntries + 26, g_enemyStateEntries + 28, g_enemyStateEntries + 29, g_enemyStateEntries + 31,
	g_enemyStateEntries + 33, g_enemyStateEntries + 34, g_enemyStateEntries + 35, g_enemyStateEntries + 35,
	g_enemyStateEntries + 35, g_enemyStateEntries + 35, g_enemyStateEntries + 35, g_enemyStateEntries + 35,
	g_enemyStateEntries + 35, g_enemyStateEntries + 35, g_enemyStateEntries + 35, g_enemyStateEntries + 35,
	g_enemyStateEntries + 35, g_enemyStateEntries + 35, g_enemyStateEntries + 35, NULL,
};

// FUNCTION: LEMBALL 0x00419980
void StateMachine(tStateEntry** p_stateTables, CAI* p_ai, CGameObject* p_object)
{
	unsigned int info;
	eAction action;
	eAction nextAction;
	eSoundEffect soundEffect;
	tStateEntry* entry;

	action = p_object->m_action;
	entry = p_stateTables[action];
	while (entry->m_predicate != NULL) {
		if (entry->m_predicate(p_ai, p_object, (tInfo*) &info) != 0) {
			break;
		}
		entry++;
	}
	if (entry->m_actionFunction != NULL) {
		entry->m_actionFunction(p_ai, p_object, (tInfo*) &info);
	}
	nextAction = entry->m_nextAction;
	soundEffect = entry->m_soundEffect;
	if (p_object->GetSndEffect() == 0) {
		p_object->SetSndEffect(soundEffect);
	}
	p_object->UpdateCollision();
	if (nextAction != ACTION_KEEP_CURRENT && action != nextAction && p_object->m_action == action) {
		p_object->m_stateTimer = g_dwGameTick * GAME_TICK_MILLISECONDS;
		p_object->Action(nextAction);
	}
}

// FUNCTION: LEMBALL 0x00419a30
void UserLemming(CAI* p_ai, CGameObject* p_object)
{
	StateMachine(g_pUserLemmingStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a50
void AIPlayerLemming(CAI* p_ai, CGameObject* p_object)
{
	StateMachine(g_pAiPlayerLemmingStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a70
void SheepState(CAI* p_ai, CGameObject* p_object)
{
	StateMachine(g_pSheepStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a90
void EnemyState(CAI* p_ai, CGameObject* p_object)
{
	StateMachine(g_pEnemyStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419ab0
bool PlayerNotFacingCursor(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->FacingCursor() == 0;
}

// FUNCTION: LEMBALL 0x00419ad0
bool PlayerNotFacingTarget(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->IsRequestingFire() && p_object->FacingTarget() == 0) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00419b00
bool PlayerRequestingFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b10
bool PlayerWaitingToFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->IsRequestingFire() && p_object->m_actionDeadline > g_dwGameTick) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00419b40
bool PlayerBored(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->Bored();
}

// FUNCTION: LEMBALL 0x00419b50
bool EnemyNotFacingTarget(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->IsRequestingFire() && p_object->FacingTarget() == 0) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00419b80
bool EnemyRequestingFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b90
bool EnemyWaitingToFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->IsRequestingFire() && p_object->m_actionDeadline > g_dwGameTick) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00419bc0
bool GameOver(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (g_nGameOver != 0 && p_ai->m_gameStatus == GAME_STATUS_COMPLETING) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00419be0
bool IsStuck(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->m_routeSearchFailed;
}

// FUNCTION: LEMBALL 0x00419bf0
bool RequestDeath(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->m_deathRequested;
}

// FUNCTION: LEMBALL 0x00419c00
bool NotFacingDestination(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (GotDestination(p_ai, p_object, p_info) == 0) {
		return false;
	}
	return p_object->FacingDestination() == 0;
}

// FUNCTION: LEMBALL 0x00419c30
bool GotDestination(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->DestinationExists();
}

// FUNCTION: LEMBALL 0x00419c40
bool AtDestination(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return g_dwGameTick >= p_object->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c60
bool NotTimeUp(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return g_dwGameTick < p_object->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c80
bool Flying(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->IsFlying();
}

// FUNCTION: LEMBALL 0x00419c90
bool IsHit(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->IsHit();
}

// FUNCTION: LEMBALL 0x00419ca0
bool IsJumping(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->m_isJumping;
}

// FUNCTION: LEMBALL 0x00419cb0
bool IsFalling(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->m_isFalling;
}

// FUNCTION: LEMBALL 0x00419cc0
bool QOnBalloon(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	return p_object->QOnBalloon();
}

// FUNCTION: LEMBALL 0x00419cd0
void PlayerTurnToFaceCursor(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->TurnToFaceCursor();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d00
void PlayerTurnToFaceTarget(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->TurnToFaceTarget();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d30
void PlayerFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Fire();
}

// FUNCTION: LEMBALL 0x00419d40
void PlayerStartFiring(CAI*, CGameObject* p_object, tInfo*)
{
	p_object->StartFiring();
}

// FUNCTION: LEMBALL 0x00419d50
void PlayerEndFiring(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->EndFiring();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d80
void StartStanding(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartStanding();
}

// FUNCTION: LEMBALL 0x00419d90
void PlayerRandomAction(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->RandomAction();
	p_object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419dd0
void PlayerStopWalking(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StopMoving();
	p_object->SetBored(GAME_OBJECT_BOREDOM_MINIMUM_DELAY_MS);
	p_object->StartStanding();
}

// FUNCTION: LEMBALL 0x00419e00
void StartLand(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartLand();
}

// FUNCTION: LEMBALL 0x00419e10
void StartSommersault(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartSommersault();
}

// FUNCTION: LEMBALL 0x00419e20
void Land(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Land();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419e50
void StartRoute(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartRoute();
}

// FUNCTION: LEMBALL 0x00419e60
void SearchRoute(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->SearchRoute();
}

// FUNCTION: LEMBALL 0x00419e70
void Die(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Die();
}

// FUNCTION: LEMBALL 0x00419e80
void Fly(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Fly();
}

// FUNCTION: LEMBALL 0x00419e90
void StartWalking(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartMoving();
}

// FUNCTION: LEMBALL 0x00419ea0
void StopWalking(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StopMoving();
}

// FUNCTION: LEMBALL 0x00419eb0
void Walk(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Move();
}

// FUNCTION: LEMBALL 0x00419ec0
void TurnToFaceDestination(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->TurnToFaceDestination();
}

// FUNCTION: LEMBALL 0x00419ed0
void Hit(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->OnConveyor() != 0) {
		p_object->OnConveyor(0, NULL, 1);
	}
	p_object->GetHit();
}

// FUNCTION: LEMBALL 0x00419f00
void Jump(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Jump();
}

// FUNCTION: LEMBALL 0x00419f10
void Fall(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Fall();
}

// FUNCTION: LEMBALL 0x00419f20
void ExternalControlEnd(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->ExternalControlEnd();
}

// FUNCTION: LEMBALL 0x00419f30
void StartBalloon(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	if (p_object->OnConveyor() != 0) {
		p_object->OnConveyor(0, NULL, 1);
	}
	p_object->StartBalloon();
}

// FUNCTION: LEMBALL 0x00419f60
void OnBalloon(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->OnBalloon();
}

// FUNCTION: LEMBALL 0x00419f70
void EnemyTurnToFaceTarget(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->TurnToFaceTarget();
}

// FUNCTION: LEMBALL 0x00419f80
void EnemyStartFiring(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->StartFiring();
}

// FUNCTION: LEMBALL 0x00419f90
void EnemyFire(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->Fire();
}

// FUNCTION: LEMBALL 0x00419fa0
void EnemyEndFiring(CAI* p_ai, CGameObject* p_object, tInfo* p_info)
{
	p_object->EndFiring();
}
