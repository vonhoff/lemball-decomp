#include "../game/game_app.h"
#include "../engine/runtime_init.h"
#include "../engine/memory_arena.h"
#include "../network/safe_vtable.h"
#include <string.h>

void DestroyNamedStatusEntry(void *pEntry);

// GLOBAL: LEMBALL 0x00498908
static void *g_GAME_StatusEntryDeleteVtable[8] = {
    (void *)DestroyNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
    (void *)ReturnStreamArgument,
    (void *)NetworkSafeVtableNoop,
};
// GLOBAL: LEMBALL 0x00498960
static void *g_GAME_StatusEntryVtable[8] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
    (void *)ReturnStreamArgument,
    (void *)NetworkSafeVtableNoop,
};

// FUNCTION: LEMBALL 0x0046E410
GAME_DynamicCString *GAME_DynamicCString::ConstructDynamicCString(void) {
    GAME_DynamicCString *pString = this;
    char *pszBuffer;

    pString->m_cchCapacity = 1;
    pszBuffer = (char *)AllocateVSMemBlock(1);
    pString->m_pszText = pszBuffer;
    *pszBuffer = '\0';
    return pString;
}

// FUNCTION: LEMBALL 0x0046E500
void DestroyDynamicCString(GAME_DynamicCString *pString) {
    FreeVSMemBlock(pString->m_pszText);
}

// FUNCTION: LEMBALL 0x0046E570
GAME_DynamicCString *GAME_DynamicCString::AssignDynamicCString(const char *pszText) {
    GAME_DynamicCString *pString = this;
    char ch;
    unsigned int cchText;
    unsigned int cDwords;
    char *pszSource;
    char *pszDest;

    cchText = 0xffffffffU;
    pszSource = (char *)pszText;
    do {
        if (cchText == 0) {
            break;
        }
        --cchText;
        ch = *pszSource;
        ++pszSource;
    } while (ch != '\0');
    cchText = ~cchText;
    if (pString->m_cchCapacity < (int)cchText) {
        FreeVSMemBlock(pString->m_pszText);
        pString->m_pszText = (char *)AllocateVSMemBlock(cchText);
        pString->m_cchCapacity = (int)cchText;
    }

    cchText = 0xffffffffU;
    do {
        if (cchText == 0) {
            break;
        }
        pszSource = (char *)pszText;
        --cchText;
        pszSource = (char *)pszText + 1;
        ch = *pszText;
        pszText = pszSource;
    } while (ch != '\0');
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
    cchText &= 3;
    while (cchText != 0) {
        *pszDest++ = *pszSource++;
        --cchText;
    }
    return pString;
}

// FUNCTION: LEMBALL 0x0046E430
GAME_DynamicCString *GAME_DynamicCString::ConstructDynamicCStringFromCString(const char *pszText) {
    GAME_DynamicCString *pString = this;
    unsigned int cchText;

    cchText = (unsigned int)strlen(pszText) + 1;
    pString->m_cchCapacity = (int)cchText;
    pString->m_pszText = (char *)AllocateVSMemBlock(cchText);
    memcpy(pString->m_pszText, pszText, cchText);
    return pString;
}

// FUNCTION: LEMBALL 0x0046E480
GAME_DynamicCString *GAME_DynamicCString::CopyConstructDynamicCString(const GAME_DynamicCString *pSource) {
    GAME_DynamicCString *pString = this;
    pString->m_cchCapacity = pSource->m_cchCapacity;
    pString->m_pszText = (char *)AllocateVSMemBlock((unsigned int)pString->m_cchCapacity);
    memcpy(pString->m_pszText, pSource->m_pszText,
           (unsigned int)strlen(pSource->m_pszText) + 1);
    return pString;
}

// FUNCTION: LEMBALL 0x0046E510
GAME_DynamicCString *GAME_DynamicCString::AssignDynamicCStringFromDynamic(
    const GAME_DynamicCString *pSource) {
    GAME_DynamicCString *pString = this;
    FreeVSMemBlock(pString->m_pszText);
    pString->m_cchCapacity = pSource->m_cchCapacity;
    pString->m_pszText = (char *)AllocateVSMemBlock((unsigned int)pString->m_cchCapacity);
    memcpy(pString->m_pszText, pSource->m_pszText,
           (unsigned int)strlen(pSource->m_pszText) + 1);
    return pString;
}

// FUNCTION: LEMBALL 0x0046E5D0
GAME_DynamicCString *GAME_DynamicCString::AppendDynamicCStringObjectAndCopyResult(
    GAME_DynamicCString *pResult, const GAME_DynamicCString *pSuffix) {
    GAME_DynamicCString *pString = this;
    unsigned int cchPrefix;
    unsigned int cchSuffix;
    unsigned int cchTotal;
    char *pszText;

    cchPrefix = (unsigned int)strlen(pString->m_pszText);
    cchSuffix = (unsigned int)strlen(pSuffix->m_pszText);
    cchTotal = cchPrefix + cchSuffix + 1;
    pszText = (char *)AllocateVSMemBlock(cchTotal);
    memcpy(pszText, pString->m_pszText, cchPrefix);
    memcpy(pszText + cchPrefix, pSuffix->m_pszText, cchSuffix + 1);
    FreeVSMemBlock(pString->m_pszText);
    pString->m_pszText = (char *)AllocateVSMemBlock(cchTotal);
    pString->m_cchCapacity = (int)cchTotal;
    memcpy(pString->m_pszText, pszText, cchTotal);
    pResult->CopyConstructDynamicCString(pString);
    FreeVSMemBlock(pszText);
    return pResult;
}

// FUNCTION: LEMBALL 0x0046E6E0
GAME_DynamicCString *GAME_DynamicCString::AppendDynamicCStringAndCopyResult(
    GAME_DynamicCString *pResult, const char *pszSuffix) {
    GAME_DynamicCString *pString = this;
    unsigned int cchPrefix;
    unsigned int cchSuffix;
    unsigned int cchTotal;
    char *pszText;

    cchPrefix = (unsigned int)strlen(pString->m_pszText);
    cchSuffix = (unsigned int)strlen(pszSuffix);
    cchTotal = cchPrefix + cchSuffix + 1;
    pszText = (char *)AllocateVSMemBlock(cchTotal);
    memcpy(pszText, pString->m_pszText, cchPrefix);
    memcpy(pszText + cchPrefix, pszSuffix, cchSuffix + 1);
    FreeVSMemBlock(pString->m_pszText);
    pString->m_pszText = (char *)AllocateVSMemBlock(cchTotal);
    pString->m_cchCapacity = (int)cchTotal;
    memcpy(pString->m_pszText, pszText, cchTotal);
    pResult->CopyConstructDynamicCString(pString);
    FreeVSMemBlock(pszText);
    return pResult;
}

// FUNCTION: LEMBALL 0x0046E7F0
void *AppendDynamicCStringToStream(void *pStream, const GAME_DynamicCString *pString) {
    AppendCStringToStream((VSINIT_FormattedOutputStream *)pStream, pString->m_pszText);
    return pStream;
}

// FUNCTION: LEMBALL 0x0046EF00
int LEMBALL_FASTCALL GetDynamicCStringLength(const GAME_DynamicCString *pString) {
    return (int)strlen(pString->m_pszText);
}

// FUNCTION: LEMBALL 0x0045AC10
GAME_StatusEntry::GAME_StatusEntry(const char *pszName) {
    m_pVtable = g_GAME_StatusEntryDeleteVtable;
    m_Name.ConstructDynamicCString();
    m_pVtable = g_GAME_StatusEntryVtable;
    m_Name.AssignDynamicCString(pszName);
    m_nMinimumValue = -1;
    m_nMaximumValue = 0;
    m_nTotalValue = 0;
    m_cSamples = 0;
}

// FUNCTION: LEMBALL 0x0045AC50
void DestroyNamedStatusEntry(void *pEntry) {
    GAME_StatusEntry *pStatusEntry;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    pStatusEntry->m_pVtable = g_GAME_StatusEntryVtable;
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
VSINIT_FormattedOutputStream *LEMBALL_FASTCALL WriteNamedStatusEntry(void *pEntry, int, VSINIT_FormattedOutputStream *pStream) {
    GAME_StatusEntry *pStatusEntry;
    unsigned int nAverage;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    if (pStatusEntry->m_cSamples == 0) {
        pStream->AppendCStringToStream("----\n");
        return pStream;
    }

    nAverage = (unsigned int)pStatusEntry->m_nTotalValue / (unsigned int)pStatusEntry->m_cSamples;
    return ((VSINIT_FormattedOutputStream *)AppendDynamicCStringToStream(
        AppendHexUIntToStream(
            AppendHexUIntToStream(
                AppendHexUIntToStream(
                    AppendHexUIntToStream(
                        AppendHexUIntToStream(pStream, nAverage)
                            ->AppendCStringToStream(" "),
                        (unsigned int)pStatusEntry->m_nTotalValue)
                        ->AppendCStringToStream(" "),
                    (unsigned int)pStatusEntry->m_nMaximumValue)
                    ->AppendCStringToStream(" "),
                (unsigned int)pStatusEntry->m_nMinimumValue)
                ->AppendCStringToStream(" "),
            (unsigned int)pStatusEntry->m_cSamples)
            ->AppendCStringToStream(" "),
        &pStatusEntry->m_Name))->AppendCStringToStream("\n");
}
