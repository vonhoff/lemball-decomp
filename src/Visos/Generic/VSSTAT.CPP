#include "Platform/Windows/Mixed/Engine/CORE/SAFEVT.H"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Control/CGame.h"
#include "Visos/Generic/Memory.h"

#include <ctype.h>
#include <string.h>

void LEMBALL_FASTCALL DestroyNamedStatusEntry(void* pEntry);

struct GameStatusEntryInitialVtable {
	int (*m_pPurecall)(void);
	void* m_pReserved;
	void(LEMBALL_FASTCALL* m_pEnterCriticalSection)(void*);
	void(LEMBALL_FASTCALL* m_pLeaveCriticalSection)(void*);
	void* (CArena::*m_pStreamOut)(VsInitFormattedOutputStream*);
	void*(LEMBALL_FASTCALL* m_pDestroyMemoryArena)(void*, int, int);
	int (CArena::*m_pAllocate)(void**, unsigned int, const char*);
	int (CArena::*m_pFree)(void*);
};
// GLOBAL: LEMBALL 0x00498908
GameStatusEntryInitialVtable g_GAME_StatusEntryDeleteVtable = {
	_purecall,
	0,
	EnterObjectCriticalSection,
	LeaveObjectCriticalSection,
	&CArena::StreamOut,
	DestroyMemoryArenaReturnThis,
	&CArena::Allocate,
	&CArena::Free,
};
// GLOBAL: LEMBALL 0x00498960
static void* g_GAME_StatusEntryVtable[8] = {
	(void*) WriteNamedStatusEntry,
	(void*) UpdateNamedStatusEntry,
	(void*) DeleteFixedBufferStreamReturnThis,
	(void*) ResetFixedBufferStream,
	(void*) AppendCharToFixedBufferStream,
	(void*) AppendCStringToFixedBufferStream,
	(void*) ReturnStreamArgument,
	0,
};

// FUNCTION: LEMBALL 0x0045ac10
GameStatusEntry::GameStatusEntry(const char* pszName)
{
	m_pVtable = (void**) &g_GAME_StatusEntryDeleteVtable;
	m_Name.ConstructDynamicCString();
	m_pVtable = g_GAME_StatusEntryVtable;
	m_Name.AssignDynamicCString(pszName);
	m_nMinimumValue = -1;
	m_nMaximumValue = 0;
	m_nTotalValue = 0;
	m_cSamples = 0;
}
// FUNCTION: LEMBALL 0x0045ac50
void LEMBALL_FASTCALL DestroyNamedStatusEntry(void* pEntry)
{
	GameStatusEntry* pStatusEntry;

	pStatusEntry = (GameStatusEntry*) pEntry;
	pStatusEntry->m_pVtable = g_GAME_StatusEntryVtable;
	pStatusEntry->m_Name.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x0045ac60
void UpdateNamedStatusEntry(void* pEntry, unsigned int nValue)
{
	GameStatusEntry* pStatusEntry;

	pStatusEntry = (GameStatusEntry*) pEntry;
	if (pStatusEntry->m_cSamples != 0) {
		pStatusEntry->m_nTotalValue += nValue;
		if ((unsigned int) pStatusEntry->m_nMaximumValue <= nValue) {
			pStatusEntry->m_nMaximumValue = (int) nValue;
		}
		if ((unsigned int) pStatusEntry->m_nMinimumValue >= nValue) {
			pStatusEntry->m_nMinimumValue = (int) nValue;
		}
	}
	++pStatusEntry->m_cSamples;
}

// FUNCTION: LEMBALL 0x0045ac90
VsInitFormattedOutputStream* LEMBALL_FASTCALL WriteNamedStatusEntry(void* pEntry,
																	int,
																	VsInitFormattedOutputStream* pStream)
{
	GameStatusEntry* pStatusEntry;
	unsigned int nAverage;

	pStatusEntry = (GameStatusEntry*) pEntry;
	if (pStatusEntry->m_cSamples == 0) {
		pStream->AppendCStringToStream("----\n");
		return pStream;
	}

	nAverage = (unsigned int) pStatusEntry->m_nTotalValue / (unsigned int) pStatusEntry->m_cSamples;
	return ((VsInitFormattedOutputStream*) AppendDynamicCStringToStream(
				AppendHexUIntToStream(
					AppendHexUIntToStream(
						AppendHexUIntToStream(
							AppendHexUIntToStream(AppendHexUIntToStream(pStream, nAverage)->AppendCStringToStream(" "),
												  (unsigned int) pStatusEntry->m_nTotalValue)
								->AppendCStringToStream(" "),
							(unsigned int) pStatusEntry->m_nMaximumValue)
							->AppendCStringToStream(" "),
						(unsigned int) pStatusEntry->m_nMinimumValue)
						->AppendCStringToStream(" "),
					(unsigned int) pStatusEntry->m_cSamples)
					->AppendCStringToStream(" "),
				&pStatusEntry->m_Name))
		->AppendCStringToStream("\n");
}
// MACINTOSH: CODE_02_Visos (Generic).bin 0x00000EB0 __ct__7CStringFv
// FUNCTION: LEMBALL 0x0046e410
CString* CString::ConstructDynamicCString(void)
{
	CString* pString = this;
	char* pszBuffer;

	pString->m_cchCapacity = 1;
	pszBuffer = (char*) AllocateVSMemBlock(1);
	pString->m_pszText = pszBuffer;
	*pszBuffer = '\0';
	return pString;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00000F0A __ct__7CStringFPc
// FUNCTION: LEMBALL 0x0046e430
CString* CString::ConstructDynamicCStringFromCString(const char* pszText)
{
	CString* pString = this;
	unsigned int cchText;

	cchText = (unsigned int) strlen(pszText) + 1;
	pString->m_cchCapacity = (int) cchText;
	pString->m_pszText = (char*) AllocateVSMemBlock(cchText);
	strcpy(pString->m_pszText, pszText);
	return pString;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00000F5C __ct__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e480
CString* CString::CopyConstructDynamicCString(const CString* pSource)
{
	CString* pString = this;
	pString->m_cchCapacity = pSource->m_cchCapacity;
	pString->m_pszText = (char*) AllocateVSMemBlock((unsigned int) pString->m_cchCapacity);
	strcpy(pString->m_pszText, pSource->m_pszText);
	return pString;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00000FA8 __dt__7CStringFv
// FUNCTION: LEMBALL 0x0046e500
void CString::DestroyDynamicCString(void)
{
	FreeVSMemBlock(m_pszText);
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001008 __as__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e510
CString* CString::AssignDynamicCStringFromDynamic(const CString* pSource)

{
	CString* pString = this;
	FreeVSMemBlock(pString->m_pszText);
	pString->m_cchCapacity = pSource->m_cchCapacity;
	pString->m_pszText = (char*) AllocateVSMemBlock((unsigned int) pString->m_cchCapacity);
	memcpy(pString->m_pszText, pSource->m_pszText, (unsigned int) strlen(pSource->m_pszText) + 1);
	return pString;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001078 __as__7CStringFPCc
// FUNCTION: LEMBALL 0x0046e570
CString* CString::AssignDynamicCString(const char* pszText)
{
	CString* pString = this;
	char ch;
	unsigned int cchText;
	unsigned int cDwords;
	char* pszSource;
	char* pszDest;

	cchText = 0xffffffffU;
	pszSource = (char*) pszText;
	do {
		if (cchText == 0) {
			break;
		}
		--cchText;
		ch = *pszSource;
		++pszSource;
	} while (ch != '\0');
	cchText = ~cchText;
	if (pString->m_cchCapacity < (int) cchText) {
		FreeVSMemBlock(pString->m_pszText);
		pString->m_pszText = (char*) AllocateVSMemBlock(cchText);
		pString->m_cchCapacity = (int) cchText;
	}

	cchText = 0xffffffffU;
	do {
		if (cchText == 0) {
			break;
		}
		pszSource = (char*) pszText;
		--cchText;
		pszSource = (char*) pszText + 1;
		ch = *pszText;
		pszText = pszSource;
	} while (ch != '\0');
	cchText = ~cchText;
	pszSource -= cchText;
	pszDest = pString->m_pszText;
	cDwords = cchText >> 2;
	while (cDwords != 0) {
		*(unsigned int*) pszDest = *(const unsigned int*) pszSource;
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

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001150 __apl__7CStringFR7CString
// FUNCTION: LEMBALL 0x0046e5d0
CString* CString::AppendDynamicCStringObjectAndCopyResult(CString* pResult,
																				const CString* pSuffix)

{
	CString* pString = this;
	unsigned int cchPrefix;
	unsigned int cchSuffix;
	unsigned int cchTotal;
	char* pszText;

	cchPrefix = (unsigned int) strlen(pString->m_pszText);
	cchSuffix = (unsigned int) strlen(pSuffix->m_pszText);
	cchTotal = cchPrefix + cchSuffix + 1;
	pszText = (char*) AllocateVSMemBlock(cchTotal);
	memcpy(pszText, pString->m_pszText, cchPrefix);
	memcpy(pszText + cchPrefix, pSuffix->m_pszText, cchSuffix + 1);
	FreeVSMemBlock(pString->m_pszText);
	pString->m_pszText = (char*) AllocateVSMemBlock(cchTotal);
	pString->m_cchCapacity = (int) cchTotal;
	memcpy(pString->m_pszText, pszText, cchTotal);
	pResult->CopyConstructDynamicCString(pString);
	FreeVSMemBlock(pszText);
	return pResult;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001228 __apl__7CStringFPCc
// FUNCTION: LEMBALL 0x0046e6e0
CString* CString::AppendDynamicCStringAndCopyResult(CString* pResult,
																		  const char* pszSuffix)

{
	CString* pString = this;
	unsigned int cchPrefix;
	unsigned int cchSuffix;
	unsigned int cchTotal;
	char* pszText;

	cchPrefix = (unsigned int) strlen(pString->m_pszText);
	cchSuffix = (unsigned int) strlen(pszSuffix);
	cchTotal = cchPrefix + cchSuffix + 1;
	pszText = (char*) AllocateVSMemBlock(cchTotal);
	memcpy(pszText, pString->m_pszText, cchPrefix);
	memcpy(pszText + cchPrefix, pszSuffix, cchSuffix + 1);
	FreeVSMemBlock(pString->m_pszText);
	pString->m_pszText = (char*) AllocateVSMemBlock(cchTotal);
	pString->m_cchCapacity = (int) cchTotal;
	memcpy(pString->m_pszText, pszText, cchTotal);
	pResult->CopyConstructDynamicCString(pString);
	FreeVSMemBlock(pszText);
	return pResult;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x0000125A __ls__FR10CVSOStreamR7CString
// FUNCTION: LEMBALL 0x0046e7f0
void* AppendDynamicCStringToStream(void* pStream, const CString* pString)
{
	((VsInitFormattedOutputStream*) pStream)->AppendCStringToStream(pString->m_pszText);
	return pStream;
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001296 getlength__7CStringFv
// FUNCTION: LEMBALL 0x0046ef00
int LEMBALL_FASTCALL GetDynamicCStringLength(const CString* pString)
{
	return (int) strlen(pString->m_pszText);
}

// MACINTOSH: CODE_02_Visos (Generic).bin 0x00001312 lower__7CStringFv
// FUNCTION: LEMBALL 0x0046efa0
CString* CString::LowercaseAndCopyDynamicCString(CString* pResult)
{
	char* pszText;
	int cchText;

	pszText = m_pszText;
	cchText = (int) strlen(pszText);
	while (cchText > 0) {
		if (isupper(*pszText)) {
			*pszText = (char) tolower(*pszText);
		}
		++pszText;
		--cchText;
	}
	pResult->CopyConstructDynamicCString(this);
	return pResult;
}
