struct GameLevelProgressState {
	int m_nCurrentLevel;
	int m_nUnused04;
	int m_nCurrentPack;
	int m_nUnused0C;
	char m_abReserved10[0x2c];
	int m_anSnapshotPackCaps[5];

	void SetSelectedLevelNumber(int nLevel);
};

// Macintosh: CGameStatus::Level(int)
// FUNCTION: LEMBALL 0x00409070
void GameLevelProgressState::SetSelectedLevelNumber(int nLevel)
{
	m_nCurrentLevel = nLevel;
	m_anSnapshotPackCaps[m_nCurrentPack] = nLevel;
}
