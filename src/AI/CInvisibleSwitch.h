#ifndef LEMBALL_CINVISIBLESWITCH_H
#define LEMBALL_CINVISIBLESWITCH_H

struct AICOORD {
	int x;
	int y;
	int z;
};

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
};

#endif
