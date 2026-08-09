#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/CGameObject.h"
#include "Visos/Generic/CVSPoint.h"
#include "views/2d/C2DRender.h"

#include <string.h>

extern void* g_pVariantResourceEntryManager;
extern void* g_pLevelDemoPlaybackController;
extern int g_GAME_ManagedEntityRegistryTable[1000];

struct LevelScreenInputEvent {
	short m_nType00;
	short m_nReserved02;
	int m_nReserved04;
	int m_nReserved08;
	int m_nReserved0C;
	int m_nReserved10;
};

struct LevelScreenInputEventSink {
	virtual void ReservedSlot0(void) = 0;
	virtual void ReservedSlot1(void) = 0;
	virtual void DispatchLevelScreenInputEvent(const LevelScreenInputEvent* pEvent) = 0;
};

struct LevelScreenManagedEntitySelectionView {
	unsigned char m_abReserved000[0x918];
	int m_nCameraOriginX918;           // 0x918
	int m_nCameraOriginY91C;           // 0x91c
	unsigned char m_abReserved920[0x3c];
	void* m_pManagedEntityArray95C;   // 0x95c
	unsigned char m_abReserved960[4]; // 0x960..0x963
	unsigned short m_nEntityCount964;  // 0x964
	unsigned char m_abReserved966[6];  // 0x966..0x96b
	void* m_pLevelMode96C;            // 0x96c
	unsigned char m_abReserved970[4]; // 0x970..0x973
	LevelScreenInputEventSink* m_pInputEventSink974;
	unsigned char m_abReserved978[0xd0];
	int m_nPendingSelectionA48;
	unsigned short m_nPendingSelectionVariantA4C;
	unsigned short m_nGroupCapacityA4E;
	unsigned short m_aGroupObjectNumbersA50[0x100]; // 0xa50..0xbb0

	int FindGameObject(const CVSPoint* pPoint, int* pObjectIndex, unsigned char nMode);
	void NoStateLeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3, unsigned char nParam4);
	void GroupingLeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3);
	void LeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3, unsigned char nParam4);
	void NoStateRightClick(const CVSPoint* pPoint, const CVSPoint* pMarker);
	void RightClick(const CVSPoint* pPoint, const CVSPoint* pMarker);
	void SelectLemming(int nIndex);
	void SelectObject(int nIndex);
	bool InGroupByObjectNo(int nObjectNo);
	void RemoveFromGroupByObjectNo(int nObjectNo);
	bool IsInGrouping(CGameObject* pObject);
	void CheckValidFormGroup(void);
	void FormGroup(void);
	void AddObjectToGroup(int nObjectNo, int fUpdateCapacity);
	void MoveGroup(int* pPosition);
};

struct VariantResourceEntryManagerView {
	unsigned char m_abReserved00[0x10];
	int m_nSelectionMode10;
};

// FUNCTION: LEMBALL 0x00437B60
int C2D::ProcessMsg(short* pMessage)
{
	char* pThis;
	void* pOwner;
	int nCommand;

	pThis = (char*) this;
	if ((g_pLevelDemoPlaybackController == 0 ||
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) &&
		((int(__fastcall*)(void*)) (*(void***) *(void**) (pThis + 0x974))[0x60 / 4])(
			*(void**) (pThis + 0x974)) == 0) {
		return 0;
	}
	if (*(int*) (pThis + 0xa78) != 0) {
		return 0;
	}
	if (*pMessage != 4 && *pMessage != 0x0c) {
		++*(int*) (pThis + 0x0c);
		return 0;
	}
	pOwner = pThis - 4;
	nCommand = *(int*) (pMessage + 4);
	switch (nCommand) {
	case 3:
		((void(__fastcall*)(void*)) 0x00403481)(pOwner);
		return 1;
	case 4:
		((void(__fastcall*)(void*)) 0x00402982)(pOwner);
		return 1;
	case 0x39:
		((void(__fastcall*)(void*, int, int)) 0x00402D60)(pOwner, 0, 3);
		return 1;
	case 0x3a:
	case 0x3b:
	case 0x3c:
	case 0x3d:
		((void(__fastcall*)(void*, int, int)) 0x00401CFD)(pOwner, 0, nCommand - 0x3a);
		return 1;
	case 0x40:
	case 0x41:
	case 0x42:
		((void(__fastcall*)(void*, int, int)) 0x00402D60)(pOwner, 0, nCommand - 0x40);
		return 1;
	default:
		return 0;
	}
}

class LevelScreenViewportRectSnapshot {
public:
	LevelScreenViewportRectSnapshot()
	{
		m_Rect.width = 0;
		m_Rect.height = 0;
		m_Rect.x = 0;
		m_Rect.y = 0;
	}
	~LevelScreenViewportRectSnapshot() {}

	CVSRect m_Rect;
};

// FUNCTION: LEMBALL 0x004368f0
CVSRect* C2D::BuildViewportRectSnapshot(void)
{
	static LevelScreenViewportRectSnapshot Snapshot;
	CVSRect Rect;

	Rect.width = *(short*) ((char*) this + 0x958);
	Rect.height = *(short*) ((char*) this + 0x95a);
	Rect.x = (short) *(int*) ((char*) this + 0x950);
	Rect.y = (short) *(int*) ((char*) this + 0x954);
	Snapshot.m_Rect = Rect;
	return &Snapshot.m_Rect;
}

// Macintosh: C2D::CheckValidFormGroup()
// FUNCTION: LEMBALL 0x004369b0
void LevelScreenManagedEntitySelectionView::CheckValidFormGroup(void)
{
	unsigned int nObjNo;
	unsigned short nIndex;
	unsigned short nCount;
	unsigned short* pGroup;
	void* pObject;

	if (m_nPendingSelectionA48 != 1) {
		return;
	}
	pGroup = m_aGroupObjectNumbersA50;
	nCount = m_nPendingSelectionVariantA4C;
	if (nCount != 0) {
		for (nIndex = 0; nIndex < nCount; ++nIndex) {
			nObjNo = pGroup[nIndex];
			pObject = (void*) g_GAME_ManagedEntityRegistryTable[nObjNo];
			if (((int(__fastcall*)(void*)) (*(void***) pObject)[0x70 / sizeof(void*)])(pObject) == 0) {
				RemoveFromGroupByObjectNo((int) nObjNo);
			}
		}
	}
}

// Macintosh: C2D::FindGameObject(const CVSPoint&, int&, unsigned char)
// FUNCTION: LEMBALL 0x00436e40
int LevelScreenManagedEntitySelectionView::FindGameObject(const CVSPoint* pPoint, int* pObjectIndex, unsigned char nMode)
{
	int* pEntity = (int*) ((char*) m_pManagedEntityArray95C + 0x28);
	unsigned int nCount = m_nEntityCount964;
	int nClickX = (int) pPoint->m_nX - m_nCameraOriginX918;
	int nClickY = (int) pPoint->m_nY - m_nCameraOriginY91C;
	int i = 0;
	int nPrimary = -1;
	int nSecondary = -1;

	while (i < (int) nCount) {
		int nType = pEntity[0];
		int nX = pEntity[-9];
		int nY = pEntity[-8];
		int nLeft;
		int nRight;
		int nTop;
		int nBottom;
		switch (nType) {
		case 2:  nLeft = nX - 8;   nRight = nY - 0x10; nTop = nX + 8;   nBottom = 8;   break;
		case 4:  nLeft = nX - 0x1c; nRight = nY - 0x30; nTop = nX + 0x14; nBottom = 8;   break;
		case 5:  nLeft = nX - 8;   nRight = nY - 0x10; nTop = nX + 8;   nBottom = 2;   break;
		case 0xc: nLeft = nX - 0xd; nRight = nY - 0x1b; nTop = nX + 0xf; nBottom = 2;  break;
		case 0x11: nLeft = nX - 8;  nRight = nY - 0xc;  nTop = nX + 8;  nBottom = 4;   break;
		case 0x14: nLeft = nX - 10; nRight = nY - 0x11; nTop = nX + 8;  nBottom = 2;   break;
		case 0x15: case 0x16: case 0x17:
			nLeft = nX - 8; nRight = nY - 0x20; nTop = nX + 8; nBottom = 0x20; break;
		case 0x1c: nLeft = nX - 10; nRight = nY - 0x25; nTop = nX + 0x24; nBottom = 6; break;
		case 0x22: nLeft = nX - 0x10; nRight = nY - 0x10; nTop = nX + 0x10; nBottom = -8; break;
		case 0x27: case 0x29: case 0x2b: case 0x2d:
			nLeft = nX - 10; nRight = nY - 0x30; nTop = nX + 10; nBottom = 0; break;
		case 0x34: nLeft = nX - 0x10; nRight = nY - 0x10; nTop = nX + 0xf; nBottom = 8; break;
		default: goto next;
		}
		nBottom += nY;
		if (nLeft <= nClickX && nClickX < nTop && nRight <= nClickY && nClickY < nBottom) {
			if (nMode == 0) {
				if (nType == 2) {
					if (nBottom > -1) {
						nSecondary = i;
					}
				}
				else {
					nPrimary = i;
				}
			}
			else if (nType == 2) {
				nPrimary = i;
			}
		}
next:
		pEntity += 0x13;
		++i;
	}
	if (nPrimary != -1) {
		*pObjectIndex = nPrimary;
		return 1;
	}
	if (nSecondary == -1) {
		return 0;
	}
	*pObjectIndex = nSecondary;
	return 1;
}

// Macintosh: C2D::AddObjectToGroup(int, unsigned char)
// FUNCTION: LEMBALL 0x00437130
void LevelScreenManagedEntitySelectionView::AddObjectToGroup(int nObjectNo, int fUpdateCapacity)
{
	m_aGroupObjectNumbersA50[m_nPendingSelectionVariantA4C] = (unsigned short) nObjectNo;
	++m_nPendingSelectionVariantA4C;
	if (fUpdateCapacity != 0) {
		m_nGroupCapacityA4E = m_nPendingSelectionVariantA4C;
	}
}

// Macintosh: C2D::FormGroup()
// FUNCTION: LEMBALL 0x00437170
void LevelScreenManagedEntitySelectionView::FormGroup(void)
{
	LevelScreenInputEvent Event;

	Event.m_nType00 = 5;
	memset(&Event.m_nReserved04, 0, 0x10);
	CheckValidFormGroup();
	if (m_nPendingSelectionVariantA4C != 0) {
		m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
		m_nPendingSelectionVariantA4C = 0;
		m_nPendingSelectionA48 = 0;
		((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 3;
	}
}

// Macintosh: C2D::MoveGroup(AICOORD&)
// FUNCTION: LEMBALL 0x004371e0
void LevelScreenManagedEntitySelectionView::MoveGroup(int* pPosition)
{
	LevelScreenInputEvent Event;

	Event.m_nType00 = 2;
	memset(&Event.m_nReserved04, 0, 0x10);
	Event.m_nReserved08 = *(short*) pPosition;
	Event.m_nReserved0C = *(short*) ((char*) pPosition + 2);
	m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	m_nPendingSelectionVariantA4C = 0;
	m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x25;
}

// Macintosh: C2D::CancelMoves()
// FUNCTION: LEMBALL 0x00437250
void LEMBALL_FASTCALL ClearActiveEntityChildrenPending(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 3;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x25;
}

// Macintosh: C2D::NextGroup()
// FUNCTION: LEMBALL 0x004372a0
void LEMBALL_FASTCALL SelectNextReadyManagedEntityFromLevelScreen(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 7;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x1b;
}

// Macintosh: C2D::PrevGroup()
// FUNCTION: LEMBALL 0x004372f0
void LEMBALL_FASTCALL SelectPreviousReadyManagedEntityFromLevelScreen(LevelScreenManagedEntitySelectionView* pScreen)
{
	LevelScreenInputEvent Event;
	Event.m_nType00 = 6;
	memset(&Event.m_nReserved04, 0, 0x10);
	pScreen->m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	pScreen->m_nPendingSelectionVariantA4C = 0;
	pScreen->m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 0x1b;
}

// Macintosh: C2D::SelectLemming(int)
// FUNCTION: LEMBALL 0x00437340
void LevelScreenManagedEntitySelectionView::SelectLemming(int nIndex)
{
	char* pModeBytes;
	CGameObject* pEntry;
	LevelScreenInputEvent Event;

	pModeBytes = (char*) m_pLevelMode96C;
	pEntry = *((CGameObject**) (pModeBytes + nIndex * 4 + 0x1d0));
	Event.m_nType00 = 8;
	Event.m_nReserved08 = (int) pEntry->m_nRegistryIndex6A;
	memset(&Event.m_nReserved04, 0, 0x10);
	m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	m_nPendingSelectionVariantA4C = 0;
	m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 3;
}

// Macintosh: C2D::SelectObject(int)
// FUNCTION: LEMBALL 0x004373b0
void LevelScreenManagedEntitySelectionView::SelectObject(int nIndex)
{
	char* pObjectsBytes;
	CGameObject* pEntry;
	LevelScreenInputEvent Event;

	pObjectsBytes = (char*) m_pManagedEntityArray95C;
	pEntry = (CGameObject*) (pObjectsBytes + (int) (nIndex * 0x4c));
	Event.m_nType00 = 8;
	Event.m_nReserved08 = (int) *(unsigned short*) ((char*) pEntry + 0x2c);
	memset(&Event.m_nReserved04, 0, 0x10);
	m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	m_nPendingSelectionVariantA4C = 0;
	m_nPendingSelectionA48 = 0;
	((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 3;
}

// Macintosh: C2D::InGroupByObjectNo(int)
// FUNCTION: LEMBALL 0x00437420
bool LevelScreenManagedEntitySelectionView::InGroupByObjectNo(int nObjectNo)
{
	unsigned short nCount;
	unsigned short nIndex;
	unsigned short* pGroup;

	pGroup = m_aGroupObjectNumbersA50;
	nCount = m_nPendingSelectionVariantA4C;
	if (nCount != 0) {
		for (nIndex = 0; nIndex < nCount; ++nIndex) {
			if (pGroup[nIndex] == nObjectNo) {
				return true;
			}
		}
	}
	return false;
}

// Macintosh: C2D::RemoveFromGroupByObjectNo(int)
// FUNCTION: LEMBALL 0x00437460
void LevelScreenManagedEntitySelectionView::RemoveFromGroupByObjectNo(int nObjectNo)
{
	unsigned short nCount;
	unsigned short nIndex;
	unsigned short* pRead;
	unsigned short* pWrite;

	pRead = m_aGroupObjectNumbersA50;
	nCount = m_nPendingSelectionVariantA4C;
	if (nCount == 0) {
		return;
	}
	pWrite = m_aGroupObjectNumbersA50;
	for (nIndex = 0; nIndex < nCount; ++nIndex) {
		if (*pRead != nObjectNo) {
			*pWrite = *pRead;
			++pWrite;
		}
		++pRead;
	}
	--nCount;
	m_nPendingSelectionVariantA4C = nCount;
	if (nCount < m_nGroupCapacityA4E) {
		--m_nGroupCapacityA4E;
	}
	if (nCount == 0) {
		m_nPendingSelectionA48 = 0;
	}
}

// Macintosh: C2D::IsInGrouping(CGameObject*)
// FUNCTION: LEMBALL 0x004374E0
bool LevelScreenManagedEntitySelectionView::IsInGrouping(CGameObject* pObject)
{
	unsigned short nCount;
	unsigned short nIndex;
	unsigned short* pGroup;

	pGroup = m_aGroupObjectNumbersA50;
	nCount = m_nPendingSelectionVariantA4C;
	for (nIndex = 0; nIndex < nCount; ++nIndex) {
		if (pObject->m_nRegistryIndex6A == pGroup[nIndex]) {
			return true;
		}
	}
	return false;
}

// Macintosh: C2D::NoStateLeftClick(const CVSPoint&, const CVSPoint&, unsigned char, unsigned char)
// FUNCTION: LEMBALL 0x00437520
void LevelScreenManagedEntitySelectionView::NoStateLeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3, unsigned char nParam4)
{
	unsigned short nStartX = (unsigned short) pMarker->m_nX;
	unsigned short nStartY = (unsigned short) pMarker->m_nY;
	int nIndex;
	int nObjectNo;

	if (FindGameObject(pPoint, &nIndex, 0) != 0) {
		char* pEntity = (char*) m_pManagedEntityArray95C + nIndex * 0x4c;
		nObjectNo = nIndex;
		switch (*(int*) (pEntity + 0x28)) {
		case 2:
			if (nParam4 == 0) {
				m_nPendingSelectionVariantA4C = 0;
				m_nGroupCapacityA4E = 0;
				m_nPendingSelectionA48 = 1;
				AddObjectToGroup(*(unsigned short*) (pEntity + 0x2c), 0);
				((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 3;
				return;
			}
			break;
		case 4:
		case 5:
		case 0xc:
		case 0x11:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x1c:
		case 0x22:
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			if (nParam3 != 0) {
				ClearActiveEntityChildrenPending(this);
			}
			SelectObject(nObjectNo);
			return;
		case 0x34:
			nStartX = *(unsigned short*) (pEntity + 0x10);
			nStartY = *(unsigned short*) (pEntity + 0x12);
			break;
		default:
			break;
		}
	}
	if (nParam3 != 0) {
		ClearActiveEntityChildrenPending(this);
	}
	CVSPoint point;
	point.m_nX = (short) nStartX;
	point.m_nY = (short) nStartY;
	MoveGroup((int*) &point);
}

// Macintosh: C2D::GroupingLeftClick(const CVSPoint&, const CVSPoint&, unsigned char)
// FUNCTION: LEMBALL 0x004376b0
void LevelScreenManagedEntitySelectionView::GroupingLeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3)
{
	int nIndex;

	if (FindGameObject(pPoint, &nIndex, 0) != 0) {
		char* pEntity = (char*) m_pManagedEntityArray95C + nIndex * 0x4c;
		switch (*(int*) (pEntity + 0x28)) {
		case 2:
			if (nParam3 == 0) {
				unsigned short nValue = *(unsigned short*) ((char*) m_pManagedEntityArray95C + nIndex * 0x4c + 0x2c);
				if (!InGroupByObjectNo(nValue)) {
					AddObjectToGroup(nValue, 0);
				}
				else {
					RemoveFromGroupByObjectNo(nValue);
				}
				((VariantResourceEntryManagerView*) g_pVariantResourceEntryManager)->m_nSelectionMode10 = 3;
				return;
			}
			break;
		case 4:
		case 5:
		case 0xc:
		case 0x11:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x1c:
		case 0x22:
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			if (m_nPendingSelectionVariantA4C != 0) {
				FormGroup();
			}
			SelectObject(nIndex);
			return;
		case 0x34:
			break;
		default:
			break;
		}
	}
	if (m_nPendingSelectionVariantA4C != 0) {
		FormGroup();
		MoveGroup((int*) pMarker);
		return;
	}
	MoveGroup((int*) pMarker);
}

// Macintosh: C2D::LeftClick(const CVSPoint&, const CVSPoint&, unsigned char, unsigned char)
// FUNCTION: LEMBALL 0x00437840
void LevelScreenManagedEntitySelectionView::LeftClick(const CVSPoint* pPoint, const CVSPoint* pMarker, unsigned char nParam3, unsigned char nParam4)
{
	if (m_nPendingSelectionA48 == 0) {
		NoStateLeftClick(pPoint, pMarker, nParam3, nParam4);
		return;
	}
	if (m_nPendingSelectionA48 != 1) {
		return;
	}
	GroupingLeftClick(pPoint, pMarker, nParam4);
}

// Macintosh: C2D::NoStateRightClick(const CVSPoint&, const CVSPoint&)
// FUNCTION: LEMBALL 0x00437890
void LevelScreenManagedEntitySelectionView::NoStateRightClick(const CVSPoint* pPoint, const CVSPoint* pMarker)
{
	LevelScreenInputEvent Event;
	int nIndex;

	Event.m_nType00 = 4;
	Event.m_nReserved02 = 0;
	Event.m_nReserved04 = 0;
	Event.m_nReserved08 = 0;
	Event.m_nReserved0C = 0;
	Event.m_nReserved10 = 0;

	if (FindGameObject(pPoint, &nIndex, 1) != 0 && *(int*) ((char*) m_pManagedEntityArray95C + nIndex * 0x4c + 0x28) == 2) {
		SelectObject(nIndex);
		return;
	}
	Event.m_nReserved08 = (int) pMarker->m_nX;
	Event.m_nReserved0C = (int) pMarker->m_nY;
	m_pInputEventSink974->DispatchLevelScreenInputEvent(&Event);
	m_nPendingSelectionA48 = 0;
}

// Macintosh: C2D::RightClick(const CVSPoint&, const CVSPoint&)
// FUNCTION: LEMBALL 0x00437930
void LevelScreenManagedEntitySelectionView::RightClick(const CVSPoint* pPoint, const CVSPoint* pMarker)
{
	if (m_nPendingSelectionA48 != 0) {
		if (m_nPendingSelectionA48 != 1) {
			return;
		}
		FormGroup();
		if (m_nPendingSelectionVariantA4C == 0) {
			m_nPendingSelectionA48 = 0;
		}
	}
	NoStateRightClick(pPoint, pMarker);
}

extern void* LoadPalResource(int nResourceId);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void* pResourceObject);
extern void* AllocateVSMemBlock(unsigned int nBytes);

// GLOBAL: LEMBALL 0x0049e8b8
static const int g_C2DRemapSourceIndices[17] = {250, 204, 205, 206, 118, 107, 101, 95, 85, 75, 69, 59, 49, 46, 44, 37, 48};
// GLOBAL: LEMBALL 0x0049e8fc
static const int g_C2DRemapTargetIndices[17] = {224, 225, 226, 227, 228, 229, 230, 231, 232, 232, 233, 234, 234, 234, 234, 235, 235};
// GLOBAL: LEMBALL 0x0049ea28
static const unsigned char g_C2DRemapSparsePairs[8] = {2, 0xf1, 0x51, 0xa8, 0x6c};

// Macintosh: C2D::RegisterRemaps()
// FUNCTION: LEMBALL 0x004363c0
void LEMBALL_FASTCALL RegisterLevelScreenPaletteRemapVariants(char* pScreen)
{
	void* pPalette;
	unsigned int cbPalette;
	unsigned char* pRemap;
	int i;
	int j;

	pPalette = LoadPalResource(0x2e);
	cbPalette = *(unsigned int*) ((char*) pPalette + 0x48);
	for (i = 0; i < 4; ++i) {
		pRemap = (unsigned char*) AllocateVSMemBlock(cbPalette);
		*(unsigned char**) (pScreen + 0x54 + i * 4) = pRemap;
		for (j = 0; j < (int) cbPalette; ++j) {
			pRemap[j] = (unsigned char) j;
		}
		for (j = 0; j < 17; ++j) {
			if (g_C2DRemapTargetIndices[j] != 0) {
				pRemap[g_C2DRemapSourceIndices[j]] = (unsigned char) g_C2DRemapTargetIndices[j];
			}
		}
		*(void**) (pScreen + 0x64 + i * 4) = RegisterPaletteRemapVariant(0x2e, pRemap, 0);
	}
	*(void**) (pScreen + 0x74) = RegisterPaletteRemapVariant(0x2e, (unsigned char*) g_C2DRemapSparsePairs, 2);
	ReleaseTypedResourceObjectReference(pPalette);
}

// Macintosh: C2D::SetUpRemapPalettes()
// FUNCTION: LEMBALL 0x00436760
void LEMBALL_FASTCALL RegisterLevelScreenResource0x2eByteRemapTable(char* pScreen)
{
	unsigned char* pRemap;
	unsigned char nValue;
	int i;

	pRemap = (unsigned char*) AllocateVSMemBlock(0x100);
	for (i = 0; i < 0x100; ++i) {
		switch (i) {
		case 0x37:
			nValue = 0x52;
			break;
		case 0x5c:
		case 0x71:
		case 0x75:
			nValue = 0x74;
			break;
		case 0x80:
			nValue = 0x8c;
			break;
		default:
			nValue = (unsigned char) i;
			break;
		}
		pRemap[i] = nValue;
	}
	*(void**) (pScreen + 0x968) = RegisterPaletteRemapVariant(0x2e, pRemap, 0);
}

extern void EmitLevelScreenVariantEntry(void* pObject,
										short x,
										short y,
										int nResourceId,
										int nFlags,
										void* pFrameSelector,
										int nValue);

// Macintosh: C2D::DrawLemmingOnConveyor(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c1a0
void C2D::DrawLemmingOnConveyor(CViewData& ViewData, int nUseRemap)
{
	int nFrame;
	int nRemap = 0;

	nFrame = (((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
	if (nFrame < 0) {
		nFrame = -nFrame;
	}
	nFrame &= 7;
	if (nUseRemap != 0) {
		nRemap = m_nLemmingRemap0968;
	}
	EmitLevelScreenVariantEntry(m_pAnimsManager0A40,
								(short) (ViewData.m_nX04 - 0x0f),
								(short) (ViewData.m_nY08 - 0x16),
								0x3f,
								nFrame,
								0,
								nRemap);
}

// Macintosh: C2D::DrawItemHandBoundaryPair(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c940
void C2D::DrawItemHandBoundaryPair(CViewData& ViewData, int nPlayer)
{
	unsigned int uN;
	int nX;
	int nRemap;
	short sX;
	short sY;

	uN = (((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) & 0x780U) >> 7;
	sX = (short) uN - 4;
	nX = (int) uN - 4;
	if (7 < uN) {
		sX = 0xc - (short) uN;
		nX = 0xc - (int) uN;
	}
	nRemap = (nPlayer < 4) ? m_anPlayerRemaps0064[nPlayer] : 0;
	sX = (short) ViewData.m_nX04 + sX;
	sY = (short) ViewData.m_nY08 + (short) (nX / 4);
	EmitLevelScreenVariantEntry(m_pAnimsManager0A40,
								(short) (sX - 0x10),
								(short) (sY - 0x40),
								0x94,
								0,
								0,
								nRemap);
	EmitLevelScreenVariantEntry(m_pAnimsManager0A40,
								(short) (sX - 9),
								(short) (sY - 9),
								0x96,
								0,
								0,
								0);
}