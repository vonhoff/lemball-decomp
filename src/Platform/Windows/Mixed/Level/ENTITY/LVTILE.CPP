class LevelTileCellDefaultView {
public:
	int m_nType;
	short m_anState[4];

	LevelTileCellDefaultView* InitializeDefault(void);
};

// FUNCTION: LEMBALL 0x004314f0
LevelTileCellDefaultView* LevelTileCellDefaultView::InitializeDefault(void)
{
	m_nType = 0x209;
	m_anState[0] = 0;
	m_anState[1] = 0;
	m_anState[2] = 0;
	m_anState[3] = 0;
	return this;
}
