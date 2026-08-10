#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Control/CGame.h"
#include "Platform/Windows/Mixed/Level/DEMO.H"
#include "Visos/Generic/MogLoad.h"
#include "Visos/Generic/Memory.h"

#include <stdio.h>

extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(void* pObject,
																	   void* pUnusedEdx,
																	   unsigned int* pSource,
																	   unsigned int nUnused,
																	   unsigned int cbBuffer);

typedef void*(LEMBALL_FASTCALL* DemoDeleteProc)(void* pObject, int nUnusedEdx, unsigned char fDelete);
extern void ReturnVoidVtableCallback(void);
extern void* g_GAME_RenderQueueNodeVtableSlots[4];

void* LEMBALL_FASTCALL DeleteLevelDemoPlaybackControllerThunk(void* pPlaybackController, int nUnused, int fDelete);
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEventThunk(void* pPlaybackController,
															int nUnused,
															unsigned short* pEventId);

static const char g_DEMO_OpenReadMode[] = "rb";
static const unsigned int g_DEMO_BinResourceTypeTag = 0x42494e20;

extern void ParseIntResourceDescriptor(void);
extern int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void* pObject);
extern int LEMBALL_FASTCALL GetField1CVfunc04(void* pObject);
extern int LEMBALL_FASTCALL GetField14Vfunc05(void* pObject);
extern int ReturnZeroVfunc06(void);
extern void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void* pObject);
extern void LEMBALL_FASTCALL UnloadTypedResourceObject(void* pObject, void* pUnusedEcx, int fReleaseMode);
extern void LEMBALL_FASTCALL NoopVfunc09(void* pObject, void* pUnusedEdx, int nArgument);
extern void* LEMBALL_FASTCALL GetEffResourceDataPointer(void* pObject);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern int LEMBALL_FASTCALL GetField28GetMemorySize(void* pObject);
extern void LEMBALL_FASTCALL InitializeTypedResourceObjectBaseVtable(void* pObject);
extern void LEMBALL_FASTCALL SetPresResourceTypeTag(void* pObject);

void LEMBALL_FASTCALL SetBinResourceTypeTag(void* pObject);
void* LEMBALL_FASTCALL DestroyBinResource(void* pObject, int nUnused, int fDelete);
void* LEMBALL_FASTCALL DestroyPresResourceAuto(void* pObject, void* pUnused, BYTE fDelete);

static void* g_DEMO_BinResourceVtableStorage[15] = {
	(void*) DestroyBinResource,
	(void*) CopyBufferIntoTypedResourceObjectAndParse,
	(void*) ParseIntResourceDescriptor,
	(void*) GetField14NeedsAgeIncrement,
	(void*) GetField1CVfunc04,
	(void*) GetField14Vfunc05,
	(void*) ReturnZeroVfunc06,
	(void*) EnsureTypedResourceObjectLoaded,
	(void*) UnloadTypedResourceObject,
	(void*) NoopVfunc09,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetBinResourceTypeTag,
	(void*) GetField28GetMemorySize,
};
static void* g_DEMO_BinResourceVtable = g_DEMO_BinResourceVtableStorage;

// GLOBAL: LEMBALL 0x00498e20
static void* g_DEMO_PresResourceVtableStorage[15] = {
	(void*) DestroyPresResourceAuto,
	(void*) CopyBufferIntoTypedResourceObjectAndParse,
	(void*) ParseIntResourceDescriptor,
	(void*) GetField14NeedsAgeIncrement,
	(void*) GetField1CVfunc04,
	(void*) GetField14Vfunc05,
	(void*) ReturnZeroVfunc06,
	(void*) EnsureTypedResourceObjectLoaded,
	(void*) UnloadTypedResourceObject,
	(void*) NoopVfunc09,
	(void*) GetEffResourceDataPointer,
	(void*) NoopOnLoadVfunc11,
	(void*) NoopVfunc12,
	(void*) SetPresResourceTypeTag,
	(void*) GetField28GetMemorySize,
};
static void* g_DEMO_PresResourceVtable = g_DEMO_PresResourceVtableStorage;

extern void* g_pMainResourceArchive;
extern void* g_MOGLOAD_CachedResourceObjectBaseVtable[15];
extern void __fastcall InitializeResourceObjectFromId(void* pObject, void* pUnusedEdx, int nResourceId);
extern void* __fastcall FinalizeLoadedResourceObjectResult(void* pObject, void* pUnusedEdx);

void* LEMBALL_FASTCALL DeleteLevelDemoPlaybackControllerThunk(void* pPlaybackController, int, int fDelete);
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEventThunk(void* pPlaybackController, int, unsigned short* pEventId);
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBufferThunk(void* pPlaybackController);
void CreateFrameTimerController(unsigned int uFrameInterval);
void DestroyFrameTimerController(void);
int LoadNextLevelDemoRecordBuffer(void* pPlaybackController);
void ReadLevelDemoLengthPrefixedRecordThunk(void* pPlaybackController, void* pRecordBuffer, unsigned int* pcbRecord);
void LEMBALL_FASTCALL ServiceLevelDemoPlayback(void* pPlaybackController);
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBuffer(void* pPlaybackController);
void LEMBALL_FASTCALL StopLevelDemoPlayback(void* pPlaybackController);
void LEMBALL_FASTCALL StopLevelDemoPlaybackThunk(void* pPlaybackController);
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEvent(void* pPlaybackController, int nUnused, unsigned short* pEventId);
void* LEMBALL_FASTCALL RestoreLevelDemoPlaybackBaseVtable(void* pPlaybackController, void*, unsigned char fDelete);
int* LoadBinResource(int nResourceId);
void LEMBALL_FASTCALL SetBinResourceTypeTag(void* pObject);
void* LEMBALL_FASTCALL DestroyBinResource(void* pObject, int nUnused, int fDelete);
FILE* OpenFileWithMode(const char* pszPath, const char* pszMode);
void CloseCrtFilePointer(FILE* pFile);
unsigned int ReadFileBytes(FILE* pFile, void* pBuffer, size_t cbBuffer);
unsigned int SeekFile(FILE* pFile, long lOffset, int nOrigin);
unsigned int TellFile(FILE* pFile);
unsigned int GetFileLengthPreservingPosition(FILE* pFile);

// GLOBAL: LEMBALL 0x493100
static void* g_LevelDemoPlaybackControllerVtable[4] = {
	(void*) InitializeNonZrleVariantRenderEntry,
	(void*) DeleteLevelDemoPlaybackControllerThunk,
	(void*) FilterLevelDemoPlaybackInputEventThunk,
	0,
};
void* g_pNonZrleVariantRenderEntryInitializeVtable[2] = {(void*) ReturnVoidVtableCallback,
														 (void*) ReturnVoidVtableCallback};

// GLOBAL: LEMBALL 0x004a6408
void* g_pLevelDemoPlaybackController;
void* g_pLevelDemoPlaybackDescriptor = 0;

union DemoLevelDemoPlaybackConstructorAddress {
	DemoLevelDemoPlaybackController* (DemoLevelDemoPlaybackController::*m_pMethod)(unsigned int);
	void* m_pAddress;
};

static DemoLevelDemoPlaybackConstructorAddress g_DEMO_LevelDemoPlaybackConstructorAddress = {
	&DemoLevelDemoPlaybackController::ConstructLevelDemoPlaybackController};

#include "Platform/Windows/Mixed/Engine/CORE/SAFEVT.H"

// LINKERILT: LEMBALL 0x004016a4
void* LEMBALL_FASTCALL DeleteLevelDemoPlaybackControllerThunk(void* pPlaybackController, int, int fDelete)

{
	DemoLevelDemoPlaybackController* pController;

	pController = (DemoLevelDemoPlaybackController*) pPlaybackController;
	pController->DestroyLevelDemoPlaybackController();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pPlaybackController);
	}
	return pPlaybackController;
}

// LINKERILT: LEMBALL 0x0040179e
void DemoLevelDemoPlaybackController::ResetLevelDemoPlaybackCursorThunk(void)
{
	ResetLevelDemoPlaybackCursor();
}

// LINKERILT: LEMBALL 0x004017a3
DemoLevelDemoPlaybackController* DemoLevelDemoPlaybackController::ConstructLevelDemoPlaybackControllerThunk(
	unsigned int uFrameInterval)

{
	return (this->*g_DEMO_LevelDemoPlaybackConstructorAddress.m_pMethod)(uFrameInterval);
}

// LINKERILT: LEMBALL 0x00401a28
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEventThunk(void* pPlaybackController, int, unsigned short* pEventId)

{
	return FilterLevelDemoPlaybackInputEvent(pPlaybackController, 0, pEventId);
}

// LINKERILT: LEMBALL 0x00401a55
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBufferThunk(void* pPlaybackController)
{
	ReleaseLevelDemoRecordBuffer(pPlaybackController);
}

// LINKERILT: LEMBALL 0x00402eaa
void LEMBALL_FASTCALL StopLevelDemoPlaybackThunk(void* pPlaybackController)
{
	StopLevelDemoPlayback(pPlaybackController);
}

// MACINTOSH: _DEMO_Init(int)
// FUNCTION: LEMBALL 0x00409150
void CreateFrameTimerController(unsigned int uFrameInterval)
{
	DemoLevelDemoPlaybackController* pPlaybackController;

	pPlaybackController = (DemoLevelDemoPlaybackController*) AllocateVSMemBlock(0x58);
	if (pPlaybackController != 0) {
		g_pLevelDemoPlaybackController = pPlaybackController->ConstructLevelDemoPlaybackControllerThunk(uFrameInterval);
		return;
	}
	g_pLevelDemoPlaybackController = 0;
}

// MACINTOSH: _DEMO_Quit()
// FUNCTION: LEMBALL 0x00409180
void DestroyFrameTimerController(void)
{
	void* pPlaybackController;

	pPlaybackController = g_pLevelDemoPlaybackController;
	if (g_pLevelDemoPlaybackController != 0) {
		((DemoDeleteProc) (*(void***) pPlaybackController)[1])(pPlaybackController, 0, 1);
		g_pLevelDemoPlaybackController = 0;
	}
}

// MACINTOSH: CDemo::CDemo(int)
// FUNCTION: LEMBALL 0x004091b0
DemoLevelDemoPlaybackController* DemoLevelDemoPlaybackController::ConstructLevelDemoPlaybackController(
	unsigned int uFrameInterval)
{
	InitializeRenderQueueNodeBase(this);
	m_nReserved42 = 0;
	*(void***) this = g_LevelDemoPlaybackControllerVtable;
	m_nReserved40 = 0;
	m_nFrameInterval14 = uFrameInterval;
	m_pActiveRecord10 = 0;
	m_pResourceTable34 = 0;
	m_nResourceId28 = 0;
	m_nResourceCount2C = 0;
	m_nResourceTotal30 = 0;
	m_pResourceObject24 = 0;
	m_pResourceHandle20 = 0;
	m_nPlaybackEnabled4C = 0;
	m_nReserved48 = 0;
	m_nStopFlag50 = 0;
	m_nLoadedRecordId18 = -1;
	m_nReserved54 = 0;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(this, -100);
	ResetLevelDemoPlaybackCursorThunk();
	return this;
}

// MACINTOSH: CDemo::~CDemo()
// FUNCTION: LEMBALL 0x00409220
void DemoLevelDemoPlaybackController::DestroyLevelDemoPlaybackController(void)
{
	void* pPlaybackController = this;
	*(void***) pPlaybackController = g_LevelDemoPlaybackControllerVtable;
	ReleaseLevelDemoRecordBufferThunk(pPlaybackController);
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)
		->UnregisterOrderedRenderDispatchClient(pPlaybackController, -100);
	*(void***) pPlaybackController = g_GAME_RenderQueueNodeVtableSlots;
}

// Compatibility shim for original ILT call site.
void DemoLevelDemoPlaybackController::DestroyLevelDemoPlaybackControllerThunk(void)
{
	DestroyLevelDemoPlaybackController();
}

void* LEMBALL_FASTCALL DeleteLevelDemoPlaybackControllerAuto(void* pObject, void*, BYTE fDelete)
{
	((DemoLevelDemoPlaybackController*) pObject)->DestroyLevelDemoPlaybackControllerThunk();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// MACINTOSH: CDemo::LoadBuffer()
// FUNCTION: LEMBALL 0x00409460
int LoadNextLevelDemoRecordBuffer(void* pPlaybackController)
{
	DemoLevelDemoPlaybackController* pController;
	unsigned char bValue;
	unsigned char* pbCursor;
	unsigned short wValue;
	unsigned int cbLength;
	FILE* pFile;
	unsigned int cbFile;
	void* pBuffer;
	unsigned int cbRead;
	int* pResourceObject;
	unsigned int* pcbRemaining;
	int nNextResourceId;

	pController = (DemoLevelDemoPlaybackController*) pPlaybackController;
	if (pController->m_pResourceHandle20 == 0) {
		pResourceObject = LoadBinResource(pController->m_nResourceId28);
		pController->m_pResourceObject24 = pResourceObject;
		if (pResourceObject[4] == 0) {
			((void (*)()) * (void**) (*pResourceObject + 0x1c))();
		}
		else {
			pResourceObject[9] = 0;
		}
		++pResourceObject[2];
		pController->m_pActiveRecord10 =
			((void* (*) (void) ) * (void**) (**(int**) pController->m_pResourceObject24 + 0x28))();
		nNextResourceId = pController->m_nResourceId28 + 1;
		pController->m_nLoadedRecordId18 = *(int*) (*(int*) pController->m_pResourceObject24 + 0x28);
		pController->m_nResourceId28 = nNextResourceId;
		if (pController->m_nResourceTotal30 + pController->m_nResourceCount2C <=
			nNextResourceId) {
			pController->m_nResourceId28 = pController->m_nResourceCount2C;
		}
	}
	else {
		pFile = OpenFileWithMode(pController->m_pResourceHandle20, g_DEMO_OpenReadMode);
		if (pFile == 0) {
			return 0;
		}
		cbFile = GetFileLengthPreservingPosition(pFile);
		pBuffer = AllocateVSMemBlock(cbFile);
		pController->m_pActiveRecord10 = pBuffer;
		cbRead = ReadFileBytes(pFile, pBuffer, cbFile);
		CloseCrtFilePointer(pFile);
		pController->m_nLoadedRecordId18 = cbRead;
	}

	pcbRemaining = (unsigned int*) &pController->m_nLoadedRecordId18;
	pbCursor = (unsigned char*) pController->m_pActiveRecord10;
	pController->m_pRecordBuffer1C = pbCursor;
	bValue = *pbCursor;
	*pcbRemaining = (unsigned int) bValue;
	wValue = (unsigned short) (pbCursor[1] << 8) | bValue;
	*pcbRemaining = (unsigned int) wValue;
	cbLength = ((unsigned int) pbCursor[2] << 0x10) | wValue;
	*pcbRemaining = cbLength;
	bValue = pbCursor[3];
	pController->m_pRecordBuffer1C = pbCursor + 4;
	*pcbRemaining = ((unsigned int) bValue << 0x18) | cbLength;
	return 1;
}

// MACINTOSH: CDemo::GetUserPacket(unsigned char*, unsigned long&)
// FUNCTION: LEMBALL 0x00409560
void ReadLevelDemoLengthPrefixedRecordThunk(void* pPlaybackController, void* pRecordBuffer, unsigned int* pcbRecord)
{
	unsigned char bValue;
	unsigned short wValue;
	unsigned int cbCopy;
	unsigned int cbRemaining;
	unsigned int* pdwCursor;

	if (*(int*) ((char*) pPlaybackController + 0x18) == -1) {
		LoadNextLevelDemoRecordBuffer(pPlaybackController);
	}

	bValue = **(unsigned char**) ((char*) pPlaybackController + 0x1c);
	*pcbRecord = (unsigned int) bValue;
	pdwCursor = (unsigned int*) *(int*) ((char*) pPlaybackController + 0x1c);
	wValue = (unsigned short) (*((unsigned char*) pdwCursor + 1) << 8) | bValue;
	*pcbRecord = (unsigned int) wValue;
	*pcbRecord = ((unsigned int) *((unsigned char*) pdwCursor + 2) << 0x10) | wValue;
	*pcbRecord = ((unsigned int) *((unsigned char*) pdwCursor + 3) << 0x18) | *pcbRecord;

	pdwCursor = (unsigned int*) (*(int*) ((char*) pPlaybackController + 0x1c) + 4);
	*(unsigned int**) ((char*) pPlaybackController + 0x1c) = pdwCursor;
	*(int*) ((char*) pPlaybackController + 0x18) -= (*pcbRecord + 4);

	cbRemaining = *pcbRecord;
	MemCopyCArena(pRecordBuffer, pdwCursor, cbRemaining);
	*(int*) ((char*) pPlaybackController + 0x1c) += *pcbRecord;
	cbCopy = cbRemaining;
	(void) cbCopy;
}

// MACINTOSH: CDemo::Reset()
// FUNCTION: LEMBALL 0x004095e0
void DemoLevelDemoPlaybackController::ResetLevelDemoPlaybackCursor(void)
{
	m_nLoadedRecordId18 = -1;
	m_nPlaybackFrame44 = 0;
	m_nStopFlag50 = 0;
	m_nActiveFrameTick3C = 0;
}

// MACINTOSH: CDemo::SetDemoMode(unsigned char)
// FUNCTION: LEMBALL 0x00409600
void DemoLevelDemoPlaybackController::SetLevelDemoPlaybackEnabled(int fEnabled)
{
	m_nPlaybackEnabled4C = fEnabled;
	if (fEnabled != 0) {
		ResetLevelDemoPlaybackCursorThunk();
		return;
	}
	ReleaseLevelDemoRecordBufferThunk(this);
}

// MACINTOSH: CDemo::Process()
// FUNCTION: LEMBALL 0x00409620
void LEMBALL_FASTCALL ServiceLevelDemoPlayback(void* pPlaybackController)
{
	DemoLevelDemoPlaybackController* pController;
	DWORD dwNow;
	DWORD dwElapsed;

	pController = (DemoLevelDemoPlaybackController*) pPlaybackController;
	if (pController->m_nStopFlag50 == 0 && pController->m_nPlaybackEnabled4C != 0) {
		dwNow = timeGetTime();
		dwElapsed = dwNow - pController->m_nFrameTimeBase38;
		if (pController->m_nActiveFrameTick3C != 0 &&
			pController->m_nActiveFrameTick3C <= dwElapsed) {
			StopLevelDemoPlaybackThunk(pPlaybackController);
			return;
		}
		++pController->m_nPlaybackFrame44;
	}
}

// MACINTOSH: CDemo::CleanUp()
// FUNCTION: LEMBALL 0x00409660
void LEMBALL_FASTCALL ReleaseLevelDemoRecordBuffer(void* pPlaybackController)
{
	DemoLevelDemoPlaybackController* pController;
	int* pReferenceCount;

	pController = (DemoLevelDemoPlaybackController*) pPlaybackController;
	if (pController->m_pResourceObject24 != 0) {
		pReferenceCount = (int*) pController->m_pResourceObject24 + 2;
		--*pReferenceCount;
		ReleaseTypedResourceObjectReference(pController->m_pResourceObject24);
		pController->m_pResourceObject24 = 0;
		pController->m_pActiveRecord10 = 0;
		pController->m_pRecordBuffer1C = 0;
		return;
	}
	if (pController->m_pActiveRecord10 != 0) {
		FreeVSMemBlock(pController->m_pActiveRecord10);
	}
	pController->m_pActiveRecord10 = 0;
	pController->m_pRecordBuffer1C = 0;
}

// MACINTOSH: CDemo::GameIsOver()
// FUNCTION: LEMBALL 0x004096a0
void LEMBALL_FASTCALL StopLevelDemoPlayback(void* pPlaybackController)
{
	((DemoLevelDemoPlaybackController*) pPlaybackController)->m_nStopFlag50 = 1;
	ReleaseLevelDemoRecordBufferThunk(pPlaybackController);
}

// MACINTOSH: CDemo::ProcessMsg(tagMESSAGE*)
// FUNCTION: LEMBALL 0x004096b0
int LEMBALL_FASTCALL FilterLevelDemoPlaybackInputEvent(void* pPlaybackController, int nUnused, unsigned short* pEventId)
{
	unsigned short wEventId;

	(void) nUnused;
	if (*(int*) ((char*) pPlaybackController + 0x4c) != 0) {
		wEventId = *pEventId;
		if ((wEventId & 0x8000) == 0) {
			switch (wEventId) {
			case 1:
			case 3:
			case 5:
				StopLevelDemoPlaybackThunk(pPlaybackController);
				return 1;
			default:
				return 1;
			case 7:
			case 0xf:
				break;
			}
		}
		else {
			*pEventId = wEventId & 0x7fff;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004098e0
void* LEMBALL_FASTCALL RestoreLevelDemoPlaybackBaseVtable(void* pPlaybackController, void*, unsigned char fDelete)
{
	*(void***) pPlaybackController = g_GAME_RenderQueueNodeVtableSlots;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pPlaybackController);
	}
	return pPlaybackController;
}

// FUNCTION: LEMBALL 0x0045e4d0
int* LoadPresResource(int nResourceId)
{
	int* pResourceObject;
	int* pObjectWords;

	pResourceObject = (int*) ((CMogRes*) g_pMainResourceArchive)->Find(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != 0x50524553) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}
	pObjectWords = (int*) AllocateVSMemBlock(0x48);
	if (pObjectWords != 0) {
		*(void**) pObjectWords = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pObjectWords[6] = 0;
		*(void**) pObjectWords = g_DEMO_PresResourceVtable;
		InitializeResourceObjectFromId(pObjectWords, 0, nResourceId);
		return (int*) FinalizeLoadedResourceObjectResult(pObjectWords, 0);
	}
	return (int*) FinalizeLoadedResourceObjectResult(0, 0);
}

// FUNCTION: LEMBALL 0x0045e540
int* LoadBinResource(int nResourceId)
{
	int* pResourceObject;
	int* pObjectWords;

	pResourceObject =
		(int*) ((CMogRes*) g_pMainResourceArchive)->Find(nResourceId);
	if (pResourceObject != 0) {
		if ((unsigned int) pResourceObject[0x10] != g_DEMO_BinResourceTypeTag) {
			ReleaseTypedResourceObjectReference(pResourceObject);
			pResourceObject = 0;
		}
		return pResourceObject;
	}

	pObjectWords = (int*) AllocateVSMemBlock(0x48);
	if (pObjectWords != 0) {
		*(void**) pObjectWords = g_MOGLOAD_CachedResourceObjectBaseVtable;
		pObjectWords[6] = 0;
		*(void**) pObjectWords = g_DEMO_BinResourceVtable;
		InitializeResourceObjectFromId(pObjectWords, 0, nResourceId);
		return (int*) FinalizeLoadedResourceObjectResult(pObjectWords, 0);
	}
	return (int*) FinalizeLoadedResourceObjectResult(0, 0);
}

// FUNCTION: LEMBALL 0x0045ec60
void LEMBALL_FASTCALL SetBinResourceTypeTag(void* pObject)
{
	*(unsigned int*) ((char*) pObject + 0x40) = g_DEMO_BinResourceTypeTag;
}

// FUNCTION: LEMBALL 0x0045ec70
void* LEMBALL_FASTCALL DestroyBinResource(void* pObject, int, int fDelete)

{
	InitializeTypedResourceObjectBaseVtable(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

void* LEMBALL_FASTCALL DestroyPresResourceAuto(void* pObject, void*, BYTE fDelete)
{
	InitializeTypedResourceObjectBaseVtable(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00462ee0
FILE* OpenFileWithMode(const char* pszPath, const char* pszMode)
{
	return fopen(pszPath, pszMode);
}

// FUNCTION: LEMBALL 0x00462f20
void CloseCrtFilePointer(FILE* pFile)
{
	fclose(pFile);
}

// FUNCTION: LEMBALL 0x00462f30
unsigned int ReadFileBytes(FILE* pFile, void* pBuffer, size_t cbBuffer)
{
	return (unsigned int) fread(pBuffer, 1, cbBuffer, pFile);
}

// FUNCTION: LEMBALL 0x00462f80
unsigned int SeekFile(FILE* pFile, long lOffset, int nOrigin)
{
	fseek(pFile, lOffset, nOrigin);
	return (unsigned int) ftell(pFile);
}

// FUNCTION: LEMBALL 0x00462fb0
unsigned int TellFile(FILE* pFile)
{
	return (unsigned int) ftell(pFile);
}

// FUNCTION: LEMBALL 0x00462fc0
unsigned int GetFileLengthPreservingPosition(FILE* pFile)
{
	unsigned int nSavedOffset;
	unsigned int cbFile;

	nSavedOffset = TellFile(pFile);
	cbFile = SeekFile(pFile, 0, SEEK_END);
	SeekFile(pFile, (long) nSavedOffset, SEEK_SET);
	return cbFile;
}
