#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"
#include "Platform/Windows/Mixed/Engine/MEDIA/VSSTRM.H"
#include "AI/CBulletManager.h"
#include "AI/CBullet.h"

void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

typedef void(LEMBALL_FASTCALL* ResetProjectileObjectProc)(void* pObject);

void LEMBALL_FASTCALL DestroyProjectileObject(void* pObject);

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00417fa0
int CBulletManager::RequestBullet(
	unsigned short nSlotId, int nBulletType, int nOwner, int nDirection,
	AICOORD source, AICOORD target)
{
	int nIndex = m_nCountD4;
	if (nIndex < 40) {
		m_apObjects[nIndex] = GetFreeProjectilePoolObject();
		CBullet* pBullet = (CBullet*) m_apObjects[m_nCountD4];
		if (pBullet != 0) {
			pBullet->Set(nSlotId, nBulletType, nOwner, nDirection, source, target);
			((CBullet*) m_apObjects[m_nCountD4])->FireBullet();
			++m_nCountD4;
		}
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00417e80
void CBulletManager::ResetProjectilePool(void)
{
	void** ppActiveProjectile;
	int cbProjectile;
	void* pProjectile;
	ResetProjectileObjectProc pReset;

	cbProjectile = 0;
	ppActiveProjectile = m_apObjects;
	m_nIndexD8 = 0;
	m_nCountD4 = 0;
	do {
		*ppActiveProjectile = NULL;
		ppActiveProjectile++;
		pProjectile = m_pProjectilePool30 + cbProjectile;
		cbProjectile += 0x1a4;
		pReset = (ResetProjectileObjectProc) (*(void***) pProjectile)[65];
		pReset(pProjectile);
	} while (cbProjectile < 0x41a0);
}

// Vtable slot 0x494008 pins the __fastcall free form; forward to the real member.
void LEMBALL_FASTCALL ResetProjectilePool(void* pPool)
{
	((CBulletManager*) pPool)->ResetProjectilePool();
}

// FUNCTION: LEMBALL 0x00417ee0
void* CBulletManager::GetFreeProjectilePoolObject(void)
{
	int iProjectile;

	iProjectile = 0;
	while (1) {
		if (iProjectile >= 20) {
			return 0;
		}
		if (*(int*) (m_pProjectilePool30 + (m_iProjectilePoolSearchStartDC + iProjectile) * 0x1a4 + 0x164) == 0) {
			break;
		}
		++iProjectile;
	}
	return m_pProjectilePool30 + (m_iProjectilePoolSearchStartDC + iProjectile) * 0x1a4;
}

// FUNCTION: LEMBALL 0x00418330
void* LEMBALL_FASTCALL DeleteProjectileObject(void* pObject, void* pUnused, unsigned int fDelete)
{
	char* pAllocation;
	char* pCursor;
	CNetworkMessage* pStream;

	(void) pUnused;
	if ((fDelete & 2) != 0) {
		pAllocation = (char*) pObject - sizeof(int);
		fDelete = *(unsigned int*) pAllocation;
		pCursor = (char*) pObject + fDelete * 0x1a4;
		while ((int) --fDelete >= 0) {
			pCursor -= 0x1a4;
			DestroyProjectileObject(pCursor);
		}
		FreeVSMemBlock(pAllocation);
		return pObject;
	}

	pStream = pObject != NULL ? (CNetworkMessage*) ((char*) pObject + 0x138) : NULL;
	pStream->CNetworkMessage::~CNetworkMessage();
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x004183d0
void LEMBALL_FASTCALL DestroyProjectileObject(void* pObject)
{
	CNetworkMessage* pStream;

	pStream = pObject != NULL ? (CNetworkMessage*) ((char*) pObject + 0x138) : NULL;
	pStream->CNetworkMessage::~CNetworkMessage();
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}
