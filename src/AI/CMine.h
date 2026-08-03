#ifndef LEMBALL_CMINE_H
#define LEMBALL_CMINE_H

class CGameObject;

class CMine {
public:
	CMine(void);
	void Trigger(int nDelay);
	void StepOn(CGameObject* pObject);

private:
	unsigned char m_abReserved00[0x150];
};

typedef char CMineSizeCheck[sizeof(CMine) == 0x150 ? 1 : -1];

#endif
