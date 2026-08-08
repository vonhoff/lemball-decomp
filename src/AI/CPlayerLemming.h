#ifndef LEMBALL_CPLAYERLEMMING_H
#define LEMBALL_CPLAYERLEMMING_H

#include "AI/CGameObject.h"

class CPlayerLemmingGroup;

class CPlayerLemming : public CGameObject {
public:
	char m_abReserved124[0x58];
	int m_fGroupLeader17C;
	int m_fGrouped180;
	char m_abReserved184[4];
	int m_fOnIce188;                  // 0x188
	void* m_pIce18C;                  // 0x18C
	int m_nBoredTime190;              // 0x190
	char m_abReserved194[0x20];
	int m_nTargetWorldX1B4;           // 0x1B4
	int m_nTargetWorldY1B8;           // 0x1B8
	char m_abReserved1BC[4];
	int m_anObjectTypes1C0[12];       // 0x1C0
	int m_anObjectValues1F0[12];      // 0x1F0
	int m_nObjectCount220;            // 0x220
	CPlayerLemmingGroup* m_pGroup224;
	unsigned short m_nLatchObjectType228;  // 0x228
	char m_abReserved22A[2];

	void SetGroup(CPlayerLemmingGroup* pGroup);
	CPlayerLemmingGroup* GetGroup(void);
	void SetGroupLeader(int fLeader);
	void SetGroup(int fGrouped);
	void SetBored(int nBaseTime);
	int RandomAction(void);
	void Die(void);
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
#ifdef LEMBALL_CPLAYERLEMMING_START_STANDING
	void StartStanding(void);
	void RequestBalloon(void);
	void Fire(void);
#endif
};

typedef char CPlayerLemmingSizeCheck[sizeof(CPlayerLemming) == 0x22c ? 1 : -1];

#endif
