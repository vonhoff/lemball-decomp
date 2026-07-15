#include "ENGINE/CORE/VSINIT.H"

struct RenderDispatchClientObject {
    void **m_pVtable;
};

int LEMBALL_FASTCALL DispatchRenderQueueEntryToClients(
    void *pDispatchQueue, int nUnused, void *pEntryWords);
int DrainRenderDispatchQueueEntries(
    void *pDispatchQueue, unsigned int cEntries);

// FUNCTION: LEMBALL 0x004631A0
int LEMBALL_FASTCALL AppendRenderDispatchQueueEntry(
    void *pDispatchQueue, int, RenderDispatchQueueEntry *pEntry) {
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
    void *pDispatchQueue, int, RenderDispatchQueueEntry *pEntry) {
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

// FUNCTION: LEMBALL 0x004635B0
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

// FUNCTION: LEMBALL 0x004636E0
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

// FUNCTION: LEMBALL 0x00463570
int TakeRenderDispatchQueueEntry(void *pDispatchQueue, unsigned int *paEntryWords, unsigned int nIndex) {
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);
    CopyRenderDispatchQueueEntryAtIndex(pDispatchQueue, paEntryWords, (int)nIndex);
    RemoveRenderDispatchQueueEntryAtIndex(pDispatchQueue, nIndex);
    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x004638A0
int LEMBALL_FASTCALL DispatchRenderQueueEntryToClients(
    void *pDispatchQueue, int, void *pEntryWords) {
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
