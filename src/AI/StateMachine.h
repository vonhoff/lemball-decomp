#ifndef LEMBALL_SCAFFOLD_AI_STATEMACHINE_H
#define LEMBALL_SCAFFOLD_AI_STATEMACHINE_H

#include "../Common.h"

void StateMachine(StateEntry** p_arg0, Ai* p_arg1, GameObject* p_arg2);
void UserLemming(Ai* p_arg0, GameObject* p_arg1);
void AiPlayerLemming(Ai* p_arg0, GameObject* p_arg1);
void SheepState(Ai* p_arg0, GameObject* p_arg1);
void EnemyState(Ai* p_arg0, GameObject* p_arg1);
bool PlayerNotFacingCursor(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool PlayerNotFacingTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool PlayerRequestingFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool PlayerWaitingToFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool PlayerBored(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool EnemyNotFacingTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool EnemyRequestingFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool EnemyWaitingToFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool GameOver(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool IsStuck(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool RequestDeath(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool NotFacingDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool GotDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool AtDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool NotTimeUp(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool Flying(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool IsHit(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool IsJumping(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool IsFalling(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
bool QOnBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerTurnToFaceCursor(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerTurnToFaceTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerEndFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartStanding(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerRandomAction(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void PlayerStopWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartLand(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartSommersault(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Land(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartRoute(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void SearchRoute(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Die(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Fly(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StopWalking(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Walk(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void TurnToFaceDestination(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Hit(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Jump(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void Fall(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void ExternalControlEnd(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void StartBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void OnBalloon(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void EnemyTurnToFaceTarget(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void EnemyStartFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void EnemyFire(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
void EnemyEndFiring(Ai* p_arg0, GameObject* p_arg1, Info* p_arg2);
#endif
