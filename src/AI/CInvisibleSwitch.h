#ifndef LEMBALL_CINVISIBLESWITCH_H
#define LEMBALL_CINVISIBLESWITCH_H

#include "AI/AICoord.h"

struct tCoord3d {
	short x;
	short y;
	short z;
};

class CGameObject;

class CInvisibleSwitch {
public:
	CInvisibleSwitch(void);
	void Restart(void);
	void Initialise(void);
	~CInvisibleSwitch(void);
	void Set(const tCoord3d& begin, const tCoord3d& end);
	void VerifyObjects(void);
	void AddObject(CGameObject* pObject);
	void StepOn(const AICOORD& position, CGameObject* pObject);
	void DoActivate(void);
	int Process(void);
	void Load(unsigned char*& pData);

private:
	unsigned char m_abReserved[0x9c];
	AICOORD m_WorldPosition9C;            // 0x9c
	unsigned char m_abReservedA8[0x10];
	int m_nStateB8;                        // 0xb8
	unsigned char m_abReservedBC[0x58];
	int m_nPendingState114;                // 0x114
	unsigned char m_abReserved118[0x10];
	int m_nLastState128;                   // 0x128
	int m_anRuntimeState12C[3];            // 0x12c..0x137
	tCoord3d m_RangeStart138;              // 0x138 (x,y,z)
	tCoord3d m_RangeEnd13E;                // 0x13e (x,y,z)
	int m_nTriggered144;                   // 0x144
	unsigned char m_abReserved148[0x170];
};

typedef char CInvisibleSwitchSizeCheck[sizeof(CInvisibleSwitch) == 0x2b8 ? 1 : -1];

#endif
