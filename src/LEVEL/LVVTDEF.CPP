#include "ENGINE/CORE/COMMON.H"

// FUNCTION: LEMBALL 0x0040a9a0
int ReturnFalseSharedVtableCallbackA(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9b0
void NoopSharedVtableCallbackA(void)
{
}

// FUNCTION: LEMBALL 0x0040a9e0
void LEMBALL_STDCALL NoopSharedVtableCallbackRet4A(void* pUnused)
{
	(void) pUnused;
}

// FUNCTION: LEMBALL 0x0040aa20
void NoopSharedVtableCallbackB(void)
{
}

// FUNCTION: LEMBALL 0x0040aac0
void LEMBALL_STDCALL NoopSharedVtableCallbackRet4B(void* pUnused)
{
	(void) pUnused;
}

// FUNCTION: LEMBALL 0x0040aaf0
void LEMBALL_FASTCALL ClearSharedVtableRuntimeFlag(void* pObject)
{
	*(int*) ((char*) pObject + 0x30) = 0;
}
