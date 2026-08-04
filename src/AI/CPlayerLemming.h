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
};

typedef char CPlayerLemmingSizeCheck[sizeof(CPlayerLemming) == 0x22c ? 1 : -1];

#endif
