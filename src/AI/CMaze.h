#ifndef LEMBALL_CMAZE_H
#define LEMBALL_CMAZE_H

struct CMap;

class CMaze {
public:
	CMaze(CMap* pMap);
	~CMaze(void);
	void ReInitialise(void);

private:
	CMap* m_pMap00;
	unsigned short** m_ppRows04;
	int m_nReserved08;
	unsigned char m_abFrontiers0C[0x1000];
	unsigned char m_nFrontier100C;
	unsigned char m_abReserved100D[3];
	int m_cColumns1010;
	int m_cRows1014;
	unsigned char m_abReserved1018[0x24];
};

typedef char CMazeSizeCheck[sizeof(CMaze) == 0x103c ? 1 : -1];

#endif
