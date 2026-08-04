#ifndef LEMBALL_CHAND_H
#define LEMBALL_CHAND_H

class CHand {
public:
	CHand(void);
	void Initialise(void);
	void Restart(void);
	~CHand(void);

private:
	unsigned char m_abReserved00[0x144];
};

typedef char CHandSizeMustMatchArrayStride[sizeof(CHand) == 0x144 ? 1 : -1];

#endif
