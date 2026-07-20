#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSGDI.H"
#include "GAME.H"
#include "SHELL/OPTIONS.H"

extern void LEMBALL_FASTCALL EnsureHelperGroup0BackingBuffer(int nHelperGroup);
extern void LEMBALL_FASTCALL ReleaseHelperGroup0BackingBuffer(int nHelperGroup);
extern void* g_GAME_PrimaryContextMenuDefinitionTable[];

struct GameHelperGroup0 {
	void SetBackingActive(int fActive);
};

// FUNCTION: LEMBALL 0x00431730
unsigned int __stdcall ComputePrimaryContextWindowStyleFlags(void)
{
	unsigned int uStyle;

	uStyle = 0x80001801;
	if (g_fCompactPrimaryContextLayout == 0) {
		if (GetSystemMetrics(0x3d) >= 0x281 && GetSystemMetrics(0x3e) >= 0x1f5) {
			uStyle = 0x80001b83;
		}
	}
	else {
		uStyle = 0x80001b83;
	}

	if (g_fStartupEditLevelOverride != 0) {
		uStyle |= 0x404;
	}
	return uStyle;
}

// FUNCTION: LEMBALL 0x00431780
void LEMBALL_FASTCALL PrepareLevelScreenRootHelperForRuntime(GamePrimaryContext* pPrimaryContext)
{
	void* pHelperGroup;

	pPrimaryContext->SetWindowOwnerScaleFactor(1);
	pPrimaryContext->ApplyWindowOwnerPaletteResource(0x2e);

	pHelperGroup = *(void**) ((char*) pPrimaryContext + 0x4c);
	pHelperGroup = (char*) *(void**) ((char*) pHelperGroup + 0x0c) + 0x98;
	((GameHelperGroup0*) pHelperGroup)->SetBackingActive(1);
	*(int*) ((char*) pPrimaryContext + 0xd4) = 0;
}

// FUNCTION: LEMBALL 0x00431eb0
int __stdcall GetPrimaryContextMenuDefinition(unsigned int* pMenuId, void** ppMenuDefinition)
{
	*pMenuId = 0x73;
	*ppMenuDefinition = g_GAME_PrimaryContextMenuDefinitionTable;
	return 1;
}

// FUNCTION: LEMBALL 0x00432530
void GamePrimaryContext::MarkNestedContextDirtyIfField20(void)
{
	int nNestedContext;
	int nDirtyTable;

	if (*(int*) ((char*) this + 0x20) == 0) {
		return;
	}

	nNestedContext = *(int*) (*(int*) ((char*) this + 0x4c) + 0x0c);
	nDirtyTable = *(int*) (nNestedContext + 0x40);
	*(int*) (*(int*) (nDirtyTable + 4) + nNestedContext + 0x78) = 1;
}

struct GameMenuDefinitionEntry {
	const char* m_pszText;
	unsigned int m_uCommandId;
	int m_nPosition;
	int m_fEnabled;
	int m_nReserved10;
	int m_nReserved14;
};

// GLOBAL: LEMBALL 0x0049e5f8
static GameMenuDefinitionEntry g_GAME_FileMenuDefinition[] = {
	{"&File", 0, 0, 1, 0, 0},
	{"E&xit", 0x9c41, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e640
static GameMenuDefinitionEntry g_GAME_OptionsMenuDefinition[] = {
	{"&Options", 0, 0, 1, 0, 0},
	{"Full Screen      F4", 0x9c4c, 5, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e688
static GameMenuDefinitionEntry g_GAME_HelpMenuDefinition[] = {
	{"&Help", 0, 0, 1, 0, 0},
	{"&Contents", 0x9c43, 2, 1, 0, 0},
	{"&Search Topic", 0x9c50, 3, 1, 0, 0},
	{"H&elp On Help", 0x9c4d, 6, 1, 0, 0},
	{"&About...", 0x9c4b, 4, 1, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

// GLOBAL: LEMBALL 0x0049e718
void* g_GAME_PrimaryContextMenuDefinitionTable[] = {
	g_GAME_FileMenuDefinition,
	g_GAME_OptionsMenuDefinition,
	g_GAME_HelpMenuDefinition,
	0,
};

// FUNCTION: LEMBALL 0x004654f0
unsigned int MapWindowOwnerFlagsToWin32Style(unsigned int uFlags)
{
	unsigned int uStyle;

	uStyle = (((unsigned int) ((uFlags & 8) == 0) - 1) & 0xcb0000) |
			 (((unsigned int) -(int) ((uFlags & 2) == 0) & 0x7f400000) + 0xc00000);
	uStyle |= (uFlags & 0x400) << 8;
	uStyle |= (uFlags & 0x40) << 10;
	uStyle |= (uFlags & 0x20) << 15;
	uStyle |= (uFlags & 0x10) << 17;
	uStyle |= (uFlags & 0x80) << 10;
	uStyle |= (uFlags & 0x100) << 11;
	return uStyle;
}

// FUNCTION: LEMBALL 0x00466540
void LEMBALL_FASTCALL EnsureHelperGroup0BackingBuffer(int nHelperGroup)
{
	unsigned short* pSize;
	short aDimensions[2];
	unsigned short aRect[2];
	int nHelperTarget;

	pSize = (unsigned short*) (*(int*) (*(int*) (nHelperGroup + 4) + 4) + nHelperGroup + 0x18);
	aRect[0] = pSize[0];
	aRect[1] = pSize[1];
	aDimensions[0] = 0;
	aDimensions[1] = 0;

	nHelperTarget = *(int*) (*(int*) (nHelperGroup + 4) + 4) + nHelperGroup;
	((VsGdiHelperSurface*) (unsigned long) (nHelperGroup + 8))
		->ComputeBackingDimensions(aDimensions, (short*) aRect, *(int*) (nHelperTarget + 0x44));
	aRect[0] = (unsigned short) aDimensions[0];
	aRect[1] = (unsigned short) aDimensions[1];

	if ((unsigned int) *(unsigned short*) (nHelperGroup + 0x50) *
			(unsigned int) *(unsigned short*) (nHelperGroup + 0x52) <
		(unsigned int) (short) aRect[0] * (unsigned int) (short) aRect[1]) {
		ReleaseHelperGroup0BackingBuffer(nHelperGroup);
	}

	if ((short) *(short*) (nHelperTarget + 0x18) * (short) *(short*) (nHelperTarget + 0x1a) != 0) {
		if (*(int*) (nHelperGroup + 0x4c) == 0) {
			*(unsigned short*) (nHelperGroup + 0x50) = aRect[0];
			*(unsigned short*) (nHelperGroup + 0x52) = aRect[1];
			*(int*) (nHelperGroup + 0x4c) =
				(int) (unsigned long) AllocateVSMemBlock((unsigned int) aRect[0] * (unsigned int) aRect[1]);
		}
		if (*(int*) (nHelperGroup + 0x4c) == 0) {
			*(int*) (nHelperGroup + 0x48) = 0;
		}
		((VsGdiHelperSurface*) (unsigned long) (nHelperGroup + 8))
			->ConfigureBackingStrideAndOrigin(*(int*) (nHelperGroup + 0x4c), (short) aRect[0]);
	}
}

// FUNCTION: LEMBALL 0x00466630
void GameHelperGroup0::SetBackingActive(int fActive)
{
	if (fActive == 0) {
		ReleaseHelperGroup0BackingBuffer((int) (unsigned long) this);
		*(int*) ((char*) this + 0x48) = 0;
	}
	else {
		EnsureHelperGroup0BackingBuffer((int) (unsigned long) this);
		*(int*) ((char*) this + 0x48) = 1;
	}
}
