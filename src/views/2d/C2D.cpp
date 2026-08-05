#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

#include <string.h>

extern void* g_pVariantResourceEntryManager;

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
	unsigned char m_abReserved000[0x974];
	LevelScreenInputEventSink* m_pInputEventSink974;
	unsigned char m_abReserved978[0xd0];
	int m_nPendingSelectionA48;
	unsigned short m_nPendingSelectionVariantA4C;
	unsigned short m_nGroupCapacityA4E;

	void SelectLemming(int nIndex);
	void SelectObject(int nIndex);
	bool InGroupByObjectNo(int nObjectNo);
	void RemoveFromGroupByObjectNo(int nObjectNo);
	bool IsInGrouping(int* pObject);
	void CheckValidFormGroup(void);
	void FormGroup(void);
	void AddObjectToGroup(int nObjectNo, int fUpdateCapacity);
	void MoveGroup(int* pPosition);
};

struct VariantResourceEntryManagerView {
	unsigned char m_abReserved00[0x10];
	int m_nSelectionMode10;
};

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
	pGroup = (unsigned short*) ((char*) this + 0xa50);
	nCount = m_nPendingSelectionVariantA4C;
	if (nCount != 0) {
		for (nIndex = 0; nIndex < nCount; ++nIndex) {
			nObjNo = pGroup[nIndex];
			pObject = *(void**) (nObjNo * 4 + 0x4a6510);
			if (((int(__fastcall*)(void*)) (*(void***) pObject)[0x70 / sizeof(void*)])(pObject) == 0) {
				RemoveFromGroupByObjectNo((int) nObjNo);
			}
		}
	}
}

// Macintosh: C2D::AddObjectToGroup(int, unsigned char)
// FUNCTION: LEMBALL 0x00437130
void LevelScreenManagedEntitySelectionView::AddObjectToGroup(int nObjectNo, int fUpdateCapacity)
{
	unsigned short nCount;
	unsigned short* pGroup;

	pGroup = (unsigned short*) ((char*) this + 0xa50);
	nCount = m_nPendingSelectionVariantA4C;
	pGroup[nCount] = (unsigned short) nObjectNo;
	++nCount;
	m_nPendingSelectionVariantA4C = nCount;
	if (fUpdateCapacity != 0) {
		m_nGroupCapacityA4E = nCount;
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
	char* pEntry;
	LevelScreenInputEvent Event;

	pModeBytes = (char*) *(void**) ((char*) this + 0x96c);
	pEntry = *(char**) (pModeBytes + nIndex * 4 + 0x1d0);
	Event.m_nType00 = 8;
	Event.m_nReserved08 = (int) *(unsigned short*) (pEntry + 0x6a);
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
	char* pEntry;
	LevelScreenInputEvent Event;

	pObjectsBytes = (char*) *(void**) ((char*) this + 0x95c);
	pEntry = pObjectsBytes + (int) (nIndex * 0x4c);
	Event.m_nType00 = 8;
	Event.m_nReserved08 = (int) *(unsigned short*) (pEntry + 0x2c);
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

	pGroup = (unsigned short*) ((char*) this + 0xa50);
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

	pRead = (unsigned short*) ((char*) this + 0xa50);
	nCount = m_nPendingSelectionVariantA4C;
	if (nCount == 0) {
		return;
	}
	pWrite = (unsigned short*) ((char*) this + 0xa50);
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
bool LevelScreenManagedEntitySelectionView::IsInGrouping(int* pObject)
{
	unsigned short nCount;
	unsigned short nIndex;
	unsigned short* pGroup;

	pGroup = (unsigned short*) ((char*) this + 0xa50);
	nCount = m_nPendingSelectionVariantA4C;
	for (nIndex = 0; nIndex < nCount; ++nIndex) {
		if (*(unsigned short*) ((char*) pObject + 0x6a) == pGroup[nIndex]) {
			return true;
		}
	}
	return false;
}
