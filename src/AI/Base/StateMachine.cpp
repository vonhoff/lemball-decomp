#include "StateMachine.h"

#include "../../Control/Game/Game.h"
#include "../Navigation/Ai.h"
#include "GameObject.h"
#include "StateEntry.h"

// GLOBAL: LEMBALL 0x0049dcc8
StateEntry* g_pUserLemmingStateTables[24];

// GLOBAL: LEMBALL 0x0049dd28
StateEntry* g_pAiPlayerLemmingStateTables[24];

// GLOBAL: LEMBALL 0x0049dd88
StateEntry* g_pSheepStateTables[24];

// GLOBAL: LEMBALL 0x0049dde8
StateEntry* g_pEnemyStateTables[24];

// 68K 0x1061f46e StateMachine__FPP11tStateEntryP3CAIP11CGameObject
// FUNCTION: LEMBALL 0x00419980
void StateMachine(StateEntry** p_arg0, Ai* p_arg1, GameObject* p_arg2)
{
	typedef bool (*StatePredicate)(Ai*, GameObject*, Info*);
	typedef void (*StateAction)(Ai*, GameObject*, Info*);

	undefined4 info;
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

// 68K 0x1061f578 UserLemming__FP3CAIP11CGameObject
// FUNCTION: LEMBALL 0x00419a30
void UserLemming(Ai* p_arg0, GameObject* p_arg1)
{
	StateMachine(g_pUserLemmingStateTables, p_arg0, p_arg1);
}

// 68K 0x1061f5b8 AIPlayerLemming__FP3CAIP11CGameObject
// FUNCTION: LEMBALL 0x00419a50
void AiPlayerLemming(Ai* p_arg0, GameObject* p_arg1)
{
	StateMachine(g_pAiPlayerLemmingStateTables, p_arg0, p_arg1);
}

// 68K 0x1061f5fc SheepState__FP3CAIP11CGameObject
// FUNCTION: LEMBALL 0x00419a70
void SheepState(Ai* p_arg0, GameObject* p_arg1)
{
	StateMachine(g_pSheepStateTables, p_arg0, p_arg1);
}

// 68K 0x1061f63a EnemyState__FP3CAIP11CGameObject
// FUNCTION: LEMBALL 0x00419a90
void EnemyState(Ai* p_arg0, GameObject* p_arg1)
{
	StateMachine(g_pEnemyStateTables, p_arg0, p_arg1);
}

// 68K 0x1061f678 PlayerNotFacingCursor__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ab0
bool PlayerNotFacingCursor(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->FacingCursor() == 0;
}

// 68K 0x1061f6ce PlayerNotFacingTarget__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ad0
bool PlayerNotFacingTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// 68K 0x1061f73e PlayerRequestingFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b00
bool PlayerRequestingFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsRequestingFire();
}

// 68K 0x1061f78c PlayerWaitingToFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b10
bool PlayerWaitingToFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// 68K 0x1061f7f4 PlayerBored__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b40
bool PlayerBored(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->Bored();
}

// 68K 0x1061f838 EnemyNotFacingTarget__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b50
bool EnemyNotFacingTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->FacingTarget() == 0) {
		return 1;
	}
	return 0;
}

// 68K 0x1061f8a8 EnemyRequestingFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b80
bool EnemyRequestingFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsRequestingFire();
}

// 68K 0x1061f8f4 EnemyWaitingToFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419b90
bool EnemyWaitingToFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->IsRequestingFire() && p_arg1->m_actionDeadline > g_dwGameTick) {
		return 1;
	}
	return 0;
}

// 68K 0x1061f95c GameOver__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419bc0
bool GameOver(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (g_nGameOver != 0 && p_arg0->m_gameStatus == 4) {
		return 1;
	}
	return 0;
}

// 68K 0x1061f9aa IsStuck__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419be0
bool IsStuck(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_routeSearchFailed;
}

// 68K 0x1061f9f4 RequestDeath__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419bf0
bool RequestDeath(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x2c;
}

// 68K 0x1061fa44 NotFacingDestination__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c00
bool NotFacingDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (GotDestination(p_arg0, p_arg1, p_arg2) == 0) {
		return 0;
	}
	return p_arg1->FacingDestination() == 0;
}

// 68K 0x1061fab2 GotDestination__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c30
bool GotDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->DestinationExists();
}

// 68K 0x1061faf4 AtDestination__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c40
bool AtDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return g_dwGameTick >= p_arg1->m_actionDeadline;
}

// 68K 0x1061fb3e NotTimeUp__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c60
bool NotTimeUp(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return g_dwGameTick < p_arg1->m_actionDeadline;
}

// 68K 0x1061fb84 Flying__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c80
bool Flying(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsFlying();
}

// 68K 0x1061fbc4 IsHit__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419c90
bool IsHit(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->IsHit();
}

// 68K 0x1061fc02 IsJumping__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ca0
bool IsJumping(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x104;
}

// 68K 0x1061fc4a IsFalling__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419cb0
bool IsFalling(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->m_unk0x108;
}

// 68K 0x1061fc92 QOnBalloon__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419cc0
bool QOnBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	return p_arg1->QOnBalloon();
}

// 68K 0x1061fcd6 PlayerTurnToFaceCursor__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419cd0
void PlayerTurnToFaceCursor(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceCursor();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// 68K 0x1061fd38 PlayerTurnToFaceTarget__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419d00
void PlayerTurnToFaceTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceTarget();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// 68K 0x1061fd9a PlayerFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419d30
void PlayerFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fire();
}

// 68K 0x1061fdde PlayerEndFiring__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419d50
void PlayerEndFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->EndFiring();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// 68K 0x1061fe38 StartStanding__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419d80
void StartStanding(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartStanding();
}

// 68K 0x1061fe7e PlayerRandomAction__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419d90
void PlayerRandomAction(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->RandomAction();
	p_arg1->SetBored(0xfa0);
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// 68K 0x1061fef4 PlayerStopWalking__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419dd0
void PlayerStopWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StopMoving();
	p_arg1->SetBored(0xfa0);
	p_arg1->StartStanding();
}

// 68K 0x1061ff62 StartLand__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e00
void StartLand(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartLand();
}

// 68K 0x1061ffa4 StartSommersault__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e10
void StartSommersault(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartSommersault();
}

// 68K 0x1061ffe8 Land__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e20
void Land(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Land();
	StartStanding(p_arg0, p_arg1, p_arg2);
}

// 68K 0x10620036 StartRoute__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e50
void StartRoute(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartRoute();
}

// 68K 0x1062007a SearchRoute__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e60
void SearchRoute(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->SearchRoute();
}

// 68K 0x106200be Die__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e70
void Die(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Die();
}

// 68K 0x106200fa Fly__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e80
void Fly(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fly();
}

// 68K 0x10620136 StartWalking__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419e90
void StartWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartMoving();
}

// 68K 0x10620176 StopWalking__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ea0
void StopWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StopMoving();
}

// 68K 0x106201b4 Walk__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419eb0
void Walk(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Move();
}

// 68K 0x106201f2 TurnToFaceDestination__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ec0
void TurnToFaceDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceDestination();
}

// 68K 0x1062023a Hit__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419ed0
void Hit(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->OnConveyor() != 0) {
		p_arg1->OnConveyor(0, 0, 1);
	}
	p_arg1->GetHit();
}

// 68K 0x106202ac Jump__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f00
void Jump(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Jump();
}

// 68K 0x106202e4 Fall__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f10
void Fall(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fall();
}

// 68K 0x1062031c ExternalControlEnd__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f20
void ExternalControlEnd(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->ExternalControlEnd();
}

// 68K 0x10620368 StartBalloon__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f30
void StartBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	if (p_arg1->OnConveyor() != 0) {
		p_arg1->OnConveyor(0, 0, 1);
	}
	p_arg1->StartBalloon();
}

// 68K 0x106203e4 OnBalloon__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f60
void OnBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->OnBalloon();
}

// 68K 0x10620426 EnemyTurnToFaceTarget__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f70
void EnemyTurnToFaceTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->TurnToFaceTarget();
}

// 68K 0x10620474 EnemyStartFiring__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f80
void EnemyStartFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->StartFiring();
}

// 68K 0x106204be EnemyFire__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419f90
void EnemyFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->Fire();
}

// 68K 0x10620500 EnemyEndFiring__FP3CAIP11CGameObjectP5tInfo
// FUNCTION: LEMBALL 0x00419fa0
void EnemyEndFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2)
{
	p_arg1->EndFiring();
}
