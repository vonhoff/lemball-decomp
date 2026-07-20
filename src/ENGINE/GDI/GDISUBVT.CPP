#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSGDI.H"
#include <string.h>

static __inline void MergeCompactHelperRect(short *pTarget, const short *pEntry) {
    short nRight;
    short nBottom;

    if (pEntry[2] < pTarget[2]) {
        pTarget[0] = (short)(pTarget[0] + pTarget[2] - pEntry[2]);
        pTarget[2] = pEntry[2];
    }
    nRight = (short)(pEntry[2] + pEntry[0]);
    if ((short)(pTarget[2] + pTarget[0]) < nRight) {
        pTarget[0] = (short)(nRight - pTarget[2]);
    }
    if (pEntry[3] < pTarget[3]) {
        pTarget[1] = (short)(pTarget[1] + pTarget[3] - pEntry[3]);
        pTarget[3] = pEntry[3];
    }
    nBottom = (short)(pEntry[3] + pEntry[1]);
    if ((short)(pTarget[3] + pTarget[1]) < nBottom) {
        pTarget[1] = (short)(nBottom - pTarget[3]);
    }
}

// These small entries are compiler-generated adjustor and interface thunks.

// FUNCTION: LEMBALL 0x0046db40
void LEMBALL_FASTCALL CompactHelperSubobjectSlot0(void *) {}

// FUNCTION: LEMBALL 0x0046db50
void LEMBALL_FASTCALL CompactHelperSubobjectSlot1(
    void *pObject, int, void *pEntryObject)
{
    char *pCompactTarget;
    char *pLinkedTarget;
    int nVariableOffset;
    int nLinkedOffset;
    short *pEntry;
    short *pLocalRect;
    short *pLinkedRect;

    pCompactTarget = (char *)pObject - *(int *)((char *)pObject - 4);
    pEntry = (short *)pEntryObject;
    nVariableOffset = *(int *)(*(int *)(pCompactTarget - 0x51c) + 4);
    pLocalRect = (short *)(pCompactTarget + nVariableOffset - 0x4f8);

    if ((*(unsigned char *)((char *)pEntryObject + 0x0d) & 0x10) == 0) {
        pLocalRect[0] = pEntry[2];
        pLocalRect[1] = pEntry[3];
        pLocalRect[2] = pEntry[4];
        pLocalRect[3] = pEntry[5];
    } else if (pEntry[2] * pEntry[3] != 0) {
        MergeCompactHelperRect(pLocalRect, pEntry + 2);
    }

    pLinkedTarget = *(char **)(pCompactTarget + nVariableOffset - 0x4fc);
    if (pLinkedTarget != (char *)g_pResourceGeometryHelperTarget &&
        (*(unsigned char *)((char *)pEntryObject + 0x0e) & 1) == 0) {
        nLinkedOffset = *(int *)(*(int *)(pLinkedTarget + 0x40) + 4);
        pLinkedRect = (short *)(pLinkedTarget + nLinkedOffset + 0x64);
        MergeCompactHelperRect(pLocalRect, pLinkedRect);
        if (pLocalRect[0] < 1 || pLocalRect[1] < 1) {
            pLocalRect[0] = 0;
            pLocalRect[1] = 0;
            pLocalRect[2] = 0;
            pLocalRect[3] = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x0046db60
void LEMBALL_FASTCALL CompactHelperSubobjectSlot2(void *) {}

// FUNCTION: LEMBALL 0x0046db70
void LEMBALL_FASTCALL CompactHelperSubobjectSlot3(void *) {}

// FUNCTION: LEMBALL 0x0046db80
void LEMBALL_FASTCALL CompactHelperSubobjectSlot4(void *) {}

// FUNCTION: LEMBALL 0x0046db90
void LEMBALL_FASTCALL CompactHelperSubobjectSlot5(void *) {}

// FUNCTION: LEMBALL 0x0046dc00
void LEMBALL_FASTCALL CompactHelperSubobjectSlot9(void *pObject, int, void *pEntryObject, void *pFrameObject) {
    typedef unsigned char *(LEMBALL_FASTCALL *GetDataProc)(void *, int);
    typedef void (LEMBALL_FASTCALL *DirtyRectProc)(
        void *, int, VsGdiRect *);
    unsigned char *pEntry;
    unsigned char *pFrame;
    unsigned char *pSource;
    unsigned char **ppRows;
    VsGdiHelperSurface *pSurface;
    VsGdiRect Rect;
    VsGdiRect ClippedRect;
    short *pRect;
    short *pClipped;
    int xDestination;
    int yDestination;
    int cxFrame;
    int cyFrame;
    int iRow;
    int xSource;

    pEntry = (unsigned char *)pEntryObject;
    pFrame = (unsigned char *)pFrameObject;
    pSurface = (VsGdiHelperSurface *)((char *)pObject - 0x55c);
    cxFrame = *(short *)(pFrame + 0x4c);
    cyFrame = *(short *)(pFrame + 0x4e);
    if (cxFrame <= 0 || cyFrame <= 0) {
        return;
    }

    xDestination = *(short *)(pEntry + 4);
    yDestination = *(short *)(pEntry + 6);
    if ((*(unsigned int *)(pEntry + 0x0c) & 0x400) == 0) {
        xDestination += *(short *)(pFrame + 0x48);
        yDestination += *(short *)(pFrame + 0x4a);
    }
    if (cxFrame > 0xff) {
        cxFrame = 0xff;
    }
    if (cyFrame > 0xff) {
        cyFrame = 0xff;
    }

    pRect = (short *)&Rect;
    pRect[0] = (short)cxFrame;
    pRect[1] = (short)cyFrame;
    pRect[2] = (short)xDestination;
    pRect[3] = (short)yDestination;
    memset(&ClippedRect, 0, sizeof(ClippedRect));
    pClipped = (short *)&ClippedRect;
    if (pSurface->ClipRenderRectToSurfaceBounds(&Rect, &ClippedRect) &&
        (pClipped[0] < 1 || pClipped[1] < 1)) {
        return;
    }
    ((DirtyRectProc)(*(void ***)pSurface)[1])(pSurface, 0, &Rect);

    pSource = ((GetDataProc)(*(void ***)pFrame)[10])(pFrame, 0);
    ppRows = *(unsigned char ***)((char *)pSurface + 4);
    for (iRow = 0; iRow < cyFrame; ++iRow) {
        unsigned char *pDestination;
        unsigned char nCommand;
        int xOutput;
        int yOutput;

        xSource = 0;
        yOutput = yDestination + iRow;
        pDestination = yOutput >= Rect.m_cy &&
                       yOutput < Rect.m_cy + Rect.m_y
            ? ppRows[yOutput]
            : 0;
        do {
            nCommand = *pSource++;
            if (nCommand < 0x80) {
                xSource += nCommand;
            } else if (nCommand > 0x80) {
                int cbLiteral;
                int i;

                cbLiteral = nCommand & 0x7f;
                for (i = 0; i < cbLiteral; ++i) {
                    xOutput = xDestination + xSource + i;
                    if (pDestination != 0 &&
                        xOutput >= Rect.m_cx &&
                        xOutput < Rect.m_cx + Rect.m_x) {
                        pDestination[xOutput] = pSource[i];
                    }
                }
                pSource += cbLiteral;
                xSource += cbLiteral;
            }
        } while (nCommand != 0x80);
    }
}

// FUNCTION: LEMBALL 0x0046dc20
void LEMBALL_FASTCALL CompactHelperSubobjectSlot11(void *) {}

// FUNCTION: LEMBALL 0x0046dcb0
void LEMBALL_FASTCALL CompactHelperSubobjectSlot17(void *) {}

// FUNCTION: LEMBALL 0x0046dcc0
void LEMBALL_FASTCALL CompactHelperSubobjectSlot18(void *) {}

// FUNCTION: LEMBALL 0x00473700
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot5(void *) {}

// FUNCTION: LEMBALL 0x00473710
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot6(void *) {}

// FUNCTION: LEMBALL 0x00473750
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot4(void *) {}

// FUNCTION: LEMBALL 0x00473770
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot8(void *) {}
