#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct MobileChunkObjectRuntimeView {
	unsigned char m_abReserved00[0x94];
	int m_nRuntime94;
	unsigned char m_abReserved98[0xa0];
	int m_anRuntime138[3];
};

// MACINTOSH: CTrampoline::Restart()
// FUNCTION: LEMBALL 0x0042a9b0
void LEMBALL_FASTCALL InitializeTramChunkObject(MobileChunkObjectRuntimeView* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	pObject->m_nRuntime94 = 0;
	pObject->m_anRuntime138[0] = 0;
	pObject->m_anRuntime138[1] = 0;
}

// MACINTOSH: CPaintGun::Restart()
// FUNCTION: LEMBALL 0x0042baf0
void LEMBALL_FASTCALL ActivatePgunChunkObject(MobileChunkObjectRuntimeView* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	pObject->m_nRuntime94 = 0;
	pObject->m_anRuntime138[0] = 0;
	pObject->m_anRuntime138[1] = 0;
	pObject->m_anRuntime138[2] = 0;
}
