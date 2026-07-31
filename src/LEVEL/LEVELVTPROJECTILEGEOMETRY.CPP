struct LevelProjectileGeometryIteratorPair {
	int m_nX;
	int m_nY;
};

struct LevelProjectileGeometryPairTableIteratorView {
	LevelProjectileGeometryIteratorPair m_aPrimaryPairs00[24];
	LevelProjectileGeometryIteratorPair m_aSecondaryPairsC0[8];
	int m_nPairCursor100;

	LevelProjectileGeometryIteratorPair* ResetSecondaryPairCursor(void);
	LevelProjectileGeometryIteratorPair* GetNextSecondaryPair(void);
	LevelProjectileGeometryIteratorPair* GetRotatedPair(int nIndex);
};

// FUNCTION: LEMBALL 0x0041a2e0
LevelProjectileGeometryIteratorPair* LevelProjectileGeometryPairTableIteratorView::ResetSecondaryPairCursor(void)
{
	m_nPairCursor100 = 0;
	return m_aSecondaryPairsC0;
}

// FUNCTION: LEMBALL 0x0041a300
LevelProjectileGeometryIteratorPair* LevelProjectileGeometryPairTableIteratorView::GetNextSecondaryPair(void)
{
	if (++m_nPairCursor100 >= 8) {
		return 0;
	}
	return &m_aSecondaryPairsC0[m_nPairCursor100];
}

// FUNCTION: LEMBALL 0x0041a320
LevelProjectileGeometryIteratorPair* LevelProjectileGeometryPairTableIteratorView::GetRotatedPair(int nIndex)
{
	if (nIndex >= 8) {
		nIndex -= 8;
	}
	return &m_aSecondaryPairsC0[nIndex];
}

// FUNCTION: LEMBALL 0x0041af00
void NoopProjectilePacketHook(void)
{
}
