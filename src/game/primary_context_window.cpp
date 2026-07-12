#include "game/game_app.h"
#include "engine/graphics_driver.h"
#include "engine/memory_arena.h"
#include "platform/startup_options.h"

extern void LEMBALL_FASTCALL EnsureHelperGroup0BackingBuffer(int nHelperGroup);
extern void LEMBALL_FASTCALL ReleaseHelperGroup0BackingBuffer(int nHelperGroup);

/* FUNCTION: LEMBALL 0x00431EB0 */
int __cdecl GetPrimaryContextMenuDefinition(unsigned int *pMenuId,
                                             const char **ppWindowTitle) {
    *pMenuId = 0x73;
    *ppWindowTitle = (const char *)(unsigned long)0x0049e718;
    return 1;
}

/* FUNCTION: LEMBALL 0x004654F0 */
unsigned int MapWindowOwnerFlagsToWin32Style(unsigned int uFlags) {
    unsigned int uStyle;

    uStyle = ((uFlags & 8) == 0) ? 0xcb0000 : 0;
    uStyle |= ((uFlags & 2) == 0) ? 0x7f400000 : 0;
    uStyle += 0xc00000;
    uStyle |= (uFlags & 0x400) << 8;
    uStyle |= (uFlags & 0x40) << 10;
    uStyle |= (uFlags & 0x20) << 15;
    uStyle |= (uFlags & 0x10) << 17;
    uStyle |= (uFlags & 0x80) << 10;
    uStyle |= (uFlags & 0x100) << 11;
    return uStyle;
}

/* FUNCTION: LEMBALL 0x00431730 */
unsigned int __stdcall ComputePrimaryContextWindowStyleFlags(void) {
    unsigned int uStyle;

    uStyle = 0x80001801;
    if (g_fCompactPrimaryContextLayout == 0) {
        if (GetSystemMetrics(0x3d) >= 0x281 &&
            GetSystemMetrics(0x3e) >= 0x1f5) {
            uStyle = 0x80001b83;
        }
    } else {
        uStyle = 0x80001b83;
    }

    if (g_fStartupEditLevelOverride != 0) {
        uStyle |= 0x404;
    }
    return uStyle;
}

/* FUNCTION: LEMBALL 0x00466540 */
void LEMBALL_FASTCALL EnsureHelperGroup0BackingBuffer(int nHelperGroup) {
    unsigned short *pSize;
    short aDimensions[2];
    unsigned short aRect[2];
    int nHelperTarget;

    pSize = (unsigned short *)(*(int *)(*(int *)(nHelperGroup + 4) + 4) +
                               nHelperGroup + 0x18);
    aRect[0] = pSize[0];
    aRect[1] = pSize[1];
    aDimensions[0] = 0;
    aDimensions[1] = 0;

    nHelperTarget = *(int *)(*(int *)(nHelperGroup + 4) + 4) + nHelperGroup;
    ((VSGDI_HelperSurface *)(unsigned long)(nHelperGroup + 8))
        ->ComputeBackingDimensions(aDimensions, (short *)aRect,
                                   *(int *)(nHelperTarget + 0x44));
    aRect[0] = (unsigned short)aDimensions[0];
    aRect[1] = (unsigned short)aDimensions[1];

    if ((unsigned int)*(unsigned short *)(nHelperGroup + 0x50) *
            (unsigned int)*(unsigned short *)(nHelperGroup + 0x52) <
        (unsigned int)(short)aRect[0] * (unsigned int)(short)aRect[1]) {
        ReleaseHelperGroup0BackingBuffer(nHelperGroup);
    }

    if ((short)*(short *)(nHelperTarget + 0x18) *
            (short)*(short *)(nHelperTarget + 0x1a) != 0) {
        if (*(int *)(nHelperGroup + 0x4c) == 0) {
            *(unsigned short *)(nHelperGroup + 0x50) = aRect[0];
            *(unsigned short *)(nHelperGroup + 0x52) = aRect[1];
            *(int *)(nHelperGroup + 0x4c) =
                (int)(unsigned long)AllocateVSMemBlock(
                    (unsigned int)aRect[0] * (unsigned int)aRect[1]);
        }
        if (*(int *)(nHelperGroup + 0x4c) == 0) {
            *(int *)(nHelperGroup + 0x48) = 0;
        }
        ((VSGDI_HelperSurface *)(unsigned long)(nHelperGroup + 8))
            ->ConfigureBackingStrideAndOrigin(
                *(int *)(nHelperGroup + 0x4c), (short)aRect[0]);
    }
}

struct GAME_HelperGroup0 {
    void SetBackingActive(int fActive);
};

/* FUNCTION: LEMBALL 0x00466630 */
void GAME_HelperGroup0::SetBackingActive(int fActive) {
    if (fActive == 0) {
        ReleaseHelperGroup0BackingBuffer((int)(unsigned long)this);
        *(int *)((char *)this + 0x48) = 0;
    } else {
        EnsureHelperGroup0BackingBuffer((int)(unsigned long)this);
        *(int *)((char *)this + 0x48) = 1;
    }
}

/* FUNCTION: LEMBALL 0x00431780 */
void LEMBALL_FASTCALL PrepareLevelScreenRootHelperForRuntime(
    GAME_PrimaryContext *pPrimaryContext) {
    void *pHelperGroup;

    pPrimaryContext->SetWindowOwnerScaleFactor(1);
#if !defined(LEMBALL_PRIMARY_WINDOW_INIT_SKIP_PALETTE)
    pPrimaryContext->ApplyWindowOwnerPaletteResource(0x2e);
#endif

    pHelperGroup = *(void **)((char *)pPrimaryContext + 0x4c);
    pHelperGroup = (char *)*(void **)((char *)pHelperGroup + 0x0c) + 0x98;
    ((GAME_HelperGroup0 *)pHelperGroup)->SetBackingActive(1);
    *(int *)((char *)pPrimaryContext + 0xd4) = 0;
}

/* FUNCTION: LEMBALL 0x00432530 */
void GAME_PrimaryContext::MarkNestedContextDirtyIfField20(void) {
    int nNestedContext;
    int nDirtyTable;

    if (*(int *)((char *)this + 0x20) == 0) {
        return;
    }

    nNestedContext = *(int *)(*(int *)((char *)this + 0x4c) + 0x0c);
    nDirtyTable = *(int *)(nNestedContext + 0x40);
    *(int *)(*(int *)(nDirtyTable + 4) + nNestedContext + 0x78) = 1;
}
