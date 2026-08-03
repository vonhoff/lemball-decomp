#ifndef LEMBALL_CLIFT_H
#define LEMBALL_CLIFT_H

class CLift {
public:
	CLift(void);
	~CLift(void);
	void CalculateCliff(void);

private:
	unsigned char m_abReserved00[0x190];
};

#endif
