#include "vsmem.h"

#include <stdlib.h>

struct VSMEM_BlockHeader {
    unsigned int m_cbPayload;
};

static long g_cbMainArenaCapacity = 0;
static long g_cbMainArenaInUse = 0;
static int g_fMainArenaReady = 0;

// FUNCTION: LEMBALL 0x0046F060
int InitializeMasterMainRamArena(void) {
    g_cbMainArenaCapacity = 2L * 1024L * 1024L;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x0046F120
void ShutdownMasterMainRamArena(void) {
    g_cbMainArenaCapacity = 0;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 0;
}

// FUNCTION: LEMBALL 0x0045A780
void *AllocateVSMemBlock(unsigned int cbBlock) {
    VSMEM_BlockHeader *pHeader;

    if (!g_fMainArenaReady || cbBlock == 0) {
        return 0;
    }

    if ((long)cbBlock > CalculateMemoryArenaAvailableBytes()) {
        return 0;
    }

    pHeader = (VSMEM_BlockHeader *)malloc(sizeof(VSMEM_BlockHeader) + cbBlock);
    if (pHeader == 0) {
        return 0;
    }

    pHeader->m_cbPayload = cbBlock;
    g_cbMainArenaInUse += (long)cbBlock;
    return pHeader + 1;
}

// FUNCTION: LEMBALL 0x0045A790
void FreeVSMemBlock(void *pvBlock) {
    VSMEM_BlockHeader *pHeader;

    if (pvBlock == 0) {
        return;
    }

    pHeader = ((VSMEM_BlockHeader *)pvBlock) - 1;
    g_cbMainArenaInUse -= (long)pHeader->m_cbPayload;
    free(pHeader);
}

// FUNCTION: LEMBALL 0x0045A350
long CalculateMemoryArenaAvailableBytes(void) {
    if (!g_fMainArenaReady) {
        return 0;
    }

    return g_cbMainArenaCapacity - g_cbMainArenaInUse;
}
