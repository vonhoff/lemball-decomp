// FUNCTION: LEMBALL 0x0040ce90
void NoopType18PacketHook(void)
{
}

// FUNCTION: LEMBALL 0x0040ff60
void NoopProjectileBonusHook(void)
{
}

struct LevelVtSmallFunctionView {
	char m_abReserved00[0xf0];
	int m_nScoreF0;
	char m_abReservedF4[0x134];
	unsigned short m_nBonus228;

	void AddClampedU16Field228(unsigned short nValue);
	void AddLevelScoreClamped(int nValue);
};

// FUNCTION: LEMBALL 0x00410ac0
void LevelVtSmallFunctionView::AddClampedU16Field228(unsigned short nValue)
{
	unsigned short nResult;

	nResult = (unsigned short) (m_nBonus228 + nValue);
	m_nBonus228 = nResult;
	if (nResult > 0x32) {
		m_nBonus228 = 0x32;
	}
}

// FUNCTION: LEMBALL 0x00413390
void LevelVtSmallFunctionView::AddLevelScoreClamped(int nValue)
{
	int nResult;

	nResult = m_nScoreF0 + nValue;
	m_nScoreF0 = nResult;
	if (nResult > 9999999) {
		m_nScoreF0 = 9999999;
	}
}

struct LevelManagedEntityResetStateView {
	char m_abReserved00[0x8c];
	int m_nRuntimeState8C;
	char m_abReserved90[0x84];
	int m_nPendingState114;
	char m_abReserved118[0x18];
	int m_fResetPending130;
	int m_nLatchedValue134;

	int ClearLatchedValue(void);
	void ApplyPendingReset(void);
};

// FUNCTION: LEMBALL 0x00416d90
int LevelManagedEntityResetStateView::ClearLatchedValue(void)
{
	int nValue;

	nValue = m_nLatchedValue134;
	if (nValue != 0) {
		m_nLatchedValue134 = 0;
	}
	return nValue;
}

// FUNCTION: LEMBALL 0x00416e70
void LevelManagedEntityResetStateView::ApplyPendingReset(void)
{
	if (m_fResetPending130 != 0) {
		m_nPendingState114 = 0;
		m_fResetPending130 = 0;
		m_nRuntimeState8C = 0;
	}
}

struct LevelManagedEntityChildSlotRecord {
	int m_nState00;
	int m_nValue04;
	int m_nValue08;
	int m_nValue0C;

	LevelManagedEntityChildSlotRecord* Initialize(void);
};

// FUNCTION: LEMBALL 0x00417ab0
LevelManagedEntityChildSlotRecord* LevelManagedEntityChildSlotRecord::Initialize(void)
{
	int nSentinel;

	nSentinel = (int) 0xaa55aa55;
	m_nState00 = 0;
	m_nValue04 = nSentinel;
	m_nValue08 = nSentinel;
	m_nValue0C = nSentinel;
	return this;
}

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

	void InitializePosition(int nX, int nY, int nUnused);
	void SetFixedPointPosition(int nX, int nY);
	void CopyFixedPointPosition(int* pDestination);
	void CopyTileXY(int* pX, int* pY);
};

// FUNCTION: LEMBALL 0x00421370
void LevelNodeChunkPositionView::InitializePosition(int nX, int nY, int nUnused)
{
	(void) nUnused;
	m_nX = nX;
	m_nY = nY;
}

// FUNCTION: LEMBALL 0x00421380
void LevelNodeChunkPositionView::SetFixedPointPosition(int nX, int nY)
{
	m_nX = nX << 12;
	m_nY = nY << 12;
}

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

// FUNCTION: LEMBALL 0x0042eed0
int __stdcall ReturnZeroMoveChunkCallback(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x0042fb90
void NoopMoveChunkPathSearch(void)
{
}
