#include "ENGINE/CORE/COMMON.H"

extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct ProjectileRuntimeStateView {
	unsigned char m_abReserved00[0xb8];
	int m_nStateB8;
	unsigned char m_abReservedBC[0xa8];
	int m_fActive164;
};

// FUNCTION: LEMBALL 0x0041a5a0
void LEMBALL_FASTCALL ResetProjectileObjectToInactiveState(ProjectileRuntimeStateView* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	pObject->m_fActive164 = 0;
	pObject->m_nStateB8 = 8;
}
