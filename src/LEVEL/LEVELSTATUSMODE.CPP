extern void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);

struct LevelStatusIndicatorOwnerView {
	char m_abReserved00[0xa8];
	int m_nStatusModeA8;

	void ShowStatusMode(int nUnused);
};

// FUNCTION: LEMBALL 0x00442240
void LevelStatusIndicatorOwnerView::ShowStatusMode(int nUnused)
{
	(void) nUnused;
	SetLevelScreenStatusIndicatorModeThunk(1, m_nStatusModeA8);
}

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

struct LevelActionButtonStateView {
	char m_abReserved00[0x10c];
	int m_nState10C;

	void ClearAndResetStatus(int nUnused);
};

// FUNCTION: LEMBALL 0x00442aa0
void LevelActionButtonStateView::ClearAndResetStatus(int nUnused)
{
	(void) nUnused;
	m_nState10C = 0;
	SetLevelScreenStatusIndicatorModeThunk(1, 0);
}

struct LevelActionButtonStatusView {
	char m_abReserved00[0x7c];
	int m_fStatusActive7C;

	void ClearStatus(int nUnused1, int nUnused2);
};

// FUNCTION: LEMBALL 0x00442ac0
void LevelActionButtonStatusView::ClearStatus(int nUnused1, int nUnused2)
{
	(void) nUnused1;
	(void) nUnused2;
	if (m_fStatusActive7C != 0) {
		m_fStatusActive7C = 0;
		SetLevelScreenStatusIndicatorModeThunk(1, 0);
	}
}
