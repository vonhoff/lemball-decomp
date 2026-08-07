#ifndef LEMBALL_CICE_H
#define LEMBALL_CICE_H

struct AICOORD;
struct tCoord3d;
class CGameObject;
class CPlayerLemming;

class CIce {
public:
	void Set(unsigned short nSlot,
			 const tCoord3d& start,
			 const tCoord3d& end,
			 int nMoveX,
			 int nMoveY,
			 unsigned char fActive);
	int StepOn(const AICOORD& position, CGameObject* pObject);
	void Leave(CPlayerLemming* pLemming);
	void Switch(void);

private:
	unsigned char m_abReserved00[0x60];

public:
	void* m_pOwnerManager60; // 0x60

private:
	unsigned char m_abReserved64[0x124];
};

typedef char CIceSizeMustMatchArrayStride[sizeof(CIce) == 0x188 ? 1 : -1];

#endif
