#include "GAME.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/CORE/VSMEM.H"

extern int g_nLevelFrameClockTimeMs;
extern void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void *pObject);

// ABI-only translation unit: moving these distinct binary entry points changes
// linker layout and breaks the reconstructed vtable addresses.

// FUNCTION: LEMBALL 0x00401091
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStreamThunk(void *pObject)
{
    ReadNetworkLevelChunkDeltaStream(pObject);
}

// FUNCTION: LEMBALL 0x004019fb
void *LEMBALL_FASTCALL DeleteNetworkLevelChunkDeltaStreamThunk(
    void *pObject, int nUnused, unsigned char fDelete)
{
    (void)nUnused;
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401ae6
void LEMBALL_FASTCALL WriteNetworkLevelChunkDeltaStreamThunk(void *pObject)
{
    VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)pObject;
    char *pOwner = *(char **)((char *)pObject + 0x2c);

    pStream->WriteEffStreamU16BE(0x2d);
    pStream->WriteEffStreamU32BE((unsigned int)g_nLevelFrameClockTimeMs);
    ((VsNetEffStreamBase *)(pOwner + 0x1c))->SaveEffStreamToMemoryRange(
        *(int *)((char *)pObject + 0x1c), 0);
    *(int *)((char *)pObject + 0x1c) +=
        *(int *)(pOwner + 0x38) - *(int *)(pOwner + 0x24);
    pStream->WriteEffStreamU16BE(0x2f);
}
