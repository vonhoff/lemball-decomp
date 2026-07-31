struct LevelBoundsPoint {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct GmobAction29PointView {
	char m_abReserved00[0x9c];
	int m_nX;
	int m_nY;
	int m_nZ;

	void CopyActionPoint(LevelBoundsPoint* pPoint);
};

// FUNCTION: LEMBALL 0x0041d5d0
void GmobAction29PointView::CopyActionPoint(LevelBoundsPoint* pPoint)
{
	int nX;
	int nZ;

	nZ = m_nZ;
	nX = m_nX;
	pPoint->m_nY = m_nY;
	pPoint->m_nX = nX - 0x8000;
	pPoint->m_nZ = nZ;
}
