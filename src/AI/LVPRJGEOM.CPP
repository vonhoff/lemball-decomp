struct LevelProjectilePoolView {
	char m_abReserved00[0x34];
	void* m_apObjects34[40];
	int m_cObjectsD4;
	int m_iObjectD8;

	void Append(void* pObject);
};

// FUNCTION: LEMBALL 0x00417f30
int __fastcall GetProjectilePoolObjectCount(LevelProjectilePoolView* pPool)
{
	return pPool->m_cObjectsD4;
}

// FUNCTION: LEMBALL 0x00417f70
void* __fastcall GetCurrentProjectilePoolObject(LevelProjectilePoolView* pPool)
{
	return pPool->m_apObjects34[pPool->m_iObjectD8];
}

// FUNCTION: LEMBALL 0x00417f80
void LevelProjectilePoolView::Append(void* pObject)
{
	m_apObjects34[m_cObjectsD4++] = pObject;
}

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


// FUNCTION: LEMBALL 0x0041af00
void NoopProjectilePacketHook(void)
{
}
