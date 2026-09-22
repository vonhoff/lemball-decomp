#include "StateMachine.h"

#include "../../Control/Game/CGame.h"
#include "../Navigation/CAi.h"
#include "AI/Base/ObjectActions.h"
#include "CGameObject.h"
#include "StateEntry.h"
#include "Views/Sound/SoundEffects.h"

// GLOBAL: LEMBALL 0x0049dcc8
StateEntry* g_pUserLemmingStateTables[24];

// GLOBAL: LEMBALL 0x0049dd28
StateEntry* g_pAiPlayerLemmingStateTables[24];

// GLOBAL: LEMBALL 0x0049dd88
StateEntry* g_pSheepStateTables[24];

// GLOBAL: LEMBALL 0x0049dde8
StateEntry* g_pEnemyStateTables[24];

// FUNCTION: LEMBALL 0x00419980
void StateMachine(StateEntry** p_stateTables, CAi* p_ai, CGameObject* p_object)
{
	typedef bool (*StatePredicate)(CAi*, CGameObject*, Info*);
	typedef void (*StateAction)(CAi*, CGameObject*, Info*);

	unsigned int info;
	eAction action;
	eAction nextAction;
	int actionArgument;
	StateEntry* entry;

	action = p_object->m_action;
	entry = p_stateTables[action];
	while (entry->m_predicate != 0) {
		if (((StatePredicate) entry->m_predicate)(p_ai, p_object, (Info*) &info) != 0) {
			break;
		}
		entry++;
	}
	if (entry->m_actionFunction != 0) {
		((StateAction) entry->m_actionFunction)(p_ai, p_object, (Info*) &info);
	}
	nextAction = entry->m_nextAction;
	actionArgument = entry->m_actionArgument;
	if (p_object->GetSndEffect() == 0) {
		p_object->SetSndEffect((eSoundEffect) actionArgument);
	}
	p_object->UpdateCollision();
	if (nextAction != 0x28 && action != nextAction && p_object->m_action == action) {
		p_object->m_stateTimer = g_dwGameTick * 0x32;
		p_object->Action(nextAction);
	}
}

// FUNCTION: LEMBALL 0x00419a30
void UserLemming(CAi* p_ai, CGameObject* p_object)
{
	StateMachine(g_pUserLemmingStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a50
void AiPlayerLemming(CAi* p_ai, CGameObject* p_object)
{
	StateMachine(g_pAiPlayerLemmingStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a70
void SheepState(CAi* p_ai, CGameObject* p_object)
{
	StateMachine(g_pSheepStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419a90
void EnemyState(CAi* p_ai, CGameObject* p_object)
{
	StateMachine(g_pEnemyStateTables, p_ai, p_object);
}

// FUNCTION: LEMBALL 0x00419ab0
bool PlayerNotFacingCursor(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->FacingCursor() == 0;
}

// FUNCTION: LEMBALL 0x00419ad0
bool PlayerNotFacingTarget(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->IsRequestingFire() && p_object->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b00
bool PlayerRequestingFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b10
bool PlayerWaitingToFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->IsRequestingFire() && p_object->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b40
bool PlayerBored(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->Bored();
}

// FUNCTION: LEMBALL 0x00419b50
bool EnemyNotFacingTarget(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->IsRequestingFire() && p_object->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b80
bool EnemyRequestingFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b90
bool EnemyWaitingToFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->IsRequestingFire() && p_object->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419bc0
bool GameOver(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (g_nGameOver != 0 && p_ai->m_gameStatus == 4) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419be0
bool IsStuck(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->m_routeSearchFailed;
}

// FUNCTION: LEMBALL 0x00419bf0
bool RequestDeath(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->m_deathRequested;
}

// FUNCTION: LEMBALL 0x00419c00
bool NotFacingDestination(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (GotDestination(p_ai, p_object, p_info) == 0) {
		return 0;
	}
	return p_object->FacingDestination() == 0;
}

// FUNCTION: LEMBALL 0x00419c30
bool GotDestination(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->DestinationExists();
}

// FUNCTION: LEMBALL 0x00419c40
bool AtDestination(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return g_dwGameTick >= p_object->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c60
bool NotTimeUp(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return g_dwGameTick < p_object->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c80
bool Flying(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->IsFlying();
}

// FUNCTION: LEMBALL 0x00419c90
bool IsHit(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->IsHit();
}

// FUNCTION: LEMBALL 0x00419ca0
bool IsJumping(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->m_isJumping;
}

// FUNCTION: LEMBALL 0x00419cb0
bool IsFalling(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->m_isFalling;
}

// FUNCTION: LEMBALL 0x00419cc0
bool QOnBalloon(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	return p_object->QOnBalloon();
}

// FUNCTION: LEMBALL 0x00419cd0
void PlayerTurnToFaceCursor(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->TurnToFaceCursor();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d00
void PlayerTurnToFaceTarget(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->TurnToFaceTarget();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d30
void PlayerFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Fire();
}

// FUNCTION: LEMBALL 0x00419d50
void PlayerEndFiring(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->EndFiring();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419d80
void StartStanding(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartStanding();
}

// FUNCTION: LEMBALL 0x00419d90
void PlayerRandomAction(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->RandomAction();
	p_object->SetBored(0xfa0);
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419dd0
void PlayerStopWalking(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StopMoving();
	p_object->SetBored(0xfa0);
	p_object->StartStanding();
}

// FUNCTION: LEMBALL 0x00419e00
void StartLand(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartLand();
}

// FUNCTION: LEMBALL 0x00419e10
void StartSommersault(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartSommersault();
}

// FUNCTION: LEMBALL 0x00419e20
void Land(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Land();
	StartStanding(p_ai, p_object, p_info);
}

// FUNCTION: LEMBALL 0x00419e50
void StartRoute(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartRoute();
}

// FUNCTION: LEMBALL 0x00419e60
void SearchRoute(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->SearchRoute();
}

// FUNCTION: LEMBALL 0x00419e70
void Die(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Die();
}

// FUNCTION: LEMBALL 0x00419e80
void Fly(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Fly();
}

// FUNCTION: LEMBALL 0x00419e90
void StartWalking(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartMoving();
}

// FUNCTION: LEMBALL 0x00419ea0
void StopWalking(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StopMoving();
}

// FUNCTION: LEMBALL 0x00419eb0
void Walk(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Move();
}

// FUNCTION: LEMBALL 0x00419ec0
void TurnToFaceDestination(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->TurnToFaceDestination();
}

// FUNCTION: LEMBALL 0x00419ed0
void Hit(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->OnConveyor() != 0) {
		p_object->OnConveyor(0, 0, 1);
	}
	p_object->GetHit();
}

// FUNCTION: LEMBALL 0x00419f00
void Jump(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Jump();
}

// FUNCTION: LEMBALL 0x00419f10
void Fall(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Fall();
}

// FUNCTION: LEMBALL 0x00419f20
void ExternalControlEnd(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->ExternalControlEnd();
}

// FUNCTION: LEMBALL 0x00419f30
void StartBalloon(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	if (p_object->OnConveyor() != 0) {
		p_object->OnConveyor(0, 0, 1);
	}
	p_object->StartBalloon();
}

// FUNCTION: LEMBALL 0x00419f60
void OnBalloon(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->OnBalloon();
}

// FUNCTION: LEMBALL 0x00419f70
void EnemyTurnToFaceTarget(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->TurnToFaceTarget();
}

// FUNCTION: LEMBALL 0x00419f80
void EnemyStartFiring(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->StartFiring();
}

// FUNCTION: LEMBALL 0x00419f90
void EnemyFire(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->Fire();
}

// FUNCTION: LEMBALL 0x00419fa0
void EnemyEndFiring(CAi* p_ai, CGameObject* p_object, Info* p_info)
{
	p_object->EndFiring();
}
