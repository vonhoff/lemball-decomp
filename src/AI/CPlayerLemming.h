#ifndef LEMBALL_CPLAYERLEMMING_H
#define LEMBALL_CPLAYERLEMMING_H

#include "AI/CGameObject.h"

class CPlayerLemmingGroup;

class CPlayerLemming : public CGameObject {
public:
	char m_abReserved124[0x58];
	int m_fGroupLeader17C;
	int m_fGrouped180;
	char m_abReserved184[0xa0];
	CPlayerLemmingGroup* m_pGroup224;
	char m_abReserved228[0x4];

	void SetGroup(CPlayerLemmingGroup* pGroup);
	CPlayerLemmingGroup* GetGroup(void);
	void SetGroupLeader(int fLeader);
	void SetGroup(int fGrouped);
	void SetBored(int nBaseTime);
	int RandomAction(void);
	void HitMine(void);
	int HasObject(int nObjectType);
	int AddObject(int nObjectType, int nObject);
	void OnConveyor(int nOnConveyor, void* pIce, int nDetach);
	void Action(int nAction);
	void ExternalControlEnd(void);
	int FacingTarget(void);
	int FacingCursor(void);
	void TurnToFaceTarget(void);
	void TurnToFaceCursor(void);
	void GetData(void);
	void AddData(void);
};

typedef char CPlayerLemmingSizeCheck[sizeof(CPlayerLemming) == 0x22c ? 1 : -1];

#endif
