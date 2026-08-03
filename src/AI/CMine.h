#ifndef LEMBALL_CMINE_H
#define LEMBALL_CMINE_H

class CGameObject;
struct AICOORD;

class CMine {
public:
	CMine(void);
	void Restart(void);
	void Initialise(void);
	void Trigger(int nDelay);
	void Set(AICOORD position);
	void StepOn(CGameObject* pObject);
	void OnGround(void);

private:
	unsigned char m_abReserved00[0x150];
};

typedef char CMineSizeCheck[sizeof(CMine) == 0x150 ? 1 : -1];

#endif
