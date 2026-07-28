#include "ENGINE/CORE/VSINIT.H"

struct ProjectilePoolOwnerView {
	char m_abReserved00[0x30];
	char* volatile m_pProjectilePool30;
	char m_abReserved34[0xa8];
	volatile int m_iProjectilePoolSearchStartDC;

	void* GetFreeProjectilePoolObject(void);
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation in LINKSCF.CPP.

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
