struct LevelThreeDwordPosition {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct GmobChunkObjectPositionView {
	char m_abReserved00[0x9c];
	LevelThreeDwordPosition m_CurrentPosition9c;

	LevelThreeDwordPosition* CopyCurrentPosition(LevelThreeDwordPosition* pPoint);
	LevelThreeDwordPosition* CopyCurrentPositionVariant(LevelThreeDwordPosition* pPoint);
};

// FUNCTION: LEMBALL 0x0041cc70
LevelThreeDwordPosition* GmobChunkObjectPositionView::CopyCurrentPosition(LevelThreeDwordPosition* pPoint)
{
	int nY;
	int nZ;

	nY = m_CurrentPosition9c.m_nY;
	nZ = m_CurrentPosition9c.m_nZ;
	pPoint->m_nX = m_CurrentPosition9c.m_nX;
	pPoint->m_nY = nY;
	pPoint->m_nZ = nZ;
	return pPoint;
}

// FUNCTION: LEMBALL 0x0041d7b0
LevelThreeDwordPosition* GmobChunkObjectPositionView::CopyCurrentPositionVariant(LevelThreeDwordPosition* pPoint)
{
	int nY;
	int nZ;

	nY = m_CurrentPosition9c.m_nY;
	nZ = m_CurrentPosition9c.m_nZ;
	pPoint->m_nX = m_CurrentPosition9c.m_nX;
	pPoint->m_nY = nY;
	pPoint->m_nZ = nZ;
	return pPoint;
}
