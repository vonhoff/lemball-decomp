#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct ActiveManagedEntityOwnerView {
	unsigned char m_abReserved00[0x58];
	int m_bAllowDelayedMoves58;
};

extern void* g_pActiveManagedEntityOwner;
extern void* g_pSessionRandomState;

struct DelayedManagedEntityMoveView {
	unsigned char m_abReserved00[0x150];
	int m_nCurrentX150;
	int m_nCurrentY154;
	int m_nCurrentZ158;
	int m_nSavedX15C;
	int m_nSavedY160;
	int m_nSavedZ164;
	int m_bDelayedMoveActive168;
	int m_nDelayedMoveTicks16C;

	void ArmDelayedMove100(void* pUnused);
	void ArmDelayedMove800(void* pUnused);
	void ArmDelayedMoveRandom(void* pUnused);
	void ArmDelayedMoveToSavedTarget(int nTicks);
};

// MACINTOSH: CEnemy::EnemyAction_TURNANDFIRERAPID(tEnemyLemmingUnion*)
// FUNCTION: LEMBALL 0x004201a0
void DelayedManagedEntityMoveView::ArmDelayedMove100(void*)
{
	ArmDelayedMoveToSavedTarget(100);
}

// MACINTOSH: CEnemy::EnemyAction_TURNANDFIRESLOW(tEnemyLemmingUnion*)
// FUNCTION: LEMBALL 0x004201b0
void DelayedManagedEntityMoveView::ArmDelayedMove800(void*)
{
	ArmDelayedMoveToSavedTarget(800);
}

// MACINTOSH: CEnemy::EnemyAction_TURNANDFIRERANDOM(tEnemyLemmingUnion*)
// FUNCTION: LEMBALL 0x004201c0
void DelayedManagedEntityMoveView::ArmDelayedMoveRandom(void*)
{
	int nRandom = (*(int*) g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	ArmDelayedMoveToSavedTarget(nRandom % 1000 + 150);
}

// MACINTOSH: CEnemy::RequestFire(int)
// FUNCTION: LEMBALL 0x004203e0
void DelayedManagedEntityMoveView::ArmDelayedMoveToSavedTarget(int nTicks)
{
	if (((ActiveManagedEntityOwnerView*) g_pActiveManagedEntityOwner)->m_bAllowDelayedMoves58 != 0 &&
		m_bDelayedMoveActive168 == 0) {
		m_nSavedX15C = m_nCurrentX150;
		m_nSavedY160 = m_nCurrentY154;
		m_nSavedZ164 = m_nCurrentZ158;
		m_nDelayedMoveTicks16C = nTicks;
		m_bDelayedMoveActive168 = 1;
	}
}
