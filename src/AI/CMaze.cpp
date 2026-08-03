#include "AI/CMaze.h"

#include "Visos/Generic/Memory.h"

struct MazeMapView {
	unsigned char m_abReserved00[0x0c];
	unsigned char* m_pCells0C;
	int m_cColumns10;
	int m_cRows14;
};

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
