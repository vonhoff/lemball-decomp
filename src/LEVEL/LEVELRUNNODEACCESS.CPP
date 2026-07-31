struct LevelTwoDwordPairVariantView {
	int m_anValues[2];

	LevelTwoDwordPairVariantView* Assign(const int* pSource);
};

// FUNCTION: LEMBALL 0x00417b30
LevelTwoDwordPairVariantView* LevelTwoDwordPairVariantView::Assign(const int* pSource)
{
	m_anValues[0] = pSource[0];
	m_anValues[1] = pSource[1];
	return this;
}

struct LevelNodeChunkPositionView {
	int m_nX;
	int m_nY;

	void CopyFixedPointPosition(int* pDestination);
	void CopyTileXY(int* pX, int* pY);
};

// FUNCTION: LEMBALL 0x004213d0
void LevelNodeChunkPositionView::CopyFixedPointPosition(int* pDestination)
{
	int y;
	int x;

	y = m_nY;
	x = m_nX;
	pDestination[0] = x;
	pDestination[1] = y;
	pDestination[2] = (int) 0xaa55aa55;
}

// FUNCTION: LEMBALL 0x004213f0
void LevelNodeChunkPositionView::CopyTileXY(int* pX, int* pY)
{
	*pX = m_nX >> 12;
	*pY = m_nY >> 12;
}

struct LevelTwoDwordPairSourceView {
	int m_anValues[2];

	void CopyTo(int* pDestination);
};

// FUNCTION: LEMBALL 0x00421410
void LevelTwoDwordPairSourceView::CopyTo(int* pDestination)
{
	pDestination[0] = m_anValues[0];
	pDestination[1] = m_anValues[1];
}
