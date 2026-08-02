#include "ENGINE/CORE/VSINIT.H"

extern void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);

struct GamePauseDialogHoverStateView {
	unsigned char m_abReserved00[0x64];
	int m_nHoveredOption64;
};

// FUNCTION: LEMBALL 0x00444bd0
void LEMBALL_FASTCALL ClearPauseDialogHoveredOption(GamePauseDialogHoverStateView* pDialog, void*, int, int)
{
	pDialog->m_nHoveredOption64 = 0;
	SetLevelScreenStatusIndicatorModeThunk(1, 0);
}

// FUNCTION: LEMBALL 0x00444bf0
void LEMBALL_FASTCALL ClearPauseDialogHoveredOptionVariant(GamePauseDialogHoverStateView* pDialog, void*, int, int)
{
	pDialog->m_nHoveredOption64 = 0;
	SetLevelScreenStatusIndicatorModeThunk(1, 0);
}
