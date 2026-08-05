#ifndef LEMBALL_CICE_H
#define LEMBALL_CICE_H

class CIce {
public:
	void Switch(void);

private:
	unsigned char m_abReserved00[0x60];
public:
	void* m_pOwnerManager60;               // 0x60
private:
	unsigned char m_abReserved64[0x124];
};

typedef char CIceSizeMustMatchArrayStride[sizeof(CIce) == 0x188 ? 1 : -1];

#endif
