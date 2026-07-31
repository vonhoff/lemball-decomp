extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;

class LevelManagedEntityRandomDelayView {
public:
	void ArmRandomActionDelay(void);

private:
	unsigned char m_abReserved00[0xbc];
	short m_nActionPhaseBC;
	unsigned char m_abReservedBE[0x0e];
	int m_nActionDeadlineCC;
};

// GLOBAL: LEMBALL 0x0049d134
int g_nManagedEntityRandomActionPhase = 0;

// FUNCTION: LEMBALL 0x00416510
void LevelManagedEntityRandomDelayView::ArmRandomActionDelay(void)
{
	int nRandom = (*(int*) g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	m_nActionDeadlineCC = g_nLevelFrameClockTick + (nRandom % 500 + 50) / 50;
	m_nActionPhaseBC = (short) g_nManagedEntityRandomActionPhase;
	g_nManagedEntityRandomActionPhase ^= 1;
}

// FUNCTION: LEMBALL 0x00419e10
void __cdecl ThunkArmRandomManagedEntityActionDelay(void* pUnused, LevelManagedEntityRandomDelayView* pEntity)
{
	(void) pUnused;
	pEntity->ArmRandomActionDelay();
}
