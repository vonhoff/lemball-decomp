#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSGDI.H"
#include <string.h>

struct RenderDispatchClientObject {
    void **m_pVtable;
};

int LEMBALL_FASTCALL DispatchRenderQueueEntryToClients(
    void *pDispatchQueue, int nUnused, void *pEntryWords);
int DrainRenderDispatchQueueEntries(
    void *pDispatchQueue, unsigned int cEntries);
int CopyRenderDispatchQueueEntryAtIndex(
    void *pDispatchQueue, unsigned int *paEntryWords, int nIndex);
int RemoveRenderDispatchQueueEntryAtIndex(
    void *pDispatchQueue, unsigned int nIndex);
extern void *g_pSingletonAnimWindowOwner;
extern "C" {
__declspec(dllimport) void *WINAPI SelectObject(void *hDc, void *hObject);
__declspec(dllimport) BOOL WINAPI DeleteObject(void *hObject);
__declspec(dllimport) BOOL WINAPI DeleteDC(void *hDc);
__declspec(dllimport) HGLOBAL WINAPI GlobalHandle(const void *pMemory);
__declspec(dllimport) int WINAPI MultiByteToWideChar(
    unsigned int nCodePage, DWORD dwFlags, const char *pszSource,
    int cchSource, unsigned short *pwszDestination, int cchDestination);
__declspec(dllimport) int WINAPI SetStretchBltMode(void *hDc, int nMode);
}
extern unsigned int g_adwGlobalResourceGeometryPaletteTable[256];

struct VsWingDrawOpenRequest {
    int m_nReserved00;
    unsigned int m_dwType04;
    int m_anReserved08[2];
    int m_nOpenMode10;
    int m_nResult14;
};

struct VsWingWindowOwnerContextView {
    unsigned char m_abReserved00[0x0c];
    void *m_pWindowState0C;
};

struct VsWingAnimWindowOwnerView {
    unsigned char m_abReserved00[0x4c];
    VsWingWindowOwnerContextView *m_pWindowOwnerContext4C;
};

struct VsWingDrawHandlerState {
    void *m_pReserved00;
    void *m_hDrawDC04;
    int m_xDestination08;
    int m_yDestination0C;
    int m_cxDestination10;
    int m_cyDestination14;
    int m_xSource18;
    int m_ySource1C;
    int m_cxSource20;
    int m_cySource24;
    int m_nReserved28;
    void *m_pWindowState2C;
    void *m_hMemoryDC30;
    void *m_hBitmap34;
    void *m_hPreviousBitmap38;
    void *m_hPalette3C;
    void *m_hPreviousPalette40;
    unsigned char m_abReserved44[0x42c];
    void *m_pAnimWindowOwner470;
};

struct VsWingDrawCodecInfo {
    DWORD m_cbSize00;
    DWORD m_dwType04;
    DWORD m_dwHandler08;
    DWORD m_dwFlags0C;
    DWORD m_dwVersion10;
    DWORD m_dwIcmVersion14;
    unsigned char m_abReserved18[0x20];
    unsigned short m_awDescription38[0x100];
};

struct VsWingBitmapInfoHeader {
    DWORD m_cbSize00;
    int m_cx04;
    int m_cy08;
    unsigned short m_cPlanes0C;
    unsigned short m_cBitsPerPixel0E;
    DWORD m_dwCompression10;
    DWORD m_cbImage14;
    int m_nXPelsPerMeter18;
    int m_nYPelsPerMeter1C;
    DWORD m_cColorsUsed20;
    DWORD m_cImportantColors24;
};

struct VsWingSuggestFormatRequest {
    VsWingBitmapInfoHeader *m_pSource00;
    VsWingBitmapInfoHeader *m_pDestination04;
};

struct VsWingDrawBeginRequest {
    DWORD m_dwFlags00;
    unsigned char m_abReserved04[0x0c];
    int m_xDestination10;
    int m_yDestination14;
    int m_cxDestination18;
    int m_cyDestination1C;
    VsWingBitmapInfoHeader *m_pSourceFormat20;
    int m_xSource24;
    int m_ySource28;
    int m_cxSource2C;
    int m_cySource30;
};

// FUNCTION: LEMBALL 0x004631a0
int LEMBALL_FASTCALL AppendRenderDispatchQueueEntry(
    void *pDispatchQueue, int, RenderDispatchQueueEntry *pEntry)

{
    RenderDispatchQueue *pQueue;
    int nResult;

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);
    pEntry->m_awords[1] = (unsigned int)pQueue->m_nReserved30;
    ++pQueue->m_nReserved38;
    ++pQueue->m_nReserved30;

    if (pQueue->m_cEntryCapacity == pQueue->m_cQueuedEntries) {
        ++pQueue->m_nReserved34;
        DrainRenderDispatchQueueEntries(pDispatchQueue, 1);
        nResult = ((int (LEMBALL_FASTCALL *)(void *, int,
                                             RenderDispatchQueueEntry *))
                       (*(void ***)pDispatchQueue)[2])(
            pDispatchQueue, 0, pEntry);
        LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
        return nResult;
    }

    ++pQueue->m_cQueuedEntries;
    *pQueue->m_pTail = *pEntry;
    ++pQueue->m_pTail;
    if (pQueue->m_pEntryBufferEnd <= pQueue->m_pTail) {
        pQueue->m_pTail = pQueue->m_pEntryBuffer;
    }
    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}
// FUNCTION: LEMBALL 0x00463230
int LEMBALL_FASTCALL DispatchRenderQueueEntryImmediately(
    void *pDispatchQueue, int, RenderDispatchQueueEntry *pEntry)
{
    RenderDispatchQueue *pQueue;
    int nResult;

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);
    pEntry->m_awords[1] = (unsigned int)pQueue->m_nReserved30;
    ++pQueue->m_nReserved3C;
    ++pQueue->m_nReserved30;
    nResult = DispatchRenderQueueEntryToClients(pDispatchQueue, 0, pEntry);
    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return nResult;
}

// FUNCTION: LEMBALL 0x00463570
int TakeRenderDispatchQueueEntry(void *pDispatchQueue, unsigned int *paEntryWords, unsigned int nIndex) {
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);
    CopyRenderDispatchQueueEntryAtIndex(pDispatchQueue, paEntryWords, (int)nIndex);
    RemoveRenderDispatchQueueEntryAtIndex(pDispatchQueue, nIndex);
    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}
// FUNCTION: LEMBALL 0x004635b0
int CopyRenderDispatchQueueEntryAtIndex(void *pDispatchQueue, unsigned int *paEntryWords, int nIndex) {
    RenderDispatchQueue *pQueue;
    RenderDispatchQueueEntry *pSource;
    int i;

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pSource = (RenderDispatchQueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pHead + nIndex * sizeof(RenderDispatchQueueEntry));
    if (pQueue->m_pEntryBufferEnd <= pSource) {
        pSource = (RenderDispatchQueueEntry *)(unsigned long)((((unsigned long)pSource -
                                                            (unsigned long)pQueue->m_pEntryBufferEnd) /
                                                           sizeof(RenderDispatchQueueEntry)) *
                                                              sizeof(RenderDispatchQueueEntry) +
                                                          (int)(unsigned long)pQueue->m_pEntryBuffer);
    }

    for (i = 0; i < 5; ++i) {
        paEntryWords[i] = pSource->m_awords[i];
    }

    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}
// FUNCTION: LEMBALL 0x004636e0
int RemoveRenderDispatchQueueEntryAtIndex(void *pDispatchQueue, unsigned int nIndex) {
    RenderDispatchQueue *pQueue;
    RenderDispatchQueueEntry *pEntry;
    RenderDispatchQueueEntry *pNextEntry;
    RenderDispatchQueueEntry *pQueueHead;
    unsigned int cQueuedEntries;
    int i;

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pQueueHead = pQueue->m_pHead;
    pEntry = pQueueHead + nIndex;
    if (pQueue->m_pEntryBufferEnd <= pEntry) {
        pEntry = (RenderDispatchQueueEntry *)(unsigned long)((((unsigned long)pEntry -
                                                           (unsigned long)pQueue->m_pEntryBufferEnd) /
                                                          sizeof(RenderDispatchQueueEntry)) *
                                                             sizeof(RenderDispatchQueueEntry) +
                                                         (int)(unsigned long)pQueue->m_pEntryBuffer);
    }

    cQueuedEntries = (unsigned int)pQueue->m_cQueuedEntries;
    if (cQueuedEntries == 1) {
        pQueue->m_cQueuedEntries = 0;
        pQueue->m_pHead = pQueue->m_pTail;
        LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
        return 1;
    }

    if (pEntry == pQueueHead) {
        pQueue->m_pHead = pQueueHead + 1;
        if (pQueue->m_pEntryBufferEnd <= pQueueHead + 1) {
            pQueue->m_pHead = pQueue->m_pEntryBuffer;
        }
        pQueue->m_cQueuedEntries = (int)cQueuedEntries - 1;
        LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
        return 1;
    }

    pNextEntry = pEntry + 1;
    if (pQueue->m_pEntryBufferEnd <= pNextEntry) {
        pNextEntry = pQueue->m_pEntryBuffer;
    }

    while (nIndex < cQueuedEntries) {
        RenderDispatchQueueEntry *pSource;
        RenderDispatchQueueEntry *pDestination;

        if (pQueue->m_pEntryBufferEnd <= pNextEntry) {
            pNextEntry = pQueue->m_pEntryBuffer;
        }

        pSource = pEntry;
        if (pQueue->m_pEntryBufferEnd <= pEntry) {
            pSource = pQueue->m_pEntryBuffer;
        }

        pEntry = pSource + 1;
        pDestination = pNextEntry;
        for (i = 0; i < 5; ++i) {
            pDestination->m_awords[i] = pSource->m_awords[i];
        }

        pNextEntry += 1;
        ++nIndex;
        if (nIndex >= (unsigned int)pQueue->m_cQueuedEntries) {
            break;
        }
    }

    pQueue->m_pTail = (RenderDispatchQueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pTail -
                                                              sizeof(RenderDispatchQueueEntry));
    if (pQueue->m_pTail <= pQueue->m_pEntryBuffer) {
        pQueue->m_pTail = (RenderDispatchQueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pEntryBufferEnd -
                                                                  sizeof(RenderDispatchQueueEntry));
    }
    pQueue->m_cQueuedEntries = pQueue->m_cQueuedEntries - 1;

    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x00463810
int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries) {
    RenderDispatchQueue *pQueue;
    unsigned int cQueuedEntries;
    unsigned int cDrained;
    unsigned int auEntryWords[5];

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    cDrained = 0;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);
    cQueuedEntries = (unsigned int)pQueue->m_cQueuedEntries;

    if (cEntries != 0) {
        do {
            if (cQueuedEntries <= cDrained) {
                break;
            }
            if (TakeRenderDispatchQueueEntry(pDispatchQueue, auEntryWords, 0) == 0) {
                LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
                return 0;
            }
            if (DispatchRenderQueueEntryToClients(pDispatchQueue, 0, auEntryWords) == 0) {
                LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
                return 0;
            }
            ++cDrained;
        } while (cDrained < cEntries);
    }

    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}
// FUNCTION: LEMBALL 0x004638a0
int LEMBALL_FASTCALL DispatchRenderQueueEntryToClients(
    void *pDispatchQueue, int, void *pEntryWords)

{
    RenderDispatchQueue *pQueue;
    unsigned int i;
    RenderDispatchClientObject *pClientObject;
    RenderDispatchClientNode *pClientNode;

    pQueue = (RenderDispatchQueue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pClientNode = pQueue->m_pClientList;
    i = 0;
    if (pQueue->m_cClients != 0) {
        do {
            pClientObject = (RenderDispatchClientObject *)pClientNode->m_pClient;
            if (((int (LEMBALL_FASTCALL *)(void *, int, void *))
                     pClientObject->m_pVtable[2])(
                    pClientNode->m_pClient, 0, pEntryWords) == 1) {
                LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
                return 1;
            }
            pClientNode = pClientNode->m_pNext;
            ++i;
        } while (i < (unsigned int)pQueue->m_cClients);
    }

    pQueue->m_cEntriesDropped = pQueue->m_cEntriesDropped + 1;
    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x00479190
void *LEMBALL_STDCALL OpenVisualSciencesWingDrawHandler(
    VsWingDrawOpenRequest *pRequest)
{
    VsWingDrawHandlerState *pState;

    if (pRequest->m_dwType04 != 0x73646976) {
        return 0;
    }
    if (pRequest->m_nOpenMode10 == 1) {
        return 0;
    }
    if (pRequest->m_nOpenMode10 == 2) {
        return 0;
    }

    pState = (VsWingDrawHandlerState *)GlobalLock(GlobalAlloc(0x42, 0x474));
    if (pState == 0) {
        pRequest->m_nResult14 = -3;
        return 0;
    }

    pState->m_pAnimWindowOwner470 = g_pSingletonAnimWindowOwner;
    pState->m_pWindowState2C =
        ((VsWingAnimWindowOwnerView *)g_pSingletonAnimWindowOwner)
            ->m_pWindowOwnerContext4C->m_pWindowState0C;
    pRequest->m_nResult14 = 0;
    return pState;
}

// FUNCTION: LEMBALL 0x00479210
int LEMBALL_STDCALL CloseVisualSciencesWingDrawHandler(
    VsWingDrawHandlerState *pState)
{
    if (pState->m_hMemoryDC30 != 0) {
        if (pState->m_hPreviousBitmap38 != 0) {
            SelectObject(pState->m_hMemoryDC30, pState->m_hPreviousBitmap38);
        }
        if (pState->m_hPreviousPalette40 != 0) {
            SelectObject(pState->m_hMemoryDC30, pState->m_hPreviousPalette40);
        }
        DeleteDC(pState->m_hMemoryDC30);
    }
    if (pState->m_hBitmap34 != 0) {
        DeleteObject(pState->m_hBitmap34);
    }
    if (pState->m_hPalette3C != 0) {
        DeleteObject(pState->m_hPalette3C);
    }
    GlobalUnlock(GlobalHandle(pState));
    GlobalFree(GlobalHandle(pState));
    return 1;
}

// FUNCTION: LEMBALL 0x004792a0
DWORD LEMBALL_STDCALL FillVisualSciencesWingDrawInfo(
    VsWingDrawCodecInfo *pInfo, DWORD cbInfo)
{
    if (pInfo == 0) {
        return 0x238;
    }
    if (cbInfo < 0x238) {
        return 0;
    }

    pInfo->m_dwType04 = 0x63646976;
    pInfo->m_dwHandler08 = 0x4e415356;
    pInfo->m_cbSize00 = 0x238;
    pInfo->m_dwFlags0C = 0x10;
    pInfo->m_dwVersion10 = 0x10000;
    pInfo->m_dwIcmVersion14 = 0x104;
    MultiByteToWideChar(0, 0, "Visual Sciences WinG Draw Handler", -1,
                        pInfo->m_awDescription38, 0x100);
    MultiByteToWideChar(0, 0, "VS - WinG Anim", -1,
                        pInfo->m_awDescription38, 0x100);
    return 0x238;
}

// FUNCTION: LEMBALL 0x00479330
int LEMBALL_STDCALL ValidateVisualSciencesWing8BppFormat(
    void *, VsWingBitmapInfoHeader *pFormat)
{
    if (pFormat == 0) {
        return -2;
    }
    if (pFormat->m_dwCompression10 != 0) {
        return -2;
    }
    return (unsigned short)(pFormat->m_cBitsPerPixel0E - 8) < 1 ? 0 : -2;
}

// FUNCTION: LEMBALL 0x00479370
int LEMBALL_STDCALL SuggestVisualSciencesWingDrawFormat(
    void *, VsWingSuggestFormatRequest *pRequest, void *)
{
    VsWingBitmapInfoHeader *pSource;
    VsWingBitmapInfoHeader *pDestination;

    pDestination = pRequest->m_pDestination04;
    if (pDestination == 0) {
        return 0x428;
    }

    pSource = pRequest->m_pSource00;
    pDestination->m_cColorsUsed20 = pSource->m_cColorsUsed20;
    pDestination->m_cImportantColors24 = pSource->m_cImportantColors24;
    pDestination->m_cbSize00 = pSource->m_cbSize00;
    pDestination->m_cx04 = pSource->m_cx04;
    pDestination->m_cy08 = pSource->m_cy08;
    pDestination->m_cbImage14 = pSource->m_cx04 * pSource->m_cy08;
    pDestination->m_cPlanes0C = 1;
    pDestination->m_cBitsPerPixel0E = 8;
    pDestination->m_dwCompression10 = 0;
    pDestination->m_cbImage14 = 0;
    return pSource->m_cColorsUsed20 * 4 + 0x28;
}

// FUNCTION: LEMBALL 0x004793e0
int LEMBALL_STDCALL BeginVisualSciencesWingDraw(
    VsWingDrawHandlerState *pState, VsWingDrawBeginRequest *pRequest, void *)
{
    int nResult;

    nResult = ValidateVisualSciencesWing8BppFormat(
        pState, pRequest->m_pSourceFormat20);
    if (nResult == 0 && (pRequest->m_dwFlags00 & 1) == 0) {
        pState->m_xDestination08 = pRequest->m_xDestination10;
        pState->m_yDestination0C = pRequest->m_yDestination14;
        pState->m_cxDestination10 = pRequest->m_cxDestination18;
        pState->m_cyDestination14 = pRequest->m_cyDestination1C;
        pState->m_xSource18 = pRequest->m_xSource24;
        pState->m_ySource1C = pRequest->m_ySource28;
        pState->m_cxSource20 = pRequest->m_cxSource2C;
        pState->m_cySource24 = pRequest->m_cySource30;
        SetStretchBltMode(pState->m_hDrawDC04, 3);

        nResult = pRequest->m_pSourceFormat20->m_cColorsUsed20;
        nResult = nResult * 4 - 4;
        if (nResult > 0) {
            memcpy(g_adwGlobalResourceGeometryPaletteTable + 1,
                   (unsigned char *)pRequest->m_pSourceFormat20 + 0x2c,
                   nResult);
            PropagateGlobalPaletteToResourceGeometries(pState->m_pWindowState2C);
        }
        nResult = 0;
    }
    return nResult;
}
