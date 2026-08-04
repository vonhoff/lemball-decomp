#ifndef LEMBALL_CICE_H
#define LEMBALL_CICE_H

class CIce {
public:
	void Switch(void);

private:
	unsigned char m_abReserved00[0x188];
};

typedef char CIceSizeMustMatchArrayStride[sizeof(CIce) == 0x188 ? 1 : -1];

#endif
