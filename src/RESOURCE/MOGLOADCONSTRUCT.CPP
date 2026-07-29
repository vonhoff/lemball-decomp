#include "ENGINE/CORE/WIN32.H"

extern void* g_MOGLOAD_CachedResourceObjectBaseVtable[];
extern void* g_MOGLOAD_StringResourceVtable[];
extern void LEMBALL_FASTCALL ResetTypedResourceObjectState(void* pObject);

// FUNCTION: LEMBALL 0x0045eaa0
void* LEMBALL_FASTCALL ConstructGrtsResourceBase(void* pObject)
{
	*(void* volatile*) pObject = g_MOGLOAD_CachedResourceObjectBaseVtable;
	*(void* volatile*) pObject = g_MOGLOAD_StringResourceVtable;
	*(int*) ((char*) pObject + 0x18) = 0;
	ResetTypedResourceObjectState(pObject);
	return pObject;
}
