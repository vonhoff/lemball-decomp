#ifndef LEMBALL_AI_OBJECTS_CENEMY_H
#define LEMBALL_AI_OBJECTS_CENEMY_H

#include "../Base/AiCoord.h"     // complete type
#include "../Base/CGameObject.h" // complete type

class CAi;
struct WaypointInformation;
enum eEnemyStateActions {
	ENEMY_ACTION_STOP = 0,
	ENEMY_ACTION_PATROL = 1,
	ENEMY_ACTION_TURN_AND_FIRE_RAPID = 2,
	ENEMY_ACTION_TURN_AND_FIRE_SLOW = 3,
	ENEMY_ACTION_TURN_AND_FIRE_RANDOM = 4
};

enum eEnemyStateRules {
	ENEMY_RULE_NONE = 0,
	ENEMY_RULE_RADIUS50 = 2,
	ENEMY_RULE_NOT_RADIUS50 = 3,
	ENEMY_RULE_RADIUS50_AND_LOS = 4,
	ENEMY_RULE_NOT_RADIUS50_AND_LOS = 5
};

union EnemyLemmingUnion {
	WaypointInformation* m_waypointInformation;
};

// SIZE 0x170
// VTABLE: LEMBALL 0x00495110
class CEnemy : public CGameObject {
public:
	friend class CEnemyGroupManager;

	CEnemy(CAi* p_arg0, int p_arg1, int p_arg2, int p_arg3, int p_arg4);
	bool CheckRadius(int p_radius);
	bool EnemyRuleRadius50();
	bool EnemyRuleRadius50AndLineOfSight();
	bool LineOfSight(AiCoord p_target);
	bool ProcessRule(eEnemyStateRules p_rule);
	virtual bool FacingTarget();               // vtable+0x98
	virtual void Fire();                       // vtable+0x88
	virtual int IsHit();                       // vtable+0x5c
	virtual bool IsRequestingFire();           // vtable+0x7c
	virtual bool Process();                    // vtable+0x14
	virtual void HitBullet(CBullet* p_bullet); // vtable+0x54
	virtual void EndFiring();                  // vtable+0x90
	virtual void GetHit();                     // vtable+0x60
	virtual void HitBall();                    // vtable+0x58
	virtual void HitMine();                    // vtable+0x64
	virtual void Restart();                    // vtable+0x104
	virtual void StartFiring();                // vtable+0x8c
	virtual void TurnToFaceTarget();           // vtable+0x9c
	virtual ~CEnemy();                         // vtable+0x00
	void EnemyActionPatrol(EnemyLemmingUnion* p_data);
	void EnemyActionTurnAndFireRandom(EnemyLemmingUnion* p_data);
	void EnemyActionTurnAndFireRapid(EnemyLemmingUnion* p_data);
	void EnemyActionTurnAndFireSlow(EnemyLemmingUnion* p_data);
	void ProcessAction(eEnemyStateRules p_rule, eEnemyStateActions p_action, EnemyLemmingUnion* p_data);
	void RequestFire(int p_interval);
	void GetEnemyType(eEnemyStateActions& p_action0,
					  eEnemyStateRules& p_rule0,
					  eEnemyStateActions& p_action1,
					  eEnemyStateRules& p_rule1,
					  eEnemyStateActions& p_action2,
					  eEnemyStateRules& p_rule2);
	void SetEnemyType(eEnemyStateActions p_action0,
					  eEnemyStateRules p_rule0,
					  eEnemyStateActions p_action1,
					  eEnemyStateRules p_rule1,
					  eEnemyStateActions p_action2,
					  eEnemyStateRules p_rule2);

private:
	unsigned int m_hit;                // 0x124
	int m_stateIndex;                  // 0x128
	eEnemyStateActions m_state0Action; // 0x12c
	eEnemyStateRules m_state0Rule;     // 0x130
	EnemyLemmingUnion m_state0Data;    // 0x134
	eEnemyStateActions m_state1Action; // 0x138
	eEnemyStateRules m_state1Rule;     // 0x13c
	EnemyLemmingUnion m_state1Data;    // 0x140
	eEnemyStateActions m_state2Action; // 0x144
	eEnemyStateRules m_state2Rule;     // 0x148
	EnemyLemmingUnion m_state2Data;    // 0x14c
	AiCoord m_targetPosition;          // 0x150
	AiCoord m_fireTarget;              // 0x15c
	int m_fireState;                   // 0x168
	int m_fireInterval;                // 0x16c
};

// SYNTHETIC: LEMBALL 0x00420ab0
// CEnemy::`scalar deleting destructor'

#endif
