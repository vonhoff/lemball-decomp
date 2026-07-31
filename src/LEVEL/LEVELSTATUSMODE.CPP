extern void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);

struct LevelStatusSpriteOwnerView {
	char m_abReserved00[0x7c];
	int m_nStatusMode7C;

	void SetStatusMode(int nUnused);
};

// FUNCTION: LEMBALL 0x00442670
void LevelStatusSpriteOwnerView::SetStatusMode(int nUnused)
{
	(void) nUnused;
	SetLevelScreenStatusIndicatorModeThunk(1, m_nStatusMode7C);
}
