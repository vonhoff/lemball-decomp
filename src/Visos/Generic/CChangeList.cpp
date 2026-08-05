#include "Visos/Generic/CChangeList.h"

#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x004669a0
CChangeList::CChangeList(int cItems, CVSSize Size, CVSSize CellSize)
{
	CChangeListItem* pItem;
	int i;

	m_Size14.width = 0;
	m_Size14.height = 0;
	m_CellSize18.width = 0;
	m_CellSize18.height = 0;
	m_MapSize1C.width = 0;
	m_MapSize1C.height = 0;
	m_Area30.width = 0;
	m_Area30.height = 0;
	m_Area30.x = 0;
	m_Area30.y = 0;

	m_pItems0C = 0;
	if (cItems != 0) {
		m_pItems0C = (CChangeListItem*) AllocateVSMemBlock((unsigned int) cItems * sizeof(CChangeListItem));
	}
	pItem = m_pItems0C;
	if (pItem != 0) {
		for (i = 0; i < cItems; ++i) {
			pItem->m_Rect.width = 0;
			pItem->m_Rect.height = 0;
			pItem->m_Rect.x = 0;
			pItem->m_Rect.y = 0;
			pItem->m_nState = 0;
			++pItem;
		}
	}

	m_cCapacity00 = cItems;
	m_CellSize18 = CellSize;
	m_Size14 = Size;
	m_MapSize1C.width = (short) ((Size.width + CellSize.width - 1) / CellSize.width);
	m_MapSize1C.height = (short) ((Size.height + CellSize.height - 1) / CellSize.height);
	m_cActive44 = 0;
	m_pMap10 = 0;
	AllocMap();
	Reset();
}

// FUNCTION: LEMBALL 0x00466a90
void CChangeList::FreeMap(void)
{
	if (m_pMap10 != 0) {
		FreeVSMemBlock(m_pMap10);
		m_pMap10 = 0;
	}
	Reset();
}

// FUNCTION: LEMBALL 0x00466ac0
void CChangeList::AllocMap(void)
{
	if (m_pMap10 == 0) {
		m_cMapEntries20 = (int) m_MapSize1C.width * (int) m_MapSize1C.height;
		m_pMap10 = (unsigned char*) AllocateVSMemBlock((unsigned int) m_cMapEntries20);
	}
}

// FUNCTION: LEMBALL 0x00466af0
void CChangeList::Resize(CVSSize Size)
{
	short cxMap;
	short cyMap;
	int cMapEntries;

	if (m_Size14.width == Size.width && m_Size14.height == Size.height) {
		return;
	}

	cxMap = (short) (Size.width / m_CellSize18.width);
	cyMap = (short) (Size.height / m_CellSize18.height);
	m_Size14 = Size;
	m_MapSize1C.width = cxMap;
	m_MapSize1C.height = cyMap;
	cMapEntries = (int) cxMap * (int) cyMap;
	if (cMapEntries != m_cMapEntries20 && m_cMapEntries20 <= cMapEntries) {
		FreeMap();
	}
	if ((int) Size.width * (int) Size.height > 0) {
		AllocMap();
	}
}

// FUNCTION: LEMBALL 0x00466b60
void __fastcall InitializeHelperUploadStatePending(int nChangeList)
{
	CChangeList* pChangeList;

	pChangeList = (CChangeList*) (unsigned long) nChangeList;
	pChangeList->m_Area30.width = 0;
	pChangeList->m_nArea2C = 0;
	// MSVC 4.00: preserve the original zero-store ordering.
	__asm {
	}
	pChangeList->m_nActive48 = 'P';
	pChangeList->m_Area30.height = 0;
	pChangeList->m_Area30.x = 0;
	pChangeList->m_Area30.y = 0;
	pChangeList->m_cItems04 = -1;
	pChangeList->m_iDrawMark08 = -1;
}

// FUNCTION: LEMBALL 0x00466b90
CChangeList::~CChangeList()
{
	if (m_pItems0C != 0) {
		FreeVSMemBlock(m_pItems0C);
		m_pItems0C = 0;
	}
	FreeMap();
}

// FUNCTION: LEMBALL 0x00466bc0
void CChangeList::PushActive(unsigned char)
{
	m_anActive38[m_cActive44] = m_nActive48;
	++m_cActive44;
}

// FUNCTION: LEMBALL 0x00466bd0
void __fastcall PopHelperUploadStateMarker(void* pObject)
{
	CChangeList* pChangeList;

	pChangeList = (CChangeList*) pObject;
	--pChangeList->m_cActive44;
	pChangeList->m_nActive48 = pChangeList->m_anActive38[pChangeList->m_cActive44];
}

// FUNCTION: LEMBALL 0x00466be0
void CChangeList::Add(const CVSRect& Rect)
{
	short cxCell;
	short cyCell;
	int nFirstColumn;
	int nFirstRow;
	unsigned int cColumns;
	int cRows;
	int nGridWidth;
	int nRow;
	unsigned char* pRow;
	unsigned int i;

	if (m_pMap10 == 0) {
		return;
	}

	cxCell = m_CellSize18.width;
	cyCell = m_CellSize18.height;
	nFirstColumn = Rect.x / cxCell;
	nFirstRow = Rect.y / cyCell;
	cColumns = (unsigned int) ((Rect.width + Rect.x - 1 + cxCell) / cxCell - nFirstColumn);
	cRows = (Rect.height + Rect.y - 1 + cyCell) / cyCell - nFirstRow;
	if (m_MapSize1C.height < nFirstRow + cRows) {
		cRows = m_MapSize1C.height - nFirstRow;
	}
	nGridWidth = m_MapSize1C.width;
	if (nGridWidth < nFirstColumn + (int) cColumns) {
		cColumns = (unsigned int) (nGridWidth - nFirstColumn);
	}

	if ((int) cColumns <= 0 || cRows <= 0) {
		return;
	}

	pRow = m_pMap10 + nGridWidth * nFirstRow + nFirstColumn;
	nRow = cRows;
	do {
		for (i = 0; i < cColumns; ++i) {
			pRow[i] = m_nActive48;
		}
		pRow += nGridWidth;
		--nRow;
	} while (nRow != 0);
	m_nArea2C += (int) Rect.width * (int) Rect.height;
}

// FUNCTION: LEMBALL 0x00466ce0
void CChangeList::SetDrawMark(const CVSRect& Rect, unsigned char nMark)
{
	unsigned char nPrevious;

	nPrevious = m_nActive48;
	m_nActive48 = nMark;
	Add(Rect);
	m_nActive48 = nPrevious;
}

// FUNCTION: LEMBALL 0x00466d10
void __fastcall PromoteHelperUploadStateToActive(int nChangeList)
{
	CChangeList* pChangeList;

	pChangeList = (CChangeList*) (unsigned long) nChangeList;
	switch (pChangeList->m_nActive48) {
	case 'P':
		pChangeList->m_nActive48 = 'A';
		break;
	case 'p':
		pChangeList->m_nActive48 = 'a';
		break;
	}
}

// FUNCTION: LEMBALL 0x00466d40
int CChangeList::GetNextArea(unsigned char nTarget, unsigned char nState, unsigned char nReplacement)
{
	unsigned char* pRow;
	int cxGrid;
	int cyGrid;
	int cxCell;
	int cyCell;
	int xCell;
	int yCell;
	int xRunEnd;
	int cRunCells;
	int cRunRows;
	int x;
	CChangeListItem* pItem;

	cxGrid = m_MapSize1C.width;
	cyGrid = m_MapSize1C.height;
	cxCell = m_CellSize18.width;
	cyCell = m_CellSize18.height;
	xCell = m_iScanX24;
	yCell = m_iScanY28;
	while (yCell < cyGrid) {
		pRow = m_pMap10 + yCell * cxGrid;
		while (xCell < cxGrid && pRow[xCell] != nTarget) {
			++xCell;
		}
		if (xCell < cxGrid) {
			break;
		}
		xCell = 0;
		++yCell;
	}
	if (yCell >= cyGrid) {
		return 0;
	}

	pRow = m_pMap10 + yCell * cxGrid;
	xRunEnd = xCell;
	while (xRunEnd < cxGrid && pRow[xRunEnd] == nTarget) {
		pRow[xRunEnd++] = nReplacement;
	}
	cRunCells = xRunEnd - xCell;
	cRunRows = 1;
	while (yCell + cRunRows < cyGrid) {
		pRow = m_pMap10 + (yCell + cRunRows) * cxGrid;
		x = xCell;
		while (x < cxGrid && pRow[x] == nTarget) {
			++x;
		}
		if (x - xCell != cRunCells) {
			break;
		}
		for (x = 0; x < cRunCells; ++x) {
			pRow[xCell + x] = nReplacement;
		}
		++cRunRows;
	}

	pItem = m_pItems0C + m_cItems04;
	pItem->m_Rect.width = (short) (cRunCells * cxCell);
	pItem->m_Rect.height = (short) (cRunRows * cyCell);
	pItem->m_Rect.x = (short) (xCell * cxCell);
	pItem->m_Rect.y = (short) (yCell * cyCell);
	pItem->m_nState = nState;
	++m_cItems04;
	m_iScanX24 = xRunEnd < cxGrid ? xRunEnd : 0;
	m_iScanY28 = yCell;
	return 1;
}

// FUNCTION: LEMBALL 0x00466ef0
int CChangeList::GetNumItems(void)
{
	if (m_pMap10 == 0 || m_pItems0C == 0) {
		return 0;
	}
	if (m_cItems04 == -1) {
		m_cItems04 = 0;
		m_iScanX24 = 0;
		m_iScanY28 = 0;
		while (GetNextArea(1, 1, 0)) {
		}
		m_iScanX24 = 0;
		m_iScanY28 = 0;
		while (GetNextArea('P', 1, 1)) {
		}
		m_iScanX24 = 0;
		m_iScanY28 = 0;
		while (GetNextArea('p', 0, 1)) {
		}
		m_iDrawMark08 = m_cItems04;
		if (m_nActive48 == 'A' || m_nActive48 == 'a') {
			m_iScanX24 = 0;
			m_iScanY28 = 0;
			while (GetNextArea('A', 1, 1)) {
			}
			m_iScanX24 = 0;
			m_iScanY28 = 0;
			while (GetNextArea('a', 0, 1)) {
			}
		}
	}
	return m_cItems04;
}

// FUNCTION: LEMBALL 0x00467000
CChangeListItem* CChangeList::GetNItem(int iItem)
{
	if (m_cItems04 == -1) {
		GetNumItems();
	}
	return m_pItems0C + iItem;
}

// FUNCTION: LEMBALL 0x00467020
int CChangeList::GetDrawMark(void)
{
	if (m_pMap10 == 0) {
		return 0;
	}
	if (m_cItems04 == -1) {
		GetNumItems();
	}
	return m_iDrawMark08;
}
