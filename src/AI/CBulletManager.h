#ifndef LEMBALL_CBULLETMANAGER_H
#define LEMBALL_CBULLETMANAGER_H

#include "AI/AICoord.h"

// Shared CBulletManager view modeled on the Macintosh blueprint class `CBulletManager`.
// Consolidates the formerly-duplicate local structs (ProjectilePool / CBulletManagerOwnerView)
// into a single definition so both the pool-iteration (Reset/GetFreeProjectilePoolObject) and
// the active-bullet list (GetFirst/GetNext/Process/RemoveObject) live on one class.

struct LevelShortRect;
struct LevelProjectilePosition;

struct CBulletManager {
	char m_abReserved00[0x30];
	char* volatile m_pProjectilePool30;      // 0x30 : storage pool of CBullet objects (stride 0x1a4)
	void* m_apObjects[20];                  // 0x34..0x83 : active bullet list
	char m_abReserved84[0x50];
	int m_nCountD4;
	int m_nIndexD8;
	volatile int m_iProjectilePoolSearchStartDC;   // 0xdc

	void RemoveObject(void* pObject);
	int FindProjectilePositionInRect(const LevelShortRect* pRect, LevelProjectilePosition* pPosition);
	void* GetFirstBullet(void);
	void* GetNextBullet(void);
	void Process(void);
	void* GetFreeProjectilePoolObject(void);
	int RequestBullet(unsigned short nSlotId, int nBulletType, int nOwner, int nDirection,
		AICOORD source, AICOORD target);
	void ResetProjectilePool(void);
	int EmitProjectileRenderEntries(void* pRenderEntry);
};

#endif
