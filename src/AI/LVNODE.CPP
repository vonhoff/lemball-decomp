extern void* AllocateVSMemBlock(unsigned int cbBlock);
extern void FreeVSMemBlock(void* pMemory);
extern int g_nLevelFrameClockTick;

// FUNCTION: LEMBALL 0x0040ce90
void NoopType18PacketHook(void)
{
}

// MACINTOSH: CPlayerLemming::StartBalloon()
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

struct LevelObjectTimerPredicateView {
	unsigned char m_abReserved00[0x190];
	unsigned int m_nDeadline190;
};

// FUNCTION: LEMBALL 0x004109f0
int __fastcall IsObjectTimerExpired(LevelObjectTimerPredicateView* pObject)
{
	return pObject->m_nDeadline190 < (unsigned int) g_nLevelFrameClockTick;
}

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

struct CNode;

struct LevelNodePoint {
	int m_anValues[3];
};

struct CNodeManager {
	CNode* m_pRecords00;
	int m_nReserved04;
	int m_cCapacity08;

	CNodeManager(int cCapacity);
	~CNodeManager(void);
	void Initialise(int cRecords);
	LevelNodePoint* GetNodePosition(LevelNodePoint* pOutput, int nIndex);
	void CopyTileByIndex(int nIndex, int* pX, int* pY);
	void Restart(void);
};

struct LevelModeNodeView {
	char m_abReserved00[0x17c];
	CNodeManager* m_pNodeManager17C;

	LevelNodePoint* CopyNodePointByIndex(LevelNodePoint* pOutput, int nIndex);
};

// MACINTOSH: CAI::GetNodePosition(int)
// FUNCTION: LEMBALL 0x00412b60
LevelNodePoint* LevelModeNodeView::CopyNodePointByIndex(LevelNodePoint* pOutput, int nIndex)
{
	m_pNodeManager17C->GetNodePosition(pOutput, nIndex);
	return pOutput;
}

// MACINTOSH: CAI::Score(int)
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

struct CNode {
	int m_nX;
	int m_nY;
	int m_cExpectedLinks08;
	int* m_pLinks0C;
	int m_nReserved10;

	CNode(void);
	void AddANeighbour(int nFirst, int nSecond);
	void Initialise(int nX, int nY, int cLinks);
	CNode* InitializeNodeChunkRecordThunk(void);
	void InitializePosition(int nX, int nY, int nUnused);
	void Restart(void);
	void RestartThunk(void);
	~CNode(void);
	void SetFixedPointPosition(int nX, int nY);
	void Position(int* pDestination);
	void CopyTileXY(int* pX, int* pY);
};

// FUNCTION: LEMBALL 0x00421180
CNodeManager::CNodeManager(int cCapacity)
{
	m_pRecords00 = 0;
	m_cCapacity08 = cCapacity;
}

// FUNCTION: LEMBALL 0x004211a0
void CNodeManager::Restart(void)
{
	int i;

	if (m_pRecords00 != 0 && 0 < m_cCapacity08) {
		for (i = 0; i < m_cCapacity08; ++i) {
			m_pRecords00[i].RestartThunk();
		}
	}
}

// FUNCTION: LEMBALL 0x004211d0
void CNodeManager::Initialise(int cRecords)
{
	int i;
	int cCapacity;
	int* pAllocation;

	m_nReserved04 = 0;
	if (cRecords == 0) {
		m_pRecords00 = 0;
		return;
	}
	if (m_pRecords00 != 0) {
		return;
	}

	cCapacity = m_cCapacity08;
	pAllocation = (int*) AllocateVSMemBlock((unsigned int) cCapacity * sizeof(CNode) + 4);
	if (pAllocation != 0) {
		*pAllocation = cCapacity;
		m_pRecords00 = (CNode*) (pAllocation + 1);
		for (i = 0; i < cCapacity; ++i) {
			m_pRecords00[i].InitializeNodeChunkRecordThunk();
		}
	}
	else {
		m_pRecords00 = 0;
	}

	for (i = 0; i < m_cCapacity08; ++i) {
		m_pRecords00[i].RestartThunk();
	}
}

// FUNCTION: LEMBALL 0x00421260
CNodeManager::~CNodeManager(void)
{
	int i;

	if (m_pRecords00 == 0) {
		return;
	}
	i = *((int*) m_pRecords00 - 1);
	while (--i >= 0) {
		m_pRecords00[i].~CNode();
	}
	FreeVSMemBlock((int*) m_pRecords00 - 1);
}

// FUNCTION: LEMBALL 0x004212a0
LevelNodePoint* CNodeManager::GetNodePosition(LevelNodePoint* pOutput, int nIndex)
{
	m_pRecords00[nIndex].Position(pOutput->m_anValues);
	return pOutput;
}

// FUNCTION: LEMBALL 0x004212c0
void CNodeManager::CopyTileByIndex(int nIndex, int* pX, int* pY)
{
	m_pRecords00[nIndex].CopyTileXY(pX, pY);
}

// FUNCTION: LEMBALL 0x004212e0
CNode::CNode(void)
{
	m_nX = 0xaa55aa55;
	m_nY = 0xaa55aa55;
	m_pLinks0C = 0;
}

// FUNCTION: LEMBALL 0x00421300
void CNode::Restart(void)
{
	m_nReserved10 = 0;
}

// FUNCTION: LEMBALL 0x00421310
CNode::~CNode(void)
{
	if (m_pLinks0C != 0) {
		FreeVSMemBlock(m_pLinks0C);
	}
}

// FUNCTION: LEMBALL 0x00421330
void CNode::Initialise(int nX, int nY, int cLinks)
{
	if (cLinks != 0 && m_pLinks0C != 0) {
		m_pLinks0C = (int*) AllocateVSMemBlock((unsigned int) cLinks * 8);
	}
	m_cExpectedLinks08 = cLinks;
	m_nX = nX << 12;
	m_nY = nY << 12;
}

// FUNCTION: LEMBALL 0x00421370
void CNode::InitializePosition(int nX, int nY, int nUnused)
{
	(void) nUnused;
	m_nX = nX;
	m_nY = nY;
}

// FUNCTION: LEMBALL 0x00421380
void CNode::SetFixedPointPosition(int nX, int nY)
{
	m_nX = nX << 12;
	m_nY = nY << 12;
}

// FUNCTION: LEMBALL 0x004213a0
void CNode::AddANeighbour(int nFirst, int nSecond)
{
	m_pLinks0C[m_nReserved10 * 2] = nFirst;
	m_pLinks0C[m_nReserved10 * 2 + 1] = nSecond;
	++m_nReserved10;
}

// FUNCTION: LEMBALL 0x004213d0
void CNode::Position(int* pDestination)
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
void CNode::CopyTileXY(int* pX, int* pY)
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
