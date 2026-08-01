#include "ENGINE/CORE/COMMON.H"

struct LevelPositionTableView {
	unsigned char m_abReserved00[0xe0];
	int m_anX[4];
	int m_anY[4];
	int m_anZ[4];
};

struct LevelPositionColumnView {
	unsigned char m_abReserved00[0xe0];
	int m_nX;
	unsigned char m_abReservedE4[0x0c];
	int m_nY;
	unsigned char m_abReservedF4[0x0c];
	int m_nZ;
};

struct LevelModePositionView {
	unsigned char m_abReserved00[0x15c];
	LevelPositionTableView* m_pPositions;

	void CopySelectedEntityPosition(int* pnX, int* pnY, int* pnZ);
	void CopyPosition(int* pnX, int* pnY, int* pnZ, int iPosition);
};

// FUNCTION: LEMBALL 0x00412d50
void LevelModePositionView::CopySelectedEntityPosition(int* pnX, int* pnY, int* pnZ)
{
	LevelPositionTableView* pPositions = m_pPositions;
	*pnX = pPositions->m_anX[0];
	*pnY = pPositions->m_anY[0];
	*pnZ = pPositions->m_anZ[0];
}

// FUNCTION: LEMBALL 0x00412d80
void LevelModePositionView::CopyPosition(int* pnX, int* pnY, int* pnZ, int iPosition)
{
	LevelPositionColumnView* pPosition = (LevelPositionColumnView*) ((char*) m_pPositions + iPosition * sizeof(int));
	*pnX = pPosition->m_nX;
	*pnY = pPosition->m_nY;
	*pnZ = pPosition->m_nZ;
}
