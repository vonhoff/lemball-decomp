#include "../engine/runtime_init.h"

struct RDISPATCH_QueueEntry {
    unsigned int m_awords[5];
};

struct RDISPATCH_ClientNode {
    void *m_pClient;
    int m_nOrder;
    RDISPATCH_ClientNode *m_pNext;
};

struct RDISPATCH_ClientObject {
    void **m_pVtable;
};

struct RDISPATCH_Queue {
    void *m_pVtable;
    int m_nReserved04;
    void *m_pLockVtable;
    char m_abCriticalSection[0x18];
    int m_cEntryCapacity;
    int m_cQueuedEntries;
    int m_cClients;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3C;
    int m_cEntriesDropped;
    RDISPATCH_QueueEntry *m_pEntryBuffer;
    RDISPATCH_QueueEntry *m_pEntryBufferEnd;
    RDISPATCH_QueueEntry *m_pHead;
    RDISPATCH_QueueEntry *m_pTail;
    RDISPATCH_ClientNode *m_pClientList;
};

// FUNCTION: LEMBALL 0x004635B0
int CopyRenderDispatchQueueEntryAtIndex(void *pDispatchQueue, unsigned int *paEntryWords, int nIndex) {
    RDISPATCH_Queue *pQueue;
    RDISPATCH_QueueEntry *pSource;
    int i;

    pQueue = (RDISPATCH_Queue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pSource = (RDISPATCH_QueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pHead + nIndex * sizeof(RDISPATCH_QueueEntry));
    if (pQueue->m_pEntryBufferEnd <= pSource) {
        pSource = (RDISPATCH_QueueEntry *)(unsigned long)((((unsigned long)pSource -
                                                            (unsigned long)pQueue->m_pEntryBufferEnd) /
                                                           sizeof(RDISPATCH_QueueEntry)) *
                                                              sizeof(RDISPATCH_QueueEntry) +
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
    RDISPATCH_Queue *pQueue;
    RDISPATCH_QueueEntry *pEntry;
    RDISPATCH_QueueEntry *pNextEntry;
    RDISPATCH_QueueEntry *pQueueHead;
    unsigned int cQueuedEntries;
    int i;

    pQueue = (RDISPATCH_Queue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pQueueHead = pQueue->m_pHead;
    pEntry = pQueueHead + nIndex;
    if (pQueue->m_pEntryBufferEnd <= pEntry) {
        pEntry = (RDISPATCH_QueueEntry *)(unsigned long)((((unsigned long)pEntry -
                                                           (unsigned long)pQueue->m_pEntryBufferEnd) /
                                                          sizeof(RDISPATCH_QueueEntry)) *
                                                             sizeof(RDISPATCH_QueueEntry) +
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
        RDISPATCH_QueueEntry *pSource;
        RDISPATCH_QueueEntry *pDestination;

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

    pQueue->m_pTail = (RDISPATCH_QueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pTail -
                                                              sizeof(RDISPATCH_QueueEntry));
    if (pQueue->m_pTail <= pQueue->m_pEntryBuffer) {
        pQueue->m_pTail = (RDISPATCH_QueueEntry *)(unsigned long)((int)(unsigned long)pQueue->m_pEntryBufferEnd -
                                                                  sizeof(RDISPATCH_QueueEntry));
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
int DispatchRenderQueueEntryToClients(void *pDispatchQueue, void *pEntryWords) {
    RDISPATCH_Queue *pQueue;
    unsigned int i;
    RDISPATCH_ClientObject *pClientObject;
    RDISPATCH_ClientNode *pClientNode;

    pQueue = (RDISPATCH_Queue *)pDispatchQueue;
    EnterObjectCriticalSection((char *)pDispatchQueue + 8);

    pClientNode = pQueue->m_pClientList;
    i = 0;
    if (pQueue->m_cClients != 0) {
        do {
            pClientObject = (RDISPATCH_ClientObject *)pClientNode->m_pClient;
            if (((int (*)(void *, void *))pClientObject->m_pVtable[2])(pClientNode->m_pClient, pEntryWords) == 1) {
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
    RDISPATCH_Queue *pQueue;
    unsigned int cQueuedEntries;
    unsigned int cDrained;
    unsigned int auEntryWords[5];

    pQueue = (RDISPATCH_Queue *)pDispatchQueue;
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
            if (DispatchRenderQueueEntryToClients(pDispatchQueue, auEntryWords) == 0) {
                LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
                return 0;
            }
            ++cDrained;
        } while (cDrained < cEntries);
    }

    LeaveObjectCriticalSection((char *)pDispatchQueue + 8);
    return 1;
}
