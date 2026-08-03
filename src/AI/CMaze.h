#ifndef LEMBALL_CMAZE_H
#define LEMBALL_CMAZE_H

struct CMap;

struct tSolution {
	short x;
	short y;
};

class CMaze {
public:
	CMaze(CMap* pMap);
	~CMaze(void);
	void ReInitialise(void);
	void Initialise(void);
	int CalcNewDistance(int x, int y);
	int FindSquare(unsigned short nDistance, int& x, int& y);
	void UpdateChangeNext(int x, int y);
	void Clear(unsigned char* pFrontier);
	void SwapChange(void);
	void BInitialise(unsigned char fReset, int nStartX, int nStartY, int nEndX, int nEndY);
	int BIteration(unsigned char& fFound, unsigned char& fExhausted);
	void BSolution(int& cSolutions, tSolution* pSolutions);

private:
	CMap* m_pMap00;
	unsigned short** m_ppRows04;
	int m_nReserved08;
	unsigned char m_abFrontiers0C[0x1000];
	unsigned char m_nFrontier100C;
	unsigned char m_abReserved100D[3];
	int m_cColumns1010;
	int m_cRows1014;
	int m_nStartX1018;
	int m_nStartY101C;
	int m_nEndX1020;
	int m_nEndY1024;
	int m_nReserved1028;
	unsigned long m_dwStartTime102C;
	int m_nCounter1030;
	int m_nCounter1034;
	int m_nSearchState1038;
};

typedef char CMazeSizeCheck[sizeof(CMaze) == 0x103c ? 1 : -1];

#endif
