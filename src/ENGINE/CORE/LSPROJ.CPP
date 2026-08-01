#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/MEDIA/VSSTRM.H"

void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

struct ProjectilePoolOwnerView {
	char m_abReserved00[0x30];
	char* volatile m_pProjectilePool30;
	void* m_apActiveProjectiles34[20];
	char m_abReserved84[0x50];
	int m_cActiveProjectilesD4;
	int m_iActiveProjectileD8;
	volatile int m_iProjectilePoolSearchStartDC;

	void* GetFreeProjectilePoolObject(void);
};

typedef void(LEMBALL_FASTCALL* ResetProjectileObjectProc)(void* pObject);

void LEMBALL_FASTCALL DestroyProjectileObject(void* pObject);

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation in LINKSCF.CPP.

// FUNCTION: LEMBALL 0x00417e80
void LEMBALL_FASTCALL ResetProjectilePool(void* pPool)
{
	void** ppActiveProjectile;
	int cbProjectile;
	void* pProjectile;
	ResetProjectileObjectProc pReset;

	cbProjectile = 0;
	ppActiveProjectile = ((ProjectilePoolOwnerView*) pPool)->m_apActiveProjectiles34;
	((ProjectilePoolOwnerView*) pPool)->m_iActiveProjectileD8 = 0;
	((ProjectilePoolOwnerView*) pPool)->m_cActiveProjectilesD4 = 0;
	do {
		*ppActiveProjectile = NULL;
		ppActiveProjectile++;
		pProjectile = ((ProjectilePoolOwnerView*) pPool)->m_pProjectilePool30 + cbProjectile;
		cbProjectile += 0x1a4;
		pReset = (ResetProjectileObjectProc) (*(void***) pProjectile)[65];
		pReset(pProjectile);
	} while (cbProjectile < 0x41a0);
}

// FUNCTION: LEMBALL 0x00417ee0
void* ProjectilePoolOwnerView::GetFreeProjectilePoolObject(void)
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
	VsNetEffStreamBase* pStream;

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

	pStream = pObject != NULL ? (VsNetEffStreamBase*) ((char*) pObject + 0x138) : NULL;
	pStream->VsNetEffStreamBase::~VsNetEffStreamBase();
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x004183d0
void LEMBALL_FASTCALL DestroyProjectileObject(void* pObject)
{
	VsNetEffStreamBase* pStream;

	pStream = pObject != NULL ? (VsNetEffStreamBase*) ((char*) pObject + 0x138) : NULL;
	pStream->VsNetEffStreamBase::~VsNetEffStreamBase();
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}
