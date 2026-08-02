#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void* g_pLevelProgressState;
extern void RequestLocalLevelGameStateChange(void* pLevelMode, int nState);

struct LevelQuitStateView {
	unsigned char m_abReserved00[0xd8];
	int m_nQuitDelay;
};

struct LevelProgressStateView {
	unsigned char m_abReserved00[0x0c];
	int m_nState;
};

// MACINTOSH: CAI::QuitGame()
// FUNCTION: LEMBALL 0x00412c60
void LEMBALL_FASTCALL RequestLevelQuitGameState(LevelQuitStateView* pLevelMode)
{
	typedef void(LEMBALL_STDCALL * RequestStateProc)(int nState);
	pLevelMode->m_nQuitDelay = 0;
	((LevelProgressStateView*) g_pLevelProgressState)->m_nState = 5;
	((RequestStateProc) RequestLocalLevelGameStateChange)(5);
}
