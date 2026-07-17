#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"

// FUNCTION: LEMBALL 0x00401d2a
void *LEMBALL_FASTCALL DeleteStreamFormatTargetState(
    void *pState, int nUnused, unsigned char fDelete)
{
    (void)nUnused;
    ConstructStreamFormatState((VsInitStreamFormatTargetState *)pState);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pState);
    }
    return pState;
}
