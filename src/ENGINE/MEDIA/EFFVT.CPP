#include "ENGINE/CORE/COMMON.H"

extern void* LEMBALL_FASTCALL DeleteNetworkLobbyVsnetRuntime(void* pObject, int nUnused, unsigned char fDelete);
extern int LEMBALL_FASTCALL HandleNetworkLobbyVsnetRuntimeEvent(void* pObject, int nUnused, unsigned short* pEvent);

// ABI-only translation unit for the distinct vtable thunk entry points in the
// network-lobby runtime table at 0x004985f8.

// FUNCTION: LEMBALL 0x004022b1
void* LEMBALL_FASTCALL DeleteNetworkLobbyVsnetRuntimeThunk(void* pObject, int nUnused, unsigned char fDelete)
{
	return DeleteNetworkLobbyVsnetRuntime(pObject, nUnused, fDelete);
}

// FUNCTION: LEMBALL 0x00402fb8
int LEMBALL_FASTCALL HandleNetworkLobbyVsnetRuntimeEventThunk(void* pObject, int nUnused, unsigned short* pEvent)
{
	return HandleNetworkLobbyVsnetRuntimeEvent(pObject, nUnused, pEvent);
}
