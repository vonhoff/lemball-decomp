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
void StateMachine(StateEntry** p_arg0, CAi* p_arg1, CGameObject* p_arg2)
{
	typedef bool (*StatePredicate)(CAi*, CGameObject*, Info*);
	typedef void (*StateAction)(CAi*, CGameObject*, Info*);

	unsigned int info;
	eAction action;
	eAction nextAction;
	int actionArgument;
	StateEntry* entry;

	action = p_arg2->m_action;
	entry = p_arg0[action];
	while (entry->m_predicate != 0) {
		if (((StatePredicate) entry->m_predicate)(p_arg1, p_arg2, (Info*) &info) != 0) {
			break;
		}
		entry++;
	}
	if (entry->m_actionFunction != 0) {
		((StateAction) entry->m_actionFunction)(p_arg1, p_arg2, (Info*) &info);
	}
	nextAction = entry->m_nextAction;
	actionArgument = entry->m_actionArgument;
	if (p_arg2->GetSndEffect() == 0) {
		p_arg2->SetSndEffect((eSoundEffect) actionArgument);
	}
	p_arg2->UpdateCollision();
	if (nextAction != 0x28 && action != nextAction && p_arg2->m_action == action) {
		p_arg2->m_stateTimer = g_dwGameTick * 0x32;
		p_arg2->Action(nextAction);
	}
}

// FUNCTION: LEMBALL 0x00419a30
void UserLemming(CAi* p_arg0, CGameObject* p_arg1)
{
	StateMachine(g_pUserLemmingStateTables, p_arg0, p_arg1);
}

// FUNCTION: LEMBALL 0x00419a50
void AiPlayerLemming(CAi* p_arg0, CGameObject* p_arg1)
{
	StateMachine(g_pAiPlayerLemmingStateTables, p_arg0, p_arg1);
}

// FUNCTION: LEMBALL 0x00419a70
void SheepState(CAi* p_arg0, CGameObject* p_arg1)
{
	StateMachine(g_pSheepStateTables, p_arg0, p_arg1);
}

// FUNCTION: LEMBALL 0x00419a90
void EnemyState(CAi* p_arg0, CGameObject* p_arg1)
{
	StateMachine(g_pEnemyStateTables, p_arg0, p_arg1);
}

// FUNCTION: LEMBALL 0x00419ab0
bool PlayerNotFacingCursor(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->FacingCursor() == 0;
}

// FUNCTION: LEMBALL 0x00419ad0
bool PlayerNotFacingTarget(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b00
bool PlayerRequestingFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b10
bool PlayerWaitingToFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b40
bool PlayerBored(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->Bored();
}

// FUNCTION: LEMBALL 0x00419b50
bool EnemyNotFacingTarget(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419b80
bool EnemyRequestingFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsRequestingFire();
}

// FUNCTION: LEMBALL 0x00419b90
bool EnemyWaitingToFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419bc0
bool GameOver(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (g_nGameOver != 0 && p_arg0->m_gameStatus == 4) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00419be0
bool IsStuck(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_routeSearchFailed;
}

// FUNCTION: LEMBALL 0x00419bf0
bool RequestDeath(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x2c;
}

// FUNCTION: LEMBALL 0x00419c00
bool NotFacingDestination(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (GotDestination(p_arg0, p_arg1, p_arg2) == 0) {
		return 0;
	}
	return p_arg1->FacingDestination() == 0;
}

// FUNCTION: LEMBALL 0x00419c30
bool GotDestination(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->DestinationExists();
}

// FUNCTION: LEMBALL 0x00419c40
bool AtDestination(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return g_dwGameTick >= p_arg1->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c60
bool NotTimeUp(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return g_dwGameTick < p_arg1->m_actionDeadline;
}

// FUNCTION: LEMBALL 0x00419c80
bool Flying(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsFlying();
}

// FUNCTION: LEMBALL 0x00419c90
bool IsHit(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsHit();
}

// FUNCTION: LEMBALL 0x00419ca0
bool IsJumping(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x104;
}

// FUNCTION: LEMBALL 0x00419cb0
bool IsFalling(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x108;
}

// FUNCTION: LEMBALL 0x00419cc0
bool QOnBalloon(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->QOnBalloon();
}

// FUNCTION: LEMBALL 0x00419cd0
void PlayerTurnToFaceCursor(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceCursor();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// FUNCTION: LEMBALL 0x00419d00
void PlayerTurnToFaceTarget(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceTarget();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// FUNCTION: LEMBALL 0x00419d30
void PlayerFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fire();
}

// FUNCTION: LEMBALL 0x00419d50
void PlayerEndFiring(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->EndFiring();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// FUNCTION: LEMBALL 0x00419d80
void StartStanding(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartStanding();
}

// FUNCTION: LEMBALL 0x00419d90
void PlayerRandomAction(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->RandomAction();
	p_arg1->SetBored(0xfa0);
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// FUNCTION: LEMBALL 0x00419dd0
void PlayerStopWalking(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StopMoving();
	p_arg1->SetBored(0xfa0);
	p_arg1->StartStanding();
}

// FUNCTION: LEMBALL 0x00419e00
void StartLand(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartLand();
}

// FUNCTION: LEMBALL 0x00419e10
void StartSommersault(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartSommersault();
}

// FUNCTION: LEMBALL 0x00419e20
void Land(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Land();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// FUNCTION: LEMBALL 0x00419e50
void StartRoute(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartRoute();
}

// FUNCTION: LEMBALL 0x00419e60
void SearchRoute(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->SearchRoute();
}

// FUNCTION: LEMBALL 0x00419e70
void Die(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Die();
}

// FUNCTION: LEMBALL 0x00419e80
void Fly(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fly();
}

// FUNCTION: LEMBALL 0x00419e90
void StartWalking(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartMoving();
}

// FUNCTION: LEMBALL 0x00419ea0
void StopWalking(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StopMoving();
}

// FUNCTION: LEMBALL 0x00419eb0
void Walk(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Move();
}

// FUNCTION: LEMBALL 0x00419ec0
void TurnToFaceDestination(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceDestination();
}

// FUNCTION: LEMBALL 0x00419ed0
void Hit(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->OnConveyor() != 0) {
		p_arg1->OnConveyor(0, 0, 1);
	}
	p_arg1->GetHit();
}

// FUNCTION: LEMBALL 0x00419f00
void Jump(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Jump();
}

// FUNCTION: LEMBALL 0x00419f10
void Fall(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fall();
}

// FUNCTION: LEMBALL 0x00419f20
void ExternalControlEnd(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->ExternalControlEnd();
}

// FUNCTION: LEMBALL 0x00419f30
void StartBalloon(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->OnConveyor() != 0) {
		p_arg1->OnConveyor(0, 0, 1);
	}
	p_arg1->StartBalloon();
}

// FUNCTION: LEMBALL 0x00419f60
void OnBalloon(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->OnBalloon();
}

// FUNCTION: LEMBALL 0x00419f70
void EnemyTurnToFaceTarget(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceTarget();
}

// FUNCTION: LEMBALL 0x00419f80
void EnemyStartFiring(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartFiring();
}

// FUNCTION: LEMBALL 0x00419f90
void EnemyFire(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fire();
}

// FUNCTION: LEMBALL 0x00419fa0
void EnemyEndFiring(CAi* p_arg0, CGameObject* p_arg1, Info* p_arg2)
{
	p_arg1->EndFiring();
}
