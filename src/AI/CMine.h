#ifndef LEMBALL_CMINE_H
#define LEMBALL_CMINE_H

class CMine {
public:
	CMine(void);

private:
	unsigned char m_abReserved00[0x150];
};

typedef char CMineSizeCheck[sizeof(CMine) == 0x150 ? 1 : -1];

#endif
