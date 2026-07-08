#include "../game/game_app.h"
#include "../engine/runtime_init.h"
#include "../engine/memory_arena.h"

static void *g_GAME_StatusEntryVtable[] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
    (void *)ReturnStreamArgument,
    0,
};

// FUNCTION: LEMBALL 0x0046E410
GAME_DynamicCString *ConstructDynamicCString(GAME_DynamicCString *pString) {
    pString->m_cchCapacity = 1;
    pString->m_pszText = (char *)AllocateVSMemBlock(1);
    pString->m_pszText[0] = '\0';
    return pString;
}

// FUNCTION: LEMBALL 0x0046E500
void DestroyDynamicCString(GAME_DynamicCString *pString) {
    FreeVSMemBlock(pString->m_pszText);
}

// FUNCTION: LEMBALL 0x0046E570
GAME_DynamicCString *AssignDynamicCString(GAME_DynamicCString *pString, const char *pszText) {
    const char *pszScan;
    const char *pszSource;
    char *pszDest;
    unsigned int cchText;
    unsigned int cDwords;
    unsigned int cBytes;

    cchText = 0xffffffff;
    pszScan = pszText;
    do {
        if (cchText == 0) {
            break;
        }
        --cchText;
    } while (*pszScan++ != '\0');
    cchText = ~cchText;
    if (pString->m_cchCapacity < (int)cchText) {
        FreeVSMemBlock(pString->m_pszText);
        pString->m_pszText = (char *)AllocateVSMemBlock((unsigned int)cchText);
        pString->m_cchCapacity = (int)cchText;
    }

    cchText = 0xffffffff;
    pszScan = pszText;
    do {
        pszSource = pszScan;
        if (cchText == 0) {
            break;
        }
        --cchText;
        pszSource = pszScan + 1;
    } while (*pszScan++ != '\0');
    cchText = ~cchText;
    pszSource -= cchText;
    pszDest = pString->m_pszText;
    cDwords = cchText >> 2;
    while (cDwords != 0) {
        *(unsigned int *)pszDest = *(const unsigned int *)pszSource;
        pszSource += 4;
        pszDest += 4;
        --cDwords;
    }
    cBytes = cchText & 3;
    while (cBytes != 0) {
        *pszDest++ = *pszSource++;
        --cBytes;
    }
    return pString;
}

// FUNCTION: LEMBALL 0x0045AC10
GAME_StatusEntry::GAME_StatusEntry(const char *pszName) {
    m_pVtable = g_GAME_StatusEntryVtable;
    ConstructDynamicCString(&m_Name);
    m_nReserved04 = 0;
    AssignDynamicCString(&m_Name, pszName);
    m_nMinimumValue = -1;
    m_nMaximumValue = 0;
    m_nTotalValue = 0;
    m_cSamples = 0;
}

// FUNCTION: LEMBALL 0x0045AC50
void DestroyNamedStatusEntry(void *pEntry) {
    GAME_StatusEntry *pStatusEntry;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    DestroyDynamicCString(&pStatusEntry->m_Name);
}

// FUNCTION: LEMBALL 0x0045AC60
void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue) {
    GAME_StatusEntry *pStatusEntry;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    if (pStatusEntry->m_cSamples != 0) {
        pStatusEntry->m_nTotalValue += nValue;
        if ((unsigned int)pStatusEntry->m_nMaximumValue <= nValue) {
            pStatusEntry->m_nMaximumValue = (int)nValue;
        }
        if ((unsigned int)pStatusEntry->m_nMinimumValue >= nValue) {
            pStatusEntry->m_nMinimumValue = (int)nValue;
        }
    }
    ++pStatusEntry->m_cSamples;
}

// FUNCTION: LEMBALL 0x0045AC90
VSINIT_FormattedOutputStream *WriteNamedStatusEntry(void *pEntry, VSINIT_FormattedOutputStream *pStream) {
    GAME_StatusEntry *pStatusEntry;
    unsigned int nAverage;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    if (pStatusEntry->m_cSamples == 0) {
        AppendCStringToStream(pStream, "----\n");
        return pStream;
    }

    nAverage = (unsigned int)pStatusEntry->m_nTotalValue / (unsigned int)pStatusEntry->m_cSamples;
    AppendHexUIntToStream(pStream, nAverage);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nTotalValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nMaximumValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nMinimumValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_cSamples);
    AppendCStringToStream(pStream, " ");
    AppendCStringToStream(pStream, pStatusEntry->m_Name.m_pszText);
    AppendCStringToStream(pStream, "\n");
    return pStream;
}
