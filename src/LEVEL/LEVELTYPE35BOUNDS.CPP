struct LevelIntPoint {
	int m_nX;
	int m_nY;
};

struct LevelType35BoundsView {
	char m_abReserved00[0x124];
	int m_nMinX124;
	int m_nMinY128;
	int m_nMaxX12C;
	int m_nMaxY130;

	void GetBounds(int* pMinX, int* pMaxX, int* pMinY, int* pMaxY);
	int ContainsPoint(const LevelIntPoint* pPoint);
	int ContainsFixedPoint(const LevelIntPoint* pPoint);
};

// FUNCTION: LEMBALL 0x0040b5b0
void LevelType35BoundsView::GetBounds(int* pMinX, int* pMaxX, int* pMinY, int* pMaxY)
{
	*pMinX = m_nMinX124;
	*pMinY = m_nMinY128;
	*pMaxX = m_nMaxX12C;
	*pMaxY = m_nMaxY130;
}

// FUNCTION: LEMBALL 0x0040b5f0
int LevelType35BoundsView::ContainsPoint(const LevelIntPoint* pPoint)
{
	int nX;

	nX = pPoint->m_nX;
	return m_nMinX124 <= nX && m_nMinY128 <= pPoint->m_nY && nX <= m_nMaxX12C && pPoint->m_nY <= m_nMaxY130;
}

// FUNCTION: LEMBALL 0x0040b630
int LevelType35BoundsView::ContainsFixedPoint(const LevelIntPoint* pPoint)
{
	int nX;

	nX = pPoint->m_nX >> 12;
	return m_nMinX124 <= nX && m_nMinY128 <= (pPoint->m_nY >> 12) && nX <= m_nMaxX12C &&
		   (pPoint->m_nY >> 12) <= m_nMaxY130;
}
