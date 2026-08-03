#include "AI/CMaze.h"

#include "Visos/Generic/Memory.h"

#include <string.h>

struct MazeMapView {
	unsigned char m_abReserved00[0x0c];
	unsigned char* m_pCells0C;
	int m_cColumns10;
	int m_cRows14;
};

struct LevelTileGridOwnerView {
	unsigned char GetWalk(int x, int y);
};

static const int g_anMazeNeighborX[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
static const int g_anMazeNeighborY[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
static const int g_afMazeNeighborEnabled[9] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
static const unsigned char g_abMazeEdgeMaskA[9] = {0, 1, 0, 8, 0, 4, 0, 2, 0};
static const unsigned char g_abMazeEdgeMaskB[9] = {0, 16, 0, 128, 0, 64, 0, 32, 0};
static const int g_anMazeBitMask[8] = {0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1};

extern void __fastcall FillReachabilityGridFromTileFlagsThunk(void* pMaze);

// FUNCTION: LEMBALL 0x00423090
CMaze::CMaze(CMap* pMap)
{
	m_pMap00 = pMap;
	m_ppRows04 = 0;
	m_nFrontier100C = 0;
	m_nReserved08 = 0;
	m_cColumns1010 = 0;
	m_cRows1014 = 0;
}

// FUNCTION: LEMBALL 0x004230c0
CMaze::~CMaze(void)
{
	int i;

	if (m_ppRows04 != 0) {
		for (i = 0; i < m_cRows1014; ++i) {
			FreeVSMemBlock(m_ppRows04[i]);
		}
		FreeVSMemBlock(m_ppRows04);
	}
}

// FUNCTION: LEMBALL 0x00423110
void CMaze::ReInitialise(void)
{
	MazeMapView* pMap;
	unsigned short nCellValue;
	int y;
	int x;

	pMap = (MazeMapView*) m_pMap00;
	for (y = 0; y < m_cRows1014; ++y) {
		for (x = 0; x < m_cColumns1010; ++x) {
			if (x < 0 || y < 0 || pMap->m_cColumns10 <= x || pMap->m_cRows14 <= y) {
				nCellValue = 3;
			}
			else {
				nCellValue = *(unsigned short*) (pMap->m_pCells0C + 6 + (y * pMap->m_cColumns10 + x) * 0xc);
			}
			m_ppRows04[y][x] = (nCellValue & 1) == 0 ? 0xff00 : 0xffff;
		}
	}
}

// FUNCTION: LEMBALL 0x00423190
void CMaze::Initialise(void)
{
	MazeMapView* pMap;
	int i;

	if (m_ppRows04 != 0) {
		for (i = 0; i < m_cRows1014; ++i) {
			FreeVSMemBlock(m_ppRows04[i]);
		}
		FreeVSMemBlock(m_ppRows04);
	}
	pMap = (MazeMapView*) m_pMap00;
	m_cColumns1010 = pMap->m_cColumns10;
	m_cRows1014 = pMap->m_cRows14;
	m_ppRows04 = (unsigned short**) AllocateVSMemBlock((unsigned int) m_cRows1014 << 2);
	for (i = 0; i < m_cRows1014; ++i) {
		m_ppRows04[i] = (unsigned short*) AllocateVSMemBlock((unsigned int) m_cColumns1010 * 2);
	}
	FillReachabilityGridFromTileFlagsThunk(this);
}

// FUNCTION: LEMBALL 0x00423230
int CMaze::CalcNewDistance(int x, int y)
{
	unsigned short nDistance;
	unsigned short nNeighborDistance;
	unsigned char nWalk;
	int fChanged;
	int i;

	nDistance = m_ppRows04[y][x];
	nWalk = ((LevelTileGridOwnerView*) m_pMap00)->GetWalk(x, y);
	fChanged = 0;
	for (i = 0; i < 9; ++i) {
		if (g_afMazeNeighborEnabled[i] != 0 && (g_abMazeEdgeMaskA[i] & nWalk) != 0 &&
			(g_abMazeEdgeMaskB[i] & nWalk) != 0) {
			nNeighborDistance = m_ppRows04[y + g_anMazeNeighborY[i]][x + g_anMazeNeighborX[i]];
			if (nNeighborDistance < nDistance) {
				fChanged = 1;
				nDistance = nNeighborDistance;
			}
		}
	}
	if (fChanged != 0) {
		m_ppRows04[y][x] = nDistance + 1;
	}
	return fChanged;
}

// FUNCTION: LEMBALL 0x004232e0
int CMaze::FindSquare(unsigned short nDistance, int& x, int& y)
{
	unsigned char nWalk;
	int nCurrentX;
	int nCurrentY;
	int nNeighborX;
	int nNeighborY;
	int fFound;
	int i;

	nCurrentX = x;
	nCurrentY = y;
	nWalk = ((LevelTileGridOwnerView*) m_pMap00)->GetWalk(nCurrentX, nCurrentY);
	fFound = 0;
	for (i = 0; i < 9; ++i) {
		if (g_afMazeNeighborEnabled[i] != 0 && (g_abMazeEdgeMaskA[i] & nWalk) != 0) {
			nNeighborX = nCurrentX + g_anMazeNeighborX[i];
			nNeighborY = nCurrentY + g_anMazeNeighborY[i];
			if (m_ppRows04[nNeighborY][nNeighborX] == nDistance) {
				fFound = 1;
				x = nNeighborX;
				y = nNeighborY;
				break;
			}
		}
	}
	return fFound;
}

// FUNCTION: LEMBALL 0x00423380
void CMaze::UpdateChangeNext(int x, int y)
{
	unsigned char* pCurrentRow;
	unsigned char* pNextRow;
	unsigned char* pCurrent;
	unsigned char* pNext;
	unsigned char nMask;
	unsigned char nInitialMask;
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	int nColumn;
	int nRow;

	nLeft = x > 0 ? x - 1 : x;
	nTop = y > 0 ? y - 1 : y;
	nRight = x < m_cColumns1010 - 1 ? x + 1 : x;
	nBottom = y < m_cRows1014 - 1 ? y + 1 : y;
	if (m_nFrontier100C == 0) {
		pCurrentRow = m_abFrontiers0C + ((nTop * 0x80 + nLeft) >> 3);
		pNextRow = m_abFrontiers0C + 0x800 + ((nTop * 0x80 + nLeft) >> 3);
	}
	else {
		pCurrentRow = m_abFrontiers0C + 0x800 + ((nTop * 0x80 + nLeft) >> 3);
		pNextRow = m_abFrontiers0C + ((nTop * 0x80 + nLeft) >> 3);
	}
	nInitialMask = (unsigned char) g_anMazeBitMask[nLeft & 7];
	for (nRow = nTop; nRow <= nBottom; ++nRow) {
		pCurrent = pCurrentRow;
		pNext = pNextRow;
		nMask = nInitialMask;
		for (nColumn = nLeft; nColumn <= nRight; ++nColumn) {
			if (nColumn != x || nRow != y) {
				*pCurrent |= nMask;
				*pNext |= nMask;
			}
			nMask >>= 1;
			if (nMask == 0) {
				++pCurrent;
				++pNext;
				nMask = 0x80;
			}
		}
		pCurrentRow += 0x10;
		pNextRow += 0x10;
	}
}

// FUNCTION: LEMBALL 0x004234a0
void CMaze::Clear(unsigned char* pFrontier)
{
	int y;

	for (y = 0; y < m_cRows1014; ++y) {
		memset(pFrontier, 0, m_cColumns1010 / 8);
		pFrontier += 0x10;
	}
}

// FUNCTION: LEMBALL 0x004234f0
void CMaze::SwapChange(void)
{
	if (m_nFrontier100C == 0) {
		Clear(m_abFrontiers0C + 0x800);
	}
	else {
		Clear(m_abFrontiers0C);
	}
	m_nFrontier100C = m_nFrontier100C == 0;
}
